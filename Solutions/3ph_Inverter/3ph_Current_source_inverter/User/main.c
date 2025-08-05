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
    RMS_Init(&Grid_state);

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
    //VOLTAGE
//    PR_Init(&pr_vol_A);
//    PR_Set(&pr_vol_A,0.001f,90.0f,2*PI,100*PI,45,20000);
//    PR_Init(&pr_vol_B);
//    PR_Set(&pr_vol_B,0.001f,90.0f,2*PI,100*PI,45,20000);
//    PR_Init(&pr_vol_C);
//    PR_Set(&pr_vol_C,0.001f,90.0f,2*PI,100*PI,45,20000);
    //CURRENT
    PR_Init(&pr_cur_A);
    PR_Set(&pr_cur_A,10.0f,900.0f,2*PI,100*PI,40,20000);
    PR_Init(&pr_cur_B);
    PR_Set(&pr_cur_B,10.0f,900.0f,2*PI,100*PI,40,20000);
    PR_Init(&pr_cur_C);
    PR_Set(&pr_cur_C,10.0f,900.0f,2*PI,100*PI,40,20000);


    //GUI
    OLED_Init();
    OLED_Clear();

    UART_Init();
    UART_Set(3000000);


    //Pherial Init
    XINT_Init();
    ADC_Init();
    EPWM_Init();
    EPWM_AQ_SoftForce_OFF();

    SVPWM_Init(&svpwm);
    SVPWM_Set(&svpwm, 3750);

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {


    }
}
