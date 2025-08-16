#include "global.h"


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

void TIM0_Set(float freq)
{
    ConfigCpuTimer(&CpuTimer0,150, 1/freq*1000000);
}

void TIM0_Start(void)
{
    //start timer0
    CpuTimer0Regs.TCR.bit.TSS = 0;
}

void TIM0_Stop(void)
{
    //start timer0
    CpuTimer0Regs.TCR.bit.TSS = 1;
}

void TIM0_Reload(void)
{
    //reload timer
    CpuTimer0Regs.TCR.bit.TRB = 1;    // 1 = reload timer
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    UART_SendChar(0xFF);

    if(CpuTimer0.InterruptCount > 10)
    {
        TIM0_Stop();
    }


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
