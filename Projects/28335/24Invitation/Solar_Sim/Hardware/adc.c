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
    //A0 DC output voltage
    state.V_output = AdcMirror.ADCRESULT0 * 0.0007326007326f * 31.0f;
    //B0 DC output current
    state.I_output = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.498f) * 4.166667f;
    //A1 DC input voltage
    state.V_input  = AdcMirror.ADCRESULT2 * 0.0007326007326f * 31.0f;
    //A1 DC output current
    state.I_input  = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.5f) * 4.166667f;
}

/**
 * @brief Interrupt Service Routine For ADC
 * 
 * @return none
 */
interrupt void ADC_int_isr(void)
{
    //get current output
    ADC_Get_Value();
    //stage judge
    if(state.V_output < 20.0f)
    {
        state.MODE = MODE_CURRENT;
    }
    if( (state.V_output >= 20.0f) && (state.V_output < 25.0f))
    {
        state.MODE = MODE_SLOPE_LOW;
    }
    if(state.V_output >= 25.0f)
    {
        state.MODE = MODE_SLOPE_HIGH;
    }
    //PID set
    if(state.MODE == MODE_CURRENT)
    {
        state.pi_V_GiveValue = state.I_set_output;
//        PID_Set(&pid_vol, 1.0f, 1.0f, 0.0f, 0, 40, 0, 750, 20000);
        PID_Run(&pid_vol, state.pi_V_GiveValue, state.I_output, 0);
    }
    if(state.MODE == MODE_SLOPE_LOW)
    {
        state.slope_INV = 25;//slope = 0.04
        state.I0    = 3;
        state.pi_V_GiveValue = (state.I0 - state.I_output) * state.slope_INV;
//        PID_Set(&pid_vol, 1.0f, 1.0f, 0.0f, 0, 40, 0, 750, 20000);
        PID_Run(&pid_vol, state.pi_V_GiveValue, state.V_output, 0);
    }
    if(state.MODE == MODE_SLOPE_HIGH)
    {
        state.slope_INV = 2.5f;//slope = 0.4
        state.I0    = 12;
        state.pi_V_GiveValue = (state.I0 - state.I_output) * state.slope_INV;
//        PID_Set(&pid_vol, 1.0f, 1.0f, 0.0f, 0, 40, 0, 750, 20000);
        PID_Run(&pid_vol, state.pi_V_GiveValue, state.V_output, 0);
    }

    //modulate
    EPwm1Regs.CMPA.half.CMPA = 3750 * 0.95f * pid_vol.voltage / state.V_input;
    EPwm2Regs.CMPA.half.CMPA = 3750 * 0.95f;//桥臂2，上常开，下常闭




    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
