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

    //Control Loop Init
    //Current State
    State_Init();
    RMS_Init(&Grid_state);

    //RAMP
    Ramp_Init(ramp_Addr);
    Ramp_Set(ramp_Addr, 50, 20000);

    //PR
    PR_Init(&pr_Vol);
    PR_Set(&pr_Vol,0.0005f,320.0f,0.5*2*PI,100*PI,40,20000);



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
    EPWM_OFF();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

    }
}
