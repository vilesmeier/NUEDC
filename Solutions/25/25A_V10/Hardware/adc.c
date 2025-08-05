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
    // A0 3PH voltage -- Inverter Uab
    state.V_ab    = (AdcMirror.ADCRESULT0 * 0.0007326007326f - 1.510083649f) * 29.9f;

    // B0 3PH current -- Inverter IA
    state.I_out_A = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.4769f) * 4.133939644f;

    // A1 3PH voltage -- Inverter Ubc
    state.V_bc    = (AdcMirror.ADCRESULT2 * 0.0007326007326f - 1.510850342f) * 29.9f;

    // B1 3PH current -- Inverter IC
    state.I_out_C = (AdcMirror.ADCRESULT3 * 0.0007326007326f - 1.521f) * 4.230834321;

    // A2 3PH voltage -- Rectifier Uca , rectifier state detection
    state.V_test  = (AdcMirror.ADCRESULT4 * 0.0007326007326f - 1.492966667f) * 31.172069825f;
//    state.V_test  = (AdcMirror.ADCRESULT4 * 0.0007326007326f);

    // B2 3PH Current -- Rectifier Ic
//    state.I_In_c  = (AdcMirror.ADCRESULT5 * 0.0007326007326f - 1.491127763f) * 4.694179105f;
    state.I_In_c  = (AdcMirror.ADCRESULT5 * 0.0007326007326f - 1.49588f) * 4.701010486f;

    // A3 DC  Voltage -- Capacitor precharge detect
    state.V_cap   = (AdcMirror.ADCRESULT6 * 0.0007326007326f) * 31.092176597f;
//    state.V_cap   = (AdcMirror.ADCRESULT6 * 0.0007326007326f);

    // B3 3PH Current -- Rectifier Ia
//    state.I_In_a  = (AdcMirror.ADCRESULT7 * 0.0007326007326f - 1.500890228f) * 4.697970079f;
    state.I_In_a  = (AdcMirror.ADCRESULT7 * 0.0007326007326f - 1.4975f) * 4.701597884;

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

    // get ref angle
    state.ref_a = SQUARE_2 * sinf(ramp.ref_angle);
    state.ref_c = SQUARE_2 * sinf(ramp.ref_angle+0.666667*PI);

    // data array to generate delay
    RMS_Push(&rms_A,state.ref_a);
    RMS_Push(&rms_C,state.ref_c);

    // 3ph power analyzer
    POWER_3ph_Set_Voltage(&power, state.V_A,    state.V_C   );
    POWER_3ph_Set_Current(&power, state.I_In_a, state.I_In_c);
    POWER_3ph_Run(&power);

    // inverter control logic ---------------------------------------------------

    // PR Voltage loop
    //Phase A
    PR_Run(&pr_vol_A, state.V_set_output * SQUARE_3_INV * state.ref_a, state.V_A, -state.I_out_A);
    //Phase C
    PR_Run(&pr_vol_C, state.V_set_output * SQUARE_3_INV * state.ref_c, state.V_C, -state.I_out_C);

    // SPWM - Modulate
    EPwm1Regs.CMPA.half.CMPA = 1875 * (pr_vol_A.out + 1.0f);
    EPwm2Regs.CMPA.half.CMPA = 1875 * (-pr_vol_A.out-pr_vol_C.out + 1.0f);
    EPwm3Regs.CMPA.half.CMPA = 1875 * (pr_vol_C.out + 1.0f);

    // Rectifier control logic ---------------------------------------------------

    // determain Rectifier state
    RMS_Run(&rms_Test,state.V_test);

    // rectifier power on logic
    if(rms_Test.sample_rms > 20.0f)
    {
        EPWM_AQ_Rectifier_SoftForce_LOW();
        if(state.V_cap > 40.0f)
        {
            // DC capacitor pre-charge
            state.flag.bits.FLAG_CAP_PRECHARGE = CAP_CHARGE_END;

            // Rectifier power on delay
            state.I_softstart = state.I_softstart + 1;

            // turn on rectifier after delay
            if(state.I_softstart > FREQ_ISR * 2.0f)
            {
                state.flag.bits.FLAG_REC_EN = MODE_ENABLE;
                EPWM_AQ_Rectifier_SoftForce_OFF();
            }
        }
    }

    // rectifier control logic
    if(state.flag.bits.FLAG_REC_EN == MODE_ENABLE)
    {
        // PQ control generate referance current

        // PR current loop
        //Phase A
        PR_Run(&pr_rec_a, state.I_set_output * state.ref_a, state.I_In_a, -state.V_A);
        //Phase C
        PR_Run(&pr_rec_c, state.I_set_output * state.ref_c, state.I_In_c, -state.V_C);

        // SPWM - Modulate
        EPwm4Regs.CMPA.half.CMPA = 1875 * (-pr_rec_a.out + 1.0f);
        EPwm5Regs.CMPA.half.CMPA = 1875 * (+pr_rec_a.out + pr_rec_c.out + 1.0f);
        EPwm6Regs.CMPA.half.CMPA = 1875 * (-pr_rec_c.out + 1.0f);
    }

    //Reference Angle Generate
    Ramp_Run(&ramp);

    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
