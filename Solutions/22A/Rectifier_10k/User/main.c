#include "global.h"

/**
 * 单极性倍频 PWM Rectifier， switch freq @ 10KHz
 */

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

    //discomment below when boot from on-chip flash
    //select BootLoader!
//    InitFlash();
//    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

    //Control Loop Init
    //current state
    State_Init();

    //UART Debug probe
    UART_Init();
    UART_Set(3000000);

    //PID
    PID_Init(&pi_DC);
    PID_Set(&pi_DC,0.05f,0.1f,0,0.0f,3.0f,-3.0f,10000.0f,10000.0f);

    //PR
    PR_Init(&pr_Input);
//    PR_Set(&pr_Input,4.9f,55.0f,2*PI,100*PI,45.0f, 10000.0f);//功率稍高，70.00，thd稳在5.0左右
    PR_Set(&pr_Input,4.85f,55.0f,2*PI,100*PI,45.0f, 10000.0f);//功率稍低，69.7/8，thd方差较大，4.8-5.8


    //RAMP
    RAMP_Init(&ramp);
    RAMP_Set(&ramp, FREQ_GRID, FREQ_ISR);

    //RMS
    RMS_Init(&rms_Vol_Input);

    //NOTCH
    NOTCH_Init(&notch);
    NOTCH_Set(&notch, 100, 10000, MODE_BANDSTOP);

    //GUI Init
//    OLED_Init();
//    OLED_Clear();



    //Pherial Init
    ADC_Init();
    EPWM_Init();
    Rectifier_OFF();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    // screen control
    while(1)
    {
        UART_VOFA_3Ch_ShowFloat(pi_DC.voltage, pi_DC.integral, pi_DC.voltage * rms_Vol_Input.angle);

    }
}
