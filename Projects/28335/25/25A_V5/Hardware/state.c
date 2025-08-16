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
    //clear Voltage value
    state.V_A = 0;
    state.V_C = 0;
    state.V_ab = 0;
    state.V_bc = 0;

    state.I_out_A = 0;
    state.I_out_C = 0;

    state.I_In_a = 0;
    state.I_In_c = 0;

    state.ref_a = 0;
    state.ref_c = 0;

    //clear target value
    state.V_set_output = 32.0f/SQUARE_3;
    state.I_set_output = 1.0f;
    state.I_softstart = 0;
    state.V_softstart = 0;
    state.pr_I_set = 0;
    state.pr_V_set = 0;
    state.V_cap = 0;

    //XINT parameter set
    state.freq = 50.0f;

    state.flag.all = 0x0000;

}
