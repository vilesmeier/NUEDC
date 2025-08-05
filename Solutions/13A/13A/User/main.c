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

    //discomment below when boot from on-chip flash
    //select BootLoader!
//    InitFlash();
//    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);

    //Control Loop Init
    //current state
    State_Init();

    //UART Debug probe
    UART_Init();
    UART_Set(115200);

    //SOGI
    SOGI_Init(&sogi_Input);
    SOGI_Set(&sogi_Input,FREQ_GRID,FREQ_ISR,20.05f,-19.95f);


    PID_Init(&pi_DC);
    PID_Set(&pi_DC,0.05f,0.001f,0,0.0f,-4.0f,4.0f,10000.0f);

    //PR
    PR_Init(&pr_Input);
    PR_Set(&pr_Input,11.5f,200.0f,2*PI,100*PI,45.0f, 20000.0f);


    //RAMP
    RAMP_Init(&ramp);
    RAMP_Set(&ramp, FREQ_GRID, FREQ_ISR);

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
    XINT_Init();
    EPWM_Init();
    Rectifier_OFF();
    Inverter_OFF();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    // screen control
    while(1)
    {
         OLED_ShowSignedFloat(19,3,d,2,1);
         OLED_ShowSignedFloat(19,4,e,3,1);

         if(xint_select == PR_KP)
         {
             OLED_ShowString(115,3,"<-");
             OLED_ShowString(115,4,"..");
         }
         else
         {
             OLED_ShowString(115,3,"..");
             OLED_ShowString(115,4,"<-");
         }

    }
}
