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

    UART_Init();
    UART_Set(115200);

    EPWM_Init();
    Ramp_Init(&ramp);
    Ramp_Set(&ramp,50,20000);

    EDIS;
    EINT;
    ERTM;

    // screen control
    while(1)
    {
        ScicRegs.SCIFFCT.bit.FFTXDLY = 0x0;
        DELAY_US(1);

        UART_VOFA_1Ch_ShowFloat(sinf(ramp.ref_angle));
    }
}
