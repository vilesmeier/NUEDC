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
    System_Clock_Close();
    EINT;
    ERTM;



    //Control Loop Init
    //Current State
    State_Init();
    RMS_Init(&Grid_state);

    //RAMP
    Ramp_Init(&ramp);
    Ramp_Set(&ramp, 50, 20000);

    //RMS
    RMS_Init(&Grid_state);

    //PR
    PR_Init(&pr_Vol);
    PR_Set(&pr_Vol,0.00001f,70.0f,0.001f*2*PI,100*PI,30,20000);
    PR_Init(&pr_Cur);
    PR_Set(&pr_Cur,8.5f,40.0f,1.0f*2*PI,100*PI,50,20000);

    //FILTER
    Filter_RC_LPF_Init(&rc);
    Filter_RC_LPF_Set(&rc, 1000, 20000);

    //GUI
    OLED_Init();
    OLED_Clear();

    UART_Init();
    UART_Set(3000000);


    //Pherial Init
    XINT_Init();
    ADC_Init();
    EPWM_Init();
    EPWM_OFF();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {
        UART_VOFA_1Ch_ShowFloat(pr_Cur.out);

    }
}
