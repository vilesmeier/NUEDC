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
    state.V_output = (AdcMirror.ADCRESULT0 * 0.0007326007326f - 1.49998f) * 31.0f;
    state.I_output = (AdcMirror.ADCRESULT1 * 0.0007326007326f - 1.49998f) * 4.166667f;

    Filter_RC_LPF_Run(&rc, state.V_output);
}

/**
 * @brief Interrupt Service Routine For ADC
 * 
 * @return none
 */
interrupt void ADC_int_isr(void)
{
    ADC_Get_Value();

    //state machine
    //get grid state,determain output mode
    if(state.Flag_Output_Mode == MODE_UNKNOWN)
    {
        RMS_Run(&Grid_state, rc.result[0]);
        EPWM_OFF();
        if(Grid_state.index == 399)
        {
            if(Grid_state.sample_rms >= 3.0f)
            {
                state.Flag_Output_Mode = MODE_CURRENT;
                state.Flag_Output_Enable = OUTPUT_DISABLE;
                EPWM_OFF();
            }
            else
            {
                state.Flag_Output_Mode = MODE_VOLTAGE;
                state.Flag_Output_Enable = OUTPUT_ENABLE;
                EPWM_ON();
            }
        }
        else
        {
            state.Flag_Output_Mode = MODE_UNKNOWN;
        }
    }

    //VOLTAGE source inverter , off grid
    else if(state.Flag_Output_Mode == MODE_VOLTAGE)
    {
        if(state.Flag_Soft_Start == SOFT_START)
        {
            state.V_softstart = state.V_softstart + 0.0012f;
        }

        if((state.V_softstart > state.V_set_rms)&&(state.Flag_Soft_Start == SOFT_START))
        {
            state.Flag_Soft_Start = NORMAL;
        }

        //pr
        if(state.Flag_Soft_Start == SOFT_START)
        {
            state.V_GiveValue    = state.V_softstart;
        }
        else if(state.Flag_Soft_Start == NORMAL)
        {
            state.V_GiveValue    = state.V_set_rms;
        }
        //PR
        PR_Run(&pr_Vol,state.V_GiveValue * SQUARE_2 * sinf(ramp.ref_angle),rc.result[0],-state.I_output);

//        //Modulate
        EPwm1Regs.CMPA.half.CMPA = 1875 * (1 + pr_Vol.out); //EPwm1Regs.TBPRD * 0.5f
        EPwm2Regs.CMPA.half.CMPA = 1875 * (1 - pr_Vol.out);



        //Reference Angle Generate
        Ramp_Run(&ramp);
    }

    //CURRENT source inverter, on grid
    else if(state.Flag_Output_Mode == MODE_CURRENT)
    {
        //RMS get angle
        RMS_Run(&Grid_state,state.V_output);
        state.Flag_Output_Enable = OUTPUT_ENABLE;
        EPWM_ON();

        //open output after get the grid phase
        if(state.Flag_Output_Enable == OUTPUT_ENABLE)
        {
            if(state.Flag_Soft_Start == SOFT_START)
            {
                state.I_softstart = state.I_softstart + 0.000025f;
            }

            if((state.I_softstart > state.I_set_rms)&&(state.Flag_Soft_Start == SOFT_START))
            {
                state.Flag_Soft_Start = NORMAL;
            }

            //pr
            if(state.Flag_Soft_Start == SOFT_START)
            {
                state.I_GiveValue    = state.I_softstart * Grid_state.angle;
            }
            else if(state.Flag_Soft_Start == NORMAL)
            {
                state.I_GiveValue    = state.I_set_rms * Grid_state.angle;
            }
            PR_Run(&pr_Cur, state.I_GiveValue, state.I_output, state.V_output);

            //Modulate
            EPwm1Regs.CMPA.half.CMPA = 1875 * (1 + pr_Cur.out); //EPwm1Regs.TBPRD * 0.5f
            EPwm2Regs.CMPA.half.CMPA = 1875 * (1 - pr_Cur.out);
        }
    }
    else
    {
        EPWM_OFF();
    }

    //Interrupt Clear
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
