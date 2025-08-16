
#include "global.h"

/**
 * @brief Init ePWM1,select ePWM1 as event trigger and sync source
 * 
 */
void EPWM1_Init(void)
{
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBPHS.half.TBPHS = 0;
    EPwm1Regs.TBCTR = 0x0000;
    EPwm1Regs.TBPRD = 1875;
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPA.half.CMPA = 938;
    EPwm1Regs.CMPB = 938;

    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;

    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;
//    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
//    EPwm1Regs.ETSEL.bit.INTEN = 1;
//    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;
}

/**
 * @brief Init ePWM2
 * 
 */
void EPWM2_Init(void)
{
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm2Regs.TBPHS.half.TBPHS = 0;
    EPwm2Regs.TBCTR = 0x0000;
    EPwm2Regs.TBPRD = 1875;
    EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPA.half.CMPA = 938;
    EPwm2Regs.CMPB = 938;

    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;
}

/**
 * @brief Init ePWM3
 * 
 */
void EPWM3_Init(void)
{
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm3Regs.TBPHS.half.TBPHS = 0;
    EPwm3Regs.TBCTR = 0x0000;
    EPwm3Regs.TBPRD = 3750;
    EPwm3Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm3Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm3Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPA.half.CMPA = 1875;
    EPwm3Regs.CMPB = 1875;

    EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CAU = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CAD = AQ_SET;
}

/**
 * @brief Init ePWM4
 * 
 */
void EPWM4_Init(void)
{
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm4Regs.TBPHS.half.TBPHS = 0;
    EPwm4Regs.TBCTR = 0x0000;
    EPwm4Regs.TBPRD = 3750;
    EPwm4Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm4Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm4Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm4Regs.CMPA.half.CMPA = 1875;
    EPwm4Regs.CMPB = 1875;

    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CAU = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CAD = AQ_SET;
}

/**
 * @brief Init ePWM1-4 ,GPIO for ePWM1-4 ,init interrupt of ePWM
 * 
 */
void EPWM_Init(void)
{

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // Disable TBCLK within the ePWM
    EDIS;

    //init GPIO for ePWM1-6
    InitEPwmGpio();

    //init system interrupt
    DINT;
    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_isr;
    IER |= M_INT3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    EDIS;
    EINT;
    ERTM;

    //init ePWM
    EPWM1_Init();
    EPWM2_Init();
    EPWM3_Init();
    EPWM4_Init();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

/**
 * @brief Turn on PWM output by AQ submodule
 *
 */
void EPWM_ON(void)
{
    EPwm1Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm2Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm3Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm4Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
}

/**
 * @brief Shut down PWM output by AQ submodule
 * 
 */
void EPWM_OFF(void)
{
    EPwm1Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm4Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
}

/**
 * @brief PWM1 Interrupt Service Routine
 * 
 * @return none
 */
interrupt void epwm1_isr(void)
{

    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
