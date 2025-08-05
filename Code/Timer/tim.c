#include "global.h"

/**
 * @brief Init CPU Timer0
 * 
 */
void TIM0_Init(void)
{
    //pherial level interrupt config
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
    EDIS;
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    EINT;
    ERTM;

    //pherial init
    InitCpuTimers();
    ConfigCpuTimer(&CpuTimer0,150, 1/20000.0f*100000.0f);

    //config interrupt
    CpuTimer0Regs.TCR.bit.TIE = 1;

    //stop timer
    CpuTimer0Regs.TCR.bit.TSS = 1;

    //reload timer
    CpuTimer0Regs.TCR.bit.TRB = 1;    // 1 = reload timer
}

/**
 * @brief Set CPUTimer0 frequency
 * 
 * @param freq frequency in Hz
 */
void TIM0_Set(float freq)
{
    ConfigCpuTimer(&CpuTimer0,150, 1/freq*1000000);
}

/**
 * @brief Start CpuTimer0
 * 
 */
void TIM0_Start(void)
{
    //start timer0
    CpuTimer0Regs.TCR.bit.TSS = 0;
}

/**
 * @brief Stop CpuTimer0
 * 
 */
void TIM0_Stop(void)
{
    //start timer0
    CpuTimer0Regs.TCR.bit.TSS = 1;
}

/**
 * @brief Reload CpuTimer0
 * 
 */
void TIM0_Reload(void)
{
    //reload timer
    CpuTimer0Regs.TCR.bit.TRB = 1;
}

/**
 * @brief Interrupt handler
 * 
 * @return interrupt 
 */
interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
