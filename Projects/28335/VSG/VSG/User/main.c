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


    //PR
    PR_Init(&pr_Cur);
    PR_Set(&pr_Cur, 3.0f, 50.0f, 2*PI, 100*PI,48.0f,20000.0f);

    //POWER_1PH
    POWER_1ph_Init(&power_v);
    POWER_1ph_Set(&power_v, 20000.0f, 0, 51.0f, 49.0f);

    //VSG
    VSG_Init(&vsg);
    VSG_Set(&vsg, 20000.0f, 100*PI, 20.0f, 20.0f, 0.0f, 0.2f, 1/10000.0f, 2*0.00067f, 3.0f, 15.0f, -10.0f);

    //GUI
    OLED_Init();
    OLED_Clear();

    //UART
    UART_Init();
    UART_Set(3000000);

    //Pherial Init
    ADC_Init();
    EPWM_Init();
    EPWM_OFF();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {
        UART_VOFA_3Ch_ShowFloat(vsg.I_ref, state.I_output, power_v.pRms);
    }
}
