#include "global.h"



void System_Clock_Close(void)
{
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;   // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;   // SCI-B
    SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 0;   // SCI-C
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;   // SPI-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 0; // McBSP-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 0; // McBSP-B
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;  // eCAN-A
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;  // eCAN-B
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 0;  // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;  // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;  // ePWM6
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;  // eCAP1
    SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;  // eCAP2
    SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;  // eCAP3
    SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;  // eCAP4
    SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 0;  // eCAP5
    SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 0;  // eCAP6
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;  // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;  // eQEP2
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 0; // CPU Timer 0
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 0; // CPU Timer 1
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 0; // CPU Timer 2
    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 0;   //DMA
}

void State_Init(void)
{
    state.Flag_Output_Mode   = MODE_UNKNOWN;
    state.Flag_Output_Enable = OUTPUT_DISABLE;
    state.Flag_Soft_Start    = SOFT_START;

    //clear Voltage value
    state.V_A = 0;
    state.V_B = 0;
    state.V_C = 0;
    state.V_ab = 0;
    state.V_bc = 0;
    state.V_ca = 0;


    //clear Current value
    state.I_A = 0;
    state.I_B = 0;
    state.I_C = 0;

    //clear target value
    state.V_set_output = 15.0f;
    state.I_set_output = 1.0f;

    //PI parameter set
    state.Xint_select = 0;
    state.kp = 0.001f;
    state.ki = 0.001f;

    //SoftStart
    state.V_softstart = 10;
    state.I_softstart = 0;
    state.pr_V_set = 0;
    state.pr_I_set = 0;
}
