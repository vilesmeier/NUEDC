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
    // A0 3PH voltag  -- Inverter ab
    state.V_ab  = (AdcMirror.ADCRESULT0 * 0.0007326007326f - 1.498647619f) * 31.072994818f;

    // B0 3PH current -- Rectifier A
    state.I_In_a   = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.50627f) * 4.149549774f;

    // A1 3PH voltage -- Inverter bc
    state.V_bc  = (AdcMirror.ADCRESULT2 * 0.0007326007326f - 1.506980952f) * 31.267308703f;

    // B1 3PH current -- Rectifier B
    state.I_In_c   = (AdcMirror.ADCRESULT3 * 0.0007326007326f - 1.500183151f)* 4.231381197f;

    // A2 3PH voltage -- Rectifier ca
    state.V_test  = (AdcMirror.ADCRESULT4 * 0.0007326007326f - 1.502966667f) * 31.172069825f;

    // B2

    // A3 DC Capacitor precharge detect
    state.V_cap = (AdcMirror.ADCRESULT6 * 0.0007326007326f) * 31.092176597;

    //Line to Phase
    state.V_A = ( 2 * state.V_ab +    state.V_bc) * 0.333333f;
    state.V_C = (    -state.V_ab - 2* state.V_bc) * 0.333333f;
}

/**
 * @brief Interrupt Service Routine For ADC
 * 
 * @return none
 */
interrupt void ADC_int_isr(void)
{
    // data process
    ADC_Get_Value();

    RMS_Run(&rms_A,state.V_A);
    RMS_Run(&rms_C,state.V_C);

    // get ref angle
    state.ref_a = SQUARE_2 * sinf(ramp.ref_angle);
    state.ref_c = SQUARE_2 * sinf(ramp.ref_angle+0.666667*PI);

    // inverter control logic ---------------------------------------------------

    //Phase A
    PR_Run(&pr_vol_A, state.V_set_output * state.ref_a, state.V_A, 0);
    //Phase C
    PR_Run(&pr_vol_C, state.V_set_output * state.ref_c, state.V_C, 0);
//    //Phase A
//    PR_Run(&pr_VC_A, pr_vol_A.voltage, state.I_A, 0);
//    //Phase C
//    PR_Run(&pr_VC_C, pr_vol_C.voltage, state.I_C, 0);

    //SVPWM - Modulate
    SVPWM_Run(&svpwm, pr_vol_A.out, pr_vol_C.out);
    EPwm1Regs.CMPA.half.CMPA = 3750 * (svpwm.Ua);
    EPwm2Regs.CMPA.half.CMPA = 3750 * (svpwm.Ub);
    EPwm3Regs.CMPA.half.CMPA = 3750 * (svpwm.Uc);



    // SPWM - Modulate
//    EPwm1Regs.CMPA.half.CMPA = 1875 * (pr_vol_A.out + 1.0f);
//    EPwm2Regs.CMPA.half.CMPA = 1875 * (-pr_vol_A.out-pr_vol_C.out + 1.0f);
//    EPwm3Regs.CMPA.half.CMPA = 1875 * (pr_vol_C.out + 1.0f);

    // Rectifier control logic ---------------------------------------------------

    // determain Rectifier state
    RMS_Run(&rms_Test,state.V_test);

    if(rms_Test.sample_rms > 5.0f)
    {
        state.flag.bits.FLAG_REC_EN = MODE_ENABLE;
    }
    else
    {
        state.flag.bits.FLAG_REC_EN = MODE_DISABLE;
    }

    if(state.V_cap > 45.0f)
    {
        state.flag.bits.FLAG_CAP_PRECHARGE = CAP_CHARGE_END;
    }
    else
    {
        state.flag.bits.FLAG_CAP_PRECHARGE = CAP_NOT_CHARGE;
    }

    // rectifier control logic
    if((state.flag.bits.FLAG_REC_EN == MODE_ENABLE)&& (state.flag.bits.FLAG_CAP_PRECHARGE == CAP_CHARGE_END))
    {
        //SoftStart
        if(state.flag.bits.I_softstart == SOFT_START)
        {
            state.I_softstart = state.I_softstart + 0.00001f;
        }

        if((state.I_softstart >= state.I_set_output)&&(state.flag.bits.I_softstart == SOFT_START))
        {
            state.flag.bits.I_softstart = NORMAL;
        }

        //PR Control
        if(state.flag.bits.I_softstart == SOFT_START)
        {
            state.pr_I_set    = state.I_softstart;
        }
        else
        {
            state.pr_I_set    = state.I_set_output;
        }

        //Phase A
        PR_Run(&pr_rec_a, state.pr_I_set * state.ref_a, state.I_In_a, -state.V_A);
        //Phase C
        PR_Run(&pr_rec_c, state.pr_I_set * state.ref_c, state.I_In_c, -state.V_C);

        // SPWM - Modulate
//        EPwm4Regs.CMPA.half.CMPA = 1875 * (-pr_rec_a.out + 1.0f);
//        EPwm5Regs.CMPA.half.CMPA = 1875 * (+pr_rec_a.out+pr_rec_c.out + 1.0f);
//        EPwm6Regs.CMPA.half.CMPA = 1875 * (-pr_rec_c.out + 1.0f);

        // SVPWM - Modulate
        SVPWM_Run(&svpwm_rec, -pr_rec_a.out, -pr_rec_c.out);
        EPwm4Regs.CMPA.half.CMPA = 3750 * (svpwm.Ua);
        EPwm5Regs.CMPA.half.CMPA = 3750 * (svpwm.Ub);
        EPwm6Regs.CMPA.half.CMPA = 3750 * (svpwm.Uc);
    }

    //Reference Angle Generate
    Ramp_Run(&ramp);

    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
