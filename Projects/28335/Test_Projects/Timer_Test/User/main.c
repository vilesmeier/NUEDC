#include "global.h"

void main(void)
{
    //Power On Reset
    InitSysCtrl();
    InitGpio();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EINT;
    ERTM;

    UART_Init(115200);

    TIM0_Init();
    TIM0_Set(20000.0f);
    TIM0_Start();




    // screen control
    while(1)
    {


    }
}
