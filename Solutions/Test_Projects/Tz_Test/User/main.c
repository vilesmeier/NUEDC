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

    // screen control
    while(1)
    {
        EPWM_TZ_Software_CBC_Set();
        DELAY_US(1000);
        EPWM_TZ_Software_CBC_Clear();
        DELAY_US(1000);

    }
}
