#include "global.h"

/**
 * @brief close unused clock
 * 
 */
void System_Clock_Close(void)
{
    // Set LSPCLK to 150MHz
    SysCtrlRegs.LOSPCP.bit.LSPCLK = 0;
    
    //Disable Clock For DSP
    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 0;      // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0;      // SCI-B
    SysCtrlRegs.PCLKCR0.bit.MCBSPAENCLK = 0;    // McBSP-A
    SysCtrlRegs.PCLKCR0.bit.MCBSPBENCLK = 0;    // McBSP-B
    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0;     // eCAN-A
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0;     // eCAN-B
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

/**
 * @brief init current machine state
 * 
 */
void State_Init(void)
{

}
