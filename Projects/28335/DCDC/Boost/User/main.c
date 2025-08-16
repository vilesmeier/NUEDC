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

    //system init
    State_Init();

    PID_Init(&pid_vol);
    PID_Set(&pid_vol, 1.0f, 10.0f, 0, 0, 50, 0, 750, 40000);
//    PID_Set(&pid_vol, 0.01f, 0.5f, 0, 0, 0, 50, 100);
    PID_Init(&pid_cur);
    PID_Set(&pid_cur, 0.01f, 0.01f, 0, 0, 50, 2, 750, 40000);

    XINT_Init();
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"kp=");
    OLED_ShowString(0,1,"ki=");


    UART_Init();
    UART_Set(115200);


    ADC_Init();
    EPWM_Init();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {
        UART_VOFA_1Ch_ShowFloat(EPwm2Regs.CMPA.half.CMPA);

        OLED_ShowSignedFloat(19,0,state.kp,4,2);
        OLED_ShowSignedFloat(19,1,state.ki,4,2);
        OLED_ShowSignedFloat(19,2,EPwm2Regs.CMPA.half.CMPA,8,2);

        if(state.Xint_select == SET_KP)
        {
            OLED_ShowString(115,0,"<-");
            OLED_ShowString(115,1,"..");
        }
        else
        {
            OLED_ShowString(115,0,"..");
            OLED_ShowString(115,1,"<-");
        }

    }
}
