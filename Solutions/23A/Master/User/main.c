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
    Ramp_Init(ramp_Addr);
    Ramp_Set(ramp_Addr, 50, 20000);

    //DQ
    DQ_Init(dq_Vol_Addr);
    DQ_Init(dq_Cur_Addr);

    //PID
    PID_Init(pid_Vol_Addr);
    PID_Set(pid_Vol_Addr, 1, 0.0008, 0, 40);
    PID_Init(pid_Cur_Addr);
    PID_Set(pid_Cur_Addr, 0.5, 0.0008, 0, 40);

    //PR
    PR_Init(pr_Vol_Addr);
    PR_Set(pr_Vol_Addr,0.0001,300,0.01,100*PI,48);
    PR_Init(pr_Cur_Addr);
    PR_Set(pr_Cur_Addr,0.0001,300,0.03*2*PI,100*PI,48);

    //SOGI
    SPLL_1PH_SOGI_reset(sogi_Vol_Addr);
    SPLL_1PH_SOGI_config(sogi_Vol_Addr,50,20000,20.05,-19.95);

    //GUI
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"Vol=");
    OLED_ShowString(0,1,"Cur_total=");
    OLED_ShowString(0,3,"K_I=");
    OLED_ShowString(0,4,"Cur_this=");
    OLED_ShowString(0,6,"MODE:");
    OLED_ShowString(0,7,"Output:");

    //Pherial Init

    ADC_Init();
    EPWM_Init();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

    }
}
