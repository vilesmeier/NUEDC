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

    Ramp_Init(&ramp);
    Ramp_Set(&ramp, 50, 20000);

    DQ_Init(&dq);

    SVPWM_Init(&svpwm);
    SVPWM_Set(&svpwm, 3750);

    UART_Init();
    UART_Set(500000);

    EPWM_Init();

    dq.d = 1.0f;
    dq.q = 0;

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

    }

}
