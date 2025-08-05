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
    AdcRegs.ADCMAXCONV.all = 0x0007;
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
    //A0 3PH voltag  -- ab
    state.V_ab  = (AdcMirror.ADCRESULT0 * 0.0007326007326f - 1.498647619f) * 31.072994818f;
    //B0 3PH current -- A
    state.I_A   = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.50627f) * 4.149549774f;
    //A1 3PH voltage -- bc
    state.V_bc  = (AdcMirror.ADCRESULT2 * 0.0007326007326f - 1.506980952f) * 31.267308703f;
    //B1 3PH current -- B
    state.I_B   = (AdcMirror.ADCRESULT3 * 0.0007326007326f - 1.500183151f)* 4.231381197f;
    //A2 3PH voltage -- ca
    state.V_ca  = (AdcMirror.ADCRESULT4 * 0.0007326007326f - 1.502966667f) * 31.172069825f;
    //B2 3PH current -- C
    state.I_C   = (AdcMirror.ADCRESULT5 * 0.0007326007326f - 1.501127763f)* 4.234179105f;

    //Line to Phase
    state.V_A = (2*state.V_ab+state.V_bc)*0.333333f;
    state.V_B = (2*state.V_bc+state.V_ca)*0.333333f;
    state.V_C = (2*state.V_ca+state.V_ab)*0.333333f;
}

/**
 * @brief Interrupt Service Routine For ADC
 * 
 * @return none
 */
interrupt void ADC_int_isr(void)
{
    ADC_Get_Value();

    //Phase A
    PR_Run(&pr_vol_A, state.V_set_output * SQUARE_2 * sinf(ramp.ref_angle), state.V_A, 0);
    //Phase C
    PR_Run(&pr_vol_C, state.V_set_output * SQUARE_2 * sinf(ramp.ref_angle+0.666667*PI), state.V_C, 0);

//    //Phase A
//    PR_Run(&pr_VC_A, pr_vol_A.voltage, state.I_A, 0);
//    //Phase C
//    PR_Run(&pr_VC_C, pr_vol_C.voltage, state.I_C, 0);

    //Modulate
//    EPwm1Regs.CMPA.half.CMPA = 1875 * (pr_vol_A.out + 1.0f);
//    EPwm2Regs.CMPA.half.CMPA = 1875 * (-pr_vol_A.out-pr_vol_C.out + 1.0f);
//    EPwm3Regs.CMPA.half.CMPA = 1875 * (pr_vol_C.out + 1.0f);

    //SVPWM
    SVPWM_Run(&svpwm, pr_vol_A.out, pr_vol_C.out);

    //Reference Angle Generate
    Ramp_Run(&ramp);


    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
