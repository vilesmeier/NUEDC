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
    state.V_Input  = ((AdcRegs.ADCRESULT0 >> 4) * 0.000732421875f - 1.5f) * 41.15f;
    //ADCResult1 - Input     AC Current - B0
    state.I_Input  = ((AdcRegs.ADCRESULT1 >> 4) * 0.000732421875f - 1.5f) * 4.166667f;
    //ADCResult2 - Rectifier DC Voltage - A1
    state.V_DC     = ((AdcRegs.ADCRESULT2 >> 4) * 0.000732421875f) * 31.0f;
    //ADCResult3 - Rectifier DC Current - B1
    state.I_DC     = ((AdcRegs.ADCRESULT3 >> 4) * 0.000732421875f - 1.5f) * 4.166667f;
    //ADCResult4 - Inverter  AC Voltage - A2
    state.V_Output = ((AdcRegs.ADCRESULT0 >> 4) * 0.000732421875f - 1.5f) * 41.15f;
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
    //get real values
    ADC_Get_Value();

    state.Flag_Input_Enable = INPUT_ENABLE;
    Rectifier_ON();

    //Rectifier control logic
    if(state.Flag_Input_Enable == INPUT_ENABLE)
    {
        RMS_Run(&rms_Vol_Input, state.V_Input);

        //Filter
        NOTCH_Run(&notch, state.V_DC);

        //DC Voltage Loop
        pi_DC.GiveValue = state.V_DC_Set;
        pi_DC.ActuralValue = notch.result;
        PID_Run(&pi_DC);

        PR_Run(&pr_Input, pi_DC.voltage / rms_Vol_Input.sample_rms * state.V_Input, state.I_Input, -state.V_Input);

//        PR_Run(&pr_Input, SQUARE_2 * pi_DC.voltage * sogi_Input.sine, state.I_Input, -state.V_Input);

        EPwm1Regs.CMPA.half.CMPA = 1875 * (1 - pr_Input.out);
        EPwm2Regs.CMPA.half.CMPA = 1875 * (1 - pr_Input.out);


        if(state.V_DC > 90.0f)
        {
            Rectifier_OFF();
        }
    }

    state.Flag_Output_Enable = !state.Flag_Output_Enable;
    if(state.Flag_Output_Enable)
    {
        //VOFA debug probe
        UART_VOFA_1Ch_ShowFloat(pr_Input.voltage);
//        UART_VOFA_2Ch_ShowFloat(pi_DC.voltage,pr_Input.out);
    }


    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
