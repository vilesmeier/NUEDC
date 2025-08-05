#include "global.h"


void System_Clock_Close(void)
{
    // Set LSPCLK to 150MHz
    SysCtrlRegs.LOSPCP.bit.LSPCLK = 0;

    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;      // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;      // SCI-B
    SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 0;      // SCI-C
    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0;      // SPI-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 0;    // McBSP-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 0;    // McBSP-B
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;     // eCAN-A
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;     // eCAN-B
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 0;     // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 0;     // ePWM6
    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 0;     // eCAP1
    SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 0;     // eCAP2
    SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 0;     // eCAP3
    SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 0;     // eCAP4
    SysCtrlRegs.PCLKCR1.bit.ECAP5ENCLK = 0;     // eCAP5
    SysCtrlRegs.PCLKCR1.bit.ECAP6ENCLK = 0;     // eCAP6
    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 0;     // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 0;     // eQEP2
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 0; // CPU Timer 0
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER1ENCLK = 0; // CPU Timer 1
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER2ENCLK = 0; // CPU Timer 2
    SysCtrlRegs.PCLKCR3.bit.DMAENCLK = 0;       //DMA
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 0;     //XINTF
}

void State_Init(void)
{
    //State Flags
    state.Flag_Input_Enable     = INPUT_DISABLE;
    state.Flag_Output_Enable    = OUTPUT_DISABLE;
    state.Flag_Input_SoftStart  = SOFT_START;
    state.Flag_Output_SoftStart = SOFT_START;

    //Actual Value
    state.V_Input  = 0;
    state.I_Input  = 0;
    state.V_DC     = 0;
    state.I_DC     = 0;
    state.V_Output = 0;
    state.I_output = 0;

    //Set Value
    state.Load_Type = R;
    state.PF = 1;
    state.Theta = 0;
    state.V_DC_Set = 45;
    state.V_Output_Set_RMS = 24;
    state.I_Input_Set_RMS = 1;
    state.I_Input_Softstart = 0;
    state.V_Output_Softstart = 0;
    state.V_GiveValue = 0;

    //key select
    state.Key_Set = 0;
}
