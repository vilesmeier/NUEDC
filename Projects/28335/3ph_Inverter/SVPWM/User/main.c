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

//    DELAY_US(2000000);  //waiting for master setup



    //Control Loop Init
    //Current State
    State_Init();
    RMS_Init(&rms_A);
    RMS_Init(&rms_B);
    RMS_Init(&rms_C);

    //RAMP
    Ramp_Init(&ramp);
    Ramp_Set(&ramp, 50, 20000);

    //DQ
    DQ_Init(&dq_vol);
    DQ_Init(&dq_cur);

    //PID
//    PID_Init(&pid_vol);
//    PID_Set(&pid_vol, 0.1f, 0.1f, 0, 0, 20, -20, 750, 20000);
//    PID_Init(&pid_cur);
//    PID_Set(&pid_cur, 0.001f, 0.001f, 0, 0, 20, -20, 750, 20000);

    //PR
    //SVPWM
    PR_Init(&pr_vol_A);
    PR_Set(&pr_vol_A,0.002f,140.0f,0.5*2*PI,100*PI,100,20000);
    PR_Init(&pr_vol_B);
    PR_Set(&pr_vol_B,0.002f,140.0f,0.5*2*PI,100*PI,100,20000);
    PR_Init(&pr_vol_C);
    PR_Set(&pr_vol_C,0.002f,140.0f,0.5*2*PI,100*PI,100,20000);

    PR_Init(&pr_VC_A);
    PR_Set(&pr_VC_A,0.020833f,0.0f,2*PI,100*PI,48,20000);
    PR_Init(&pr_VC_B);
    PR_Set(&pr_VC_B,0.020833f,0.0f,2*PI,100*PI,48,20000);
    PR_Init(&pr_VC_C);
    PR_Set(&pr_VC_C,0.020833f,0.0f,2*PI,100*PI,48,20000);

    //CURRENT MODE
    PR_Init(&pr_cur_A);
    PR_Set(&pr_cur_A,4.0f,50.0f,2*PI,100*PI,48,20000);
    PR_Init(&pr_cur_B);
    PR_Set(&pr_cur_B,4.0f,50.0f,2*PI,100*PI,48,20000);
    PR_Init(&pr_cur_C);
    PR_Set(&pr_cur_C,4.0f,50.0f,2*PI,100*PI,48,20000);



    //GUI
    OLED_Init();
    OLED_Clear();

    UART_Init();
    UART_Set(1152000);


    //Pherial Init
    XINT_Init();
    ADC_Init();
    EPWM_Init();
    EPWM_AQ_SoftForce_LOW();

    SVPWM_Init(&svpwm);

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

        UART_VOFA_3Ch_ShowFloat(dq_vol.a, dq_vol.b, dq_vol.c);

    }
}
