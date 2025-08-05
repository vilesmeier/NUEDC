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
    state.V_ab  = (AdcMirror.ADCRESULT0 * 0.0007326007326f -1.5f) * 31.0f;
    //B0 3PH current -- A
    state.I_A  = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.498f) * 4.166667f;
    //A1 3PH voltage -- bc
    state.V_bc  = (AdcMirror.ADCRESULT2 * 0.0007326007326f -1.5f) * 31.0f;
    //B1 3PH current -- B
    state.I_B  = (AdcMirror.ADCRESULT3 * 0.0007326007326f - 1.498f)* 4.166667f;
    //A2 3PH voltage -- ca
    state.V_ca  = (AdcMirror.ADCRESULT4 * 0.0007326007326f -1.5f) * 31.0f;
    //B2 3PH current -- C
    state.I_C  = (AdcMirror.ADCRESULT5 * 0.0007326007326f - 1.498f)* 4.166667f;

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

    if(state.Flag_Output_Mode == MODE_UNKNOWN)
    {
        RMS_Run(&rms_A, state.V_A);
        RMS_Run(&rms_B, state.V_B);
        RMS_Run(&rms_C, state.V_C);
        if(rms_A.index == 399)
        {
            if(rms_A.sample_rms >= 3.0f)
            {
                state.Flag_Output_Mode = MODE_CURRENT;
                state.Flag_Output_Enable = OUTPUT_DISABLE;
                EPWM_AQ_SoftForce_OFF();
            }
            else
            {
                state.Flag_Output_Mode = MODE_VOLTAGE;
                state.Flag_Output_Enable = OUTPUT_ENABLE;
                EPWM_AQ_SoftForce_OFF();
            }
        }
        else
        {
            state.Flag_Output_Mode = MODE_UNKNOWN;
            EPWM_AQ_SoftForce_LOW();
        }
    }

    //PR_VOLTAGE
    else if(state.Flag_Output_Mode == MODE_VOLTAGE)
    {
        //Phase A
        PR_Run(&pr_vol_A, state.V_set_output * SQUARE_2 * sinf(ramp.ref_angle), state.V_A, 0);
        //Phase C
        PR_Run(&pr_vol_C, state.V_set_output * SQUARE_2 * sinf(ramp.ref_angle+0.666667*PI), state.V_C, 0);

        //Phase A
        PR_Run(&pr_VC_A, pr_vol_A.voltage, state.I_A, 0);
        //Phase C
        PR_Run(&pr_VC_C, pr_vol_C.voltage, state.I_C, 0);

        //Modulate
//        EPwm1Regs.CMPA.half.CMPA = 1875 * (pr_VC_A.voltage + 1.0f);
//        EPwm2Regs.CMPA.half.CMPA = 1875 * (-pr_VC_A.voltage-pr_VC_C.voltage + 1.0f);
//        EPwm3Regs.CMPA.half.CMPA = 1875 * (pr_VC_C.voltage + 1.0f);

        //SVPWM
        SVPWM_Run(&svpwm, pr_VC_A.voltage, pr_VC_C.voltage);

        //Reference Angle Generate
        Ramp_Run(&ramp);
    }

    //PR_CURRENT
    else if(state.Flag_Output_Mode == MODE_CURRENT)
    {
        RMS_Run(&rms_A, state.V_A);
        RMS_Run(&rms_B, state.V_B);
        RMS_Run(&rms_C, state.V_C);

        //SoftStart
        if(state.Flag_Soft_Start == SOFT_START)
        {
            state.I_softstart = state.I_softstart + 0.000001f;
        }

        if((state.I_softstart >= state.I_set_output)&&(state.Flag_Soft_Start == SOFT_START))
        {
            state.Flag_Soft_Start = NORMAL;
        }

        //PR Control
        if(state.Flag_Soft_Start == SOFT_START)
        {
            state.pr_I_set    = state.I_softstart;
        }
        else if(state.Flag_Soft_Start == NORMAL);
        {
            state.pr_I_set    = state.I_set_output;
        }
        //Phase A
        PR_Run(&pr_cur_A, state.pr_I_set * rms_A.angle, state.I_A, 0);
        //Phase C
        PR_Run(&pr_cur_C, state.pr_I_set * rms_C.angle, state.I_C, 0);

        //Modulate
        EPwm1Regs.CMPA.half.CMPA = 1875 * (pr_cur_A.out + 1.0f);
        EPwm2Regs.CMPA.half.CMPA = 1875 * (-pr_cur_A.out-pr_cur_C.out + 1.0f);
        EPwm3Regs.CMPA.half.CMPA = 1875 * (pr_cur_C.out + 1.0f);

    }




    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
