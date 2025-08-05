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



    State_Init();
    RMS_Init(&Grid_state);

    //RAMP
    Ramp_Init(&ramp);
    Ramp_Set(&ramp, 50, 20000);

    //PR
    PR_Init(&pr_vol_A);
    PR_Set(&pr_vol_A,0.001f,90.0f,2*PI,100*PI,45,20000);
    PR_Init(&pr_vol_B);
    PR_Set(&pr_vol_B,0.001f,90.0f,2*PI,100*PI,45,20000);
    PR_Init(&pr_vol_C);
    PR_Set(&pr_vol_C,0.001f,90.0f,2*PI,100*PI,45,20000);


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

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {


    }
}
