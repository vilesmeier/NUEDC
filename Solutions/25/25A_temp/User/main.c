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

//    //flash init
//    //select BootLoader!
//    InitFlash();
//    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

    //Control Loop Init
    //Current State
    State_Init();
    RMS_Init(&rms_A);
    RMS_Init(&rms_B);
    RMS_Init(&rms_C);

    //RAMP
    Ramp_Init(&ramp);
    Ramp_Set(&ramp, 50, 20000);

    //PR
    //SVPWM
    PR_Init(&pr_vol_A);
    PR_Set(&pr_vol_A,0.002f,120.0f,0.5*2*PI,100*PI,100,20000);
    PR_Init(&pr_vol_B);
    PR_Set(&pr_vol_B,0.002f,120.0f,0.5*2*PI,100*PI,100,20000);
    PR_Init(&pr_vol_C);
    PR_Set(&pr_vol_C,0.002f,120.0f,0.5*2*PI,100*PI,100,20000);

    PR_Init(&pr_VC_A);
    PR_Set(&pr_VC_A,0.02f,0.0f,2*PI,100*PI,50,20000);
    PR_Init(&pr_VC_B);
    PR_Set(&pr_VC_B,0.02f,0.0f,2*PI,100*PI,50,20000);
    PR_Init(&pr_VC_C);
    PR_Set(&pr_VC_C,0.02f,0.0f,2*PI,100*PI,50,20000);

    //GUI
    OLED_Init();
    OLED_Clear();

    UART_Init();
    UART_Set(1152000);


    //Pherial Init
    XINT_Init();
    ADC_Init();
    EPWM_Init();

    SVPWM_Init(&svpwm);

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

    }
}
