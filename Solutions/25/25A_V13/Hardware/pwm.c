#include "global.h"

/**
 * @brief Init ePWM1,select ePWM1 as event trigger and sync source
 * 
 */
void EPWM1_Init(void)
{
    EALLOW;
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

    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;//正向线，写高就高
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CAD = AQ_CLEAR;

    EPwm1Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm1Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
//    EPwm1Regs.TZSEL.bit.OSHT1 = 1;

    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;

    EDIS;
}

/**
 * @brief Init ePWM2
 * 
 */
void EPWM2_Init(void)
{
    EALLOW;
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

    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;//正向线，写高就高
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CAD = AQ_CLEAR;


    EPwm2Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm2Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
//    EPwm2Regs.TZSEL.bit.OSHT1 = 1;

    EDIS;
}

/**
 * @brief Init ePWM3
 * 
 */
void EPWM3_Init(void)
{
    EALLOW;
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

    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;//正向线，写高就高
    EPwm3Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CAD = AQ_CLEAR;

    EPwm3Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm3Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
//    EPwm3Regs.TZSEL.bit.OSHT1 = 1;

    EDIS;
}


/**
 * @brief Init ePWM4
 * 
 */
void EPWM4_Init(void)
{
    EALLOW;
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

    EPwm4Regs.AQCTLA.bit.CAU = AQ_CLEAR;//正向线，写高就高
    EPwm4Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm4Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLB.bit.CAD = AQ_CLEAR;

    EPwm4Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm4Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
//    EPwm4Regs.TZSEL.bit.OSHT1 = 1;

    EDIS;
}

/**
 * @brief Init ePWM5
 * 
 */
void EPWM5_Init(void)
{
    EALLOW;
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm5Regs.TBPHS.half.TBPHS = 0;
    EPwm5Regs.TBCTR = 0x0000;
    EPwm5Regs.TBPRD = 3750;
    EPwm5Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm5Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm5Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPA.half.CMPA = 1875;
    EPwm5Regs.CMPB = 1875;

    EPwm5Regs.AQCTLA.bit.CAU = AQ_CLEAR;//正向线，写高就高
    EPwm5Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm5Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm5Regs.AQCTLB.bit.CAD = AQ_CLEAR;

    EPwm5Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm5Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
//    EPwm5Regs.TZSEL.bit.OSHT1 = 1;

    EDIS;
}

/**
 * @brief Init ePWM6
 * 
 */
void EPWM6_Init(void)
{
    EALLOW;
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm6Regs.TBPHS.half.TBPHS = 0;
    EPwm6Regs.TBCTR = 0x0000;
    EPwm6Regs.TBPRD = 3750;
    EPwm6Regs.TBCTL.bit.CTRMODE=TB_COUNT_UPDOWN;
    EPwm6Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;
    EPwm6Regs.TBCTL.bit.CLKDIV=TB_DIV1;

    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPA.half.CMPA = 1875;
    EPwm6Regs.CMPB = 1875;

    EPwm6Regs.AQCTLA.bit.CAU = AQ_CLEAR;//正向线，写高就高
    EPwm6Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm6Regs.AQCTLB.bit.CAU = AQ_SET;
    EPwm6Regs.AQCTLB.bit.CAD = AQ_CLEAR;

    EPwm6Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm6Regs.TZCTL.bit.TZB = TZ_FORCE_LO;
//    EPwm6Regs.TZSEL.bit.OSHT1 = 1;


    EDIS;
}

/**
 * @brief Init ePWM1-6 ,GPIO for ePWM1-6 ,init interrupt of ePWM，Init Gpio for Trip Zone
 * 
 */
