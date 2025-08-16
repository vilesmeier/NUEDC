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

    EPWM_Init();
    Ramp_Init(&ramp);
    Ramp_Set(&ramp,50,20000);

    UART_Init();
    UART_Set(115200);

    POWER_3ph_Init(&v);
    POWER_3ph_Set(&v, 20000, 50);

    // screen control
    while(1)
    {

    }
}
