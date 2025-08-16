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

    Uint8 i = 0;

    for(i = 0;i<6;i++)
    {
        NOTCH_Init(&notch[i]);
    }
    //ÏÝ²¨Æ÷²âÊÔ
    for(i = 0; i<3;i++)
    {
        NOTCH_Set(&notch[i],50,20000,MODE_BANDSTOP);
    }

    for(i = 3; i<6; i++)
    {
        NOTCH_Set(&notch[i],50,20000,MODE_BANDPASS);
    }

    ADC_Init();
    EPWM_Init();
    Ramp_Init(&ramp);
    Ramp_Set(&ramp,50,20000);

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

    }

}
