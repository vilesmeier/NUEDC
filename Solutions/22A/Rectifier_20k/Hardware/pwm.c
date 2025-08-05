
#include "global.h"

/**
 * @brief PWM Init
 * 
 */
void EPWM_Init(void)
{

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;   // Disable TBCLK within the ePWM
    EDIS;

    InitEPwmGpio();

    DINT;
    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_isr;
    IER |= M_INT3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    EDIS;
    EINT;
    ERTM;

    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBPHS.half.TBPHS = 0;
    EPwm1Regs.TBCTR = 0x0000;
    EPwm1Regs.TBPRD = 3750;
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPA.half.CMPA = 1875;
    EPwm1Regs.CMPB = 1875;

    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CAD = AQ_SET;
//    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
//    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
//    EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;

//    //DB
//    EPwm1Regs.DBCTL.all = 0xb;
//    EPwm1Regs.DBRED = 100;
//    EPwm1Regs.DBFED = 100;

    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;                 //0£ºDISABLE;1£ºENABLE
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;
//    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
//    EPwm1Regs.ETSEL.bit.INTEN = 1;
//    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;

    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm2Regs.TBPHS.half.TBPHS = 0;
    EPwm2Regs.TBCTR = 0x0000;
    EPwm2Regs.TBPRD = 3750;
    EPwm2Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm2Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPA.half.CMPA = 1875;
    EPwm2Regs.CMPB = 1875;

    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CAD = AQ_SET;
//    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
//    EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
//    EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;

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
//    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
//    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;
//    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;
//    EPwm3Regs.AQCTLB.bit.CAD = AQ_CLEAR;

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
 * @brief  Turn on PWM output of Front Rectifier
 * 
 */
void Rectifier_ON(void)
{
    EPwm1Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm2Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
}

/**
 * @brief Turn off PWM output of Front Rectifier
 * 
 */
void Rectifier_OFF(void)
{
    EPwm1Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
}

/**
 * @brief Turn on PWM output of back Inverter
 * 
 */
void Inverter_ON(void)
{
    EPwm3Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm4Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
}

/**
 * @brief Turn off PWM output of back Inverter
 * 
 */
void Inverter_OFF(void)
{
    EPwm3Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm4Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
}

/**
 * @brief PWM1 Interrupt Service Routine
 * 
 * @return 
 */
interrupt void epwm1_isr(void)
{

    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}
