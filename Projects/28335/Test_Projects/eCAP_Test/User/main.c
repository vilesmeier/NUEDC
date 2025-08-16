#include "pwm.h"
#include "cap.h"
#include "DSP28x_Project.h"

CAP_DUTY ecap;

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
    ECAP_Init(&ecap);

    while(1)
    {

    }

}
