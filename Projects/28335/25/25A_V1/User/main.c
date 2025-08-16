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

    //Control Loop Init
    //Current State
    State_Init();

    //RAMP
    Ramp_Init(&ramp);
    Ramp_Set(&ramp, 50, 20000);

    //PR
    //SVPWM
    PR_Init(&pr_vol_A);
    PR_Set(&pr_vol_A,0.1f,1000.0f,0.1*2*PI,100*PI,50,20000);
    PR_Init(&pr_vol_B);
    PR_Set(&pr_vol_B,0.1f,1000.0f,0.1*2*PI,100*PI,50,20000);
    PR_Init(&pr_vol_C);
    PR_Set(&pr_vol_C,0.1f,1000.0f,0.1*2*PI,100*PI,50,20000);

    PR_Init(&pr_VC_A);
    PR_Set(&pr_VC_A,0.02f,0.0f,2*PI,100*PI,50,20000);
    PR_Init(&pr_VC_B);
    PR_Set(&pr_VC_B,0.02f,0.0f,2*PI,100*PI,50,20000);
    PR_Init(&pr_VC_C);
    PR_Set(&pr_VC_C,0.02f,0.0f,2*PI,100*PI,50,20000);

    //GUI
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"freq=");

    UART_Init();
    UART_Set(1152000);


    //Pherial Init
    ADC_Init();
    EPWM_Init();

    SVPWM_Init(&svpwm);

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {
        if(GpioDataRegs.GPADAT.bit.GPIO30 == 0)
        {
            state.freq += 1.0f;
            if(state.freq > 100.0f)
            {
                state.freq = 20.0f;
            }

        }
        else if(GpioDataRegs.GPADAT.bit.GPIO31 == 0)
        {
            state.freq -= 1.0f;
            if(state.freq < 20.0f)
            {
                state.freq = 100.0f;
            }
        }
        else if(GpioDataRegs.GPBDAT.bit.GPIO49 == 0)
        {
            state.freq = 100.0f;
        }
        else if(GpioDataRegs.GPBDAT.bit.GPIO48 == 0)
        {
            state.freq = 20.0f;
        }

        Ramp_Set(&ramp, state.freq, 20000);
        PR_Set(&pr_vol_A,0.1f,1000.0f,0.1*2*PI,state.freq * 2*PI,50,20000);
        PR_Set(&pr_vol_B,0.1f,1000.0f,0.1*2*PI,state.freq * 2*PI,50,20000);
        PR_Set(&pr_vol_C,0.1f,1000.0f,0.1*2*PI,state.freq * 2*PI,50,20000);

        OLED_ShowFloat(31,0,state.freq,4,1);


    }
}
