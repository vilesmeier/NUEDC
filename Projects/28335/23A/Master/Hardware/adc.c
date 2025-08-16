#include "global.h"


/**
 * @brief ADC Init
 * 
 */..
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
    state.V_output = ((AdcRegs.ADCRESULT0 >> 4) * 0.000732422f - 1.49415f ) * 23.0f;
    state.I_output = ((AdcRegs.ADCRESULT1 >> 4) * 0.000732422f - 1.48850f ) * 4.166667f;
//    state.V_output = ((AdcRegs.ADCRESULT0 >> 4 + AdcRegs.ADCRESULT2 >> 4) * 0.5f * 0.000732422f - 1.49415f ) * 23.0f;
//    state.I_output = ((AdcRegs.ADCRESULT1 >> 4 + AdcRegs.ADCRESULT3 >> 4) * 0.5f * 0.000732422f - 1.49895f ) * 3.33333f;
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
        RMS_Run(&Grid_state, state.V_output);
        if(Grid_state.index == 0)
        {
            if(Grid_state.sample_rms >= 5.0f)
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

    //voltage source inverter , off grid
    else if(state.Flag_Output_Mode == MODE_VOLTAGE)
    {
        //PR
        pr_Vol.ActuralValue = state.V_output;
        pr_Vol.GiveValue    = state.V_set_rms * SQUARE_2 * sinf(ramp.ref_angle);
        PR_Run(pr_Vol_Addr);

        //Modulate
        EPwm1Regs.CMPA.half.CMPA = 1875 * (1 + pr_Vol.Result); //EPwm1Regs.TBPRD * 0.5f
        EPwm2Regs.CMPA.half.CMPA = 1875 * (1 - pr_Vol.Result);

    }


    //current source inverter, on grid
    else if(state.Flag_Output_Mode == MODE_CURRENT)
    {
        //SOGI
        SPLL_1PH_SOGI_run(sogi_Vol_Addr,state.V_output);

        //Determine PLL Status
        if((sogi_Vol.u_Q[0] <= 0.1) && (sogi_Vol.u_Q[0] >= -0.1))
        {
            //turn on output after PLL get grid phase
            state.Flag_Output_Enable = OUTPUT_ENABLE;
            state.Flag_Soft_Start = SOFT_START;
            EPWM_ON();
        }

        //open output after get the grid phase
        if(state.Flag_Output_Enable == OUTPUT_ENABLE)
        {
            if(state.Flag_Soft_Start == SOFT_START)
            {
                state.I_softstart = state.I_softstart + 0.005;
            }

            if((state.I_softstart > state.I_set_rms)&&(state.Flag_Soft_Start == SOFT_START))
            {
                state.Flag_Soft_Start = NORMAL;
            }

            //pr
            if(state.Flag_Soft_Start == SOFT_START)
            {
                pr_Cur.GiveValue    = state.I_softstart * SQUARE_2 * sogi_Vol.sine;
            }
            else if(state.Flag_Soft_Start == NORMAL)
            {
                pr_Cur.GiveValue    = state.I_set_rms * SQUARE_2 * sogi_Vol.sine;
            }
            pr_Cur.ActuralValue = state.I_output;
            PR_Run(pr_Cur_Addr);

            //Modulate
            EPwm1Regs.CMPA.half.CMPA = 1875 * (1 + pr_Cur.Result); //EPwm1Regs.TBPRD * 0.5f
            EPwm2Regs.CMPA.half.CMPA = 1875 * (1 - pr_Cur.Result);
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