void EPWM_Init(void)
{

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    //init GPIO for ePWM1-6
    InitEPwmGpio();
    //init GPIO for Tz1-6
    InitTzGpio();

    //init ePWM
    EPWM1_Init();
    EPWM2_Init();
    EPWM3_Init();
    EPWM4_Init();
    EPWM5_Init();
    EPWM6_Init();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

/**
 * @brief Turn on PWM output by AQ submodule
 *
 */
void EPWM_AQ_Inverter_SoftForce_OFF(void)
{
    EALLOW;
    //Output A AQ Action
    EPwm1Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm2Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm3Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;

    //Output B AQ action
    EPwm1Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
    EPwm2Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
    EPwm3Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
    EDIS;
}

/**
 * @brief Shut down PWM output by AQ submodule
 * 
 */
void EPWM_AQ_Inverter_SoftForce_LOW(void)
{
    EALLOW;
    EPwm1Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;

    //Output B AQ action
    EPwm1Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;
    EPwm2Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;
    EPwm3Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;
    EDIS;
}

/**
 * @brief Turn on PWM output by AQ submodule
 *
 */
void EPWM_AQ_Rectifier_SoftForce_OFF(void)
{
    EALLOW;
    //Output A AQ Action
    EPwm4Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm5Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;
    EPwm6Regs.AQCSFRC.bit.CSFA = AQ_NO_ACTION;

    //Output B AQ action
    EPwm4Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
    EPwm5Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
    EPwm6Regs.AQCSFRC.bit.CSFB = AQ_NO_ACTION;
    EDIS;
}

void EPWM_AQ_Rectifier_SoftForce_LOW(void)
{
    EALLOW;
    EPwm4Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm5Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;
    EPwm6Regs.AQCSFRC.bit.CSFA = AQ_CLEAR;

    //Output B AQ action
    EPwm4Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;
    EPwm5Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;
    EPwm6Regs.AQCSFRC.bit.CSFB = AQ_CLEAR;
    EDIS;
}

/**
 * @brief Set OneShot event,turn off PWM until clear OST flag
 * 
 */
void EPWM_TZ_Software_OST_Set(void)
{
    EALLOW;
    EPwm1Regs.TZFRC.bit.OST = 0x1;
    EPwm2Regs.TZFRC.bit.OST = 0x1;
    EPwm3Regs.TZFRC.bit.OST = 0x1;
    EPwm4Regs.TZFRC.bit.OST = 0x1;
    EPwm5Regs.TZFRC.bit.OST = 0x1;
    EPwm6Regs.TZFRC.bit.OST = 0x1;
    EDIS;
}

/**
 * @brief Set Cycle-by-cycle event,clear in next TBCTR = 0 event
 * 
 */
void EPWM_TZ_Software_CBC_Set(void)
{
    EALLOW;
    EPwm1Regs.TZFRC.bit.CBC = 0x1;
    EPwm2Regs.TZFRC.bit.CBC = 0x1;
    EPwm3Regs.TZFRC.bit.CBC = 0x1;
    EPwm4Regs.TZFRC.bit.CBC = 0x1;
    EPwm5Regs.TZFRC.bit.CBC = 0x1;
    EPwm6Regs.TZFRC.bit.CBC = 0x1;
    EDIS;
}

/**
 * @brief clear Oneshot event flag
 * 
 */
void EPWM_TZ_Software_OST_Clear(void)
{
    EALLOW;
    EPwm1Regs.TZCLR.bit.OST = 0x1;
    EPwm2Regs.TZCLR.bit.OST = 0x1;
    EPwm3Regs.TZCLR.bit.OST = 0x1;
    EPwm4Regs.TZCLR.bit.OST = 0x1;
    EPwm5Regs.TZCLR.bit.OST = 0x1;
    EPwm6Regs.TZCLR.bit.OST = 0x1;
    EDIS;
}

/**
 * @brief Clear Cycle-by-cycle event flag
 * 
 */
void EPWM_TZ_Software_CBC_Clear(void)
{
    EALLOW;
    EPwm1Regs.TZCLR.bit.CBC = 0x1;
    EPwm2Regs.TZCLR.bit.CBC = 0x1;
    EPwm3Regs.TZCLR.bit.CBC = 0x1;
    EPwm4Regs.TZCLR.bit.CBC = 0x1;
    EPwm5Regs.TZCLR.bit.CBC = 0x1;
    EPwm6Regs.TZCLR.bit.CBC = 0x1;
    EDIS;
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
