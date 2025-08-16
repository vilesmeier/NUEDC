#include "global.h"

/**
 * µ¥¼«ÐÔ±¶Æµ PWM Rectifier£¬ switch freq @ 20KHz
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
    PID_Set(&pi_DC,0.05f,0.1f,0,0.0f,4.0f,-4.0f,10000.0f,20000.0f);

    //PR
    PR_Init(&pr_Input);
    PR_Set(&pr_Input,8.5f,70.0f,2*PI,100*PI,45.0f, 20000.0f);

    //RMS
    RMS_Init(&rms_Cur_Input);
    RMS_Init(&rms_Vol_Input);

    //NOTCH
    NOTCH_Init(&notch);
    NOTCH_Set(&notch, 100, 20000, MODE_BANDSTOP);

    //GUI Init
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,3,"rKP");
    OLED_ShowString(0,4,"rKR");



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
        UART_VOFA_3Ch_ShowFloat(state.V_DC, pr_Input.nolimit, pr_Input.out);

    }
}
