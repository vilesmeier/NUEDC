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
    state.V_Input  = (AdcMirror.ADCRESULT0 * 0.0007326007326f - 1.49998f) * 31.0f;
    //ADCResult1 - Input     AC Current - B0
    state.I_Input  = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.49998f) * 4.166667f;
    //ADCResult2 - Rectifier DC Voltage - A1
    state.V_DC     = (AdcMirror.ADCRESULT2 * 0.0007326007326f) * 31.0f;
    //ADCResult3 - Rectifier DC Current - B1
    state.I_DC     = (AdcMirror.ADCRESULT3 * 0.0007326007326f - 1.49998f) * 4.166667f;
}



/**
 * @brief Interrupt Service Routine For ADC
 * 
 * @return none
 */
interrupt void ADC_int_isr(void)
{
    // get real values
    ADC_Get_Value();

    // get grid RMS
    RMS_Run(&rms_Vol_Input, state.V_Input);

    // get phase ref signal
    RMS_Push(&rms_Vol_Ref,rms_Vol_Input.angle);

    if(state.Flag_Input_Enable == INPUT_DISABLE)
       {
           if(rms_Vol_Input.index == 399)
           {
               state.Flag_Input_Enable = INPUT_ENABLE;
               Rectifier_ON();
           }
       }

    //control logic
    if(state.Flag_Input_Enable == INPUT_ENABLE)
    {
        // 1ph power analyzer
        POWER_1ph_Run(&power_v, state.V_Input, state.I_Input);

        // gengerate Qref
        state.Q = power_v.pRms * sqrtf(1/(state.pf_set*state.pf_set) - 1);

        // I_Q ref amplitude generate
        state.I_var = state.Q / rms_Vol_Input.sample_rms;

        // I_Q ref signal generate
        state.I_var = state.I_var * RMS_Delay(&rms_Vol_Ref, 0.5f * PI);

        // Filter
        NOTCH_Run(&notch, state.V_DC);

        // PI Voltage Loop
        PID_Run(&pi_DC, state.V_DC_Set, notch.result, 0);

        // generate Iref by PQ control
        state.I_ref = pi_DC.voltage * RMS_Current_Value(&rms_Vol_Ref) + state.I_var;

        // PR Current Loop
        PR_Run(&pr_Input, state.I_ref, state.I_Input, -state.V_Input);

        EPwm1Regs.CMPA.half.CMPA = 1875 * (1 - pr_Input.out);
        EPwm2Regs.CMPA.half.CMPA = 1875 * (1 + pr_Input.out);
    }

    if(state.V_DC > 90.0f)
    {
        Rectifier_OFF();
    }


    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
