#include "global.h"

/**
 * @brief ADC Init
 * 
 */
void ADC_Init(void)
{
    EALLOW;
    DINT;
    PieVectTable.ADCINT = &ADC_int_isr;
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;

    InitAdc();

    AdcRegs.ADCTRL1.bit.ACQ_PS = 0x1;
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x3;
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 0x1;
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0x1;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0x1;

    //ADC转换设置
    AdcRegs.ADCMAXCONV.all = 0x0002;
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;
    AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;
    AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;
    AdcRegs.ADCCHSELSEQ2.bit.CONV04 = 0x4;
    AdcRegs.ADCCHSELSEQ2.bit.CONV05 = 0x5;
    AdcRegs.ADCCHSELSEQ2.bit.CONV06 = 0x6;
    AdcRegs.ADCCHSELSEQ2.bit.CONV07 = 0x7;

    //ADC中断设置
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0x0;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;

    EDIS;
    EINT;
    ERTM;
}

/**
 * @brief Signal Process
 * 
 */
void ADC_Get_Value(void)
{
    //ADCResult0 - Input     AC Voltage - A0
    state.V_Input  = ((AdcRegs.ADCRESULT0 >> 4) * 0.000732421875f - 1.5f) * 31.0f;
    //ADCResult1 - Input     AC Current - B0
    state.I_Input  = ((AdcRegs.ADCRESULT1 >> 4) * 0.000732421875f - 1.5f) * 4.166667f;
    //ADCResult2 - Rectifier DC Voltage - A1
    state.V_DC     = ((AdcRegs.ADCRESULT2 >> 4) * 0.000732421875f - 1.5f) * 31.0f;
    //ADCResult3 - Rectifier DC Current - B1
    state.I_DC     = ((AdcRegs.ADCRESULT3 >> 4) * 0.000732421875f - 1.5f) * 4.166667f;
    //ADCResult4 - Inverter  AC Voltage - A2
    state.V_Output = ((AdcRegs.ADCRESULT0 >> 4) * 0.000732421875f - 1.5f) * 31.0f;
    //ADCResult5 - Inverter  AC Current - B2
    state.I_output = ((AdcRegs.ADCRESULT1 >> 4) * 0.000732421875f - 1.5f) * 4.166667f;
}

/**
 * @brief Interrupt Service Routine For ADC
 * 
 * @return none
 */
interrupt void ADC_int_isr(void)
{
    ADC_Get_Value();

    SOGI_Run(&sogi_Input,state.V_Input);

    //Capacitor Over Voltage Protection
    if(state.V_DC >= 90)
    {
        Inverter_OFF();
        Rectifier_OFF();
    }

    //determain PLL status, turn on input after get grid phase
    if(fabs(sogi_Input.u_Q[0]) <= 0.1)
    {
        state.Flag_Input_Enable = INPUT_ENABLE;
        Rectifier_ON();
    }

    //enable output feedback after establish DC Voltage
    if(state.V_DC >= 0.9f * state.V_DC_Set)
    {
        state.Flag_Output_Enable = OUTPUT_ENABLE;
        Inverter_ON();
    }

    //output Inverter softstart
    if(state.Flag_Output_SoftStart == SOFT_START && state.Flag_Output_Enable == OUTPUT_ENABLE)
    {
        state.V_Output_Softstart = state.V_Output_Softstart + 0.0001;
        if(state.V_Output_Softstart > state.V_Output_Set_RMS)
        {
            state.Flag_Output_SoftStart = NORMAL;
        }
    }

    //input recifier softstart
    if(state.Flag_Input_SoftStart == SOFT_START && state.Flag_Input_Enable == INPUT_ENABLE)
    {
        state.I_Input_Softstart = state.I_Input_Softstart + 0.0001;
        if(state.I_Input_Softstart > state.I_Input_Set_RMS)
        {
            state.Flag_Input_SoftStart = NORMAL;
        }
    }

    //Rectifier control logic
    if(state.Flag_Input_Enable == INPUT_ENABLE)
    {
        //delay for OSG + Park transform
        RMS_Push(&rms_Input_Cur,state.I_Input);
        if(rms_Input_Cur.index >= 100)
        {
            dq_Input.beta = rms_Input_Cur.data[rms_Input_Cur.index - 100];
        }
        else
        {
            dq_Input.beta = rms_Input_Cur.data[rms_Input_Cur.index + 300];
        }
        dq_Input.alpha = state.I_Input;
        dq_Input.sin = -sogi_Input.cosine;
        dq_Input.cos = sogi_Input.sine;
        Park_Run(&dq_Input);

        //Sync PI
        pi_Input_Cur_D.ActuralValue = dq_Input.d;
        pi_Input_Cur_Q.ActuralValue = dq_Input.q;
        if(state.Flag_Output_SoftStart == SOFT_START)
        {
            pi_Input_Cur_D.GiveValue = state.I_Input_Softstart * state.PF;
            pi_Input_Cur_Q.GiveValue = state.I_Input_Softstart * sqrtf(1 - state.PF *state.PF);
        }
        else
        {
            pi_Input_Cur_D.GiveValue = state.I_Input_Set_RMS * state.PF;
            pi_Input_Cur_Q.GiveValue = state.I_Input_Set_RMS * sqrtf(1 - state.PF *state.PF);
        }
        PID_Run(&pi_Input_Cur_D);
        PID_Run(&pi_Input_Cur_Q);

        //inverse park transform
        dq_Input.d = pi_Input_Cur_D.voltage;
        dq_Input.q = pi_Input_Cur_Q.voltage;
        Inverse_Park_Run(&dq_Input);

        //modulate
        EPwm1Regs.CMPA.half.CMPA = 1875 * (1 + dq_Input.alpha);
        EPwm2Regs.CMPA.half.CMPA = 1875 * (1 - dq_Input.alpha);
    }

    //inverter control logic
    if(state.Flag_Output_Enable == OUTPUT_ENABLE)
    {
        //PI control for DC voltage
        pi_DC.ActuralValue = -state.V_DC;
        pi_DC.GiveValue    = -state.V_DC_Set;
        PID_Run(&pi_DC);

        //PR control for inverter output current
        pr_Output_Cur.GiveValue    = (pi_DC.voltage + pi_DC.pid_max) * 0.5f * sogi_Input.sine;
        pr_Output_Cur.ActuralValue = state.I_output;
        PR_Run(&pr_Output_Cur);

        //modulate
        EPwm3Regs.CMPA.half.CMPA = 1875 * (1 + pr_Output_Cur.Result);
        EPwm4Regs.CMPA.half.CMPA = 1875 * (1 - pr_Output_Cur.Result);
    }

    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
