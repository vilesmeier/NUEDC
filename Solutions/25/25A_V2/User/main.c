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
    XINT_GPIO_Init();
    ADC_Init();
    EPWM_Init();

    SVPWM_Init(&svpwm);

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {

        state.flag.bits.GPIO30_CUR = GpioDataRegs.GPADAT.bit.GPIO30;
        state.flag.bits.GPIO31_CUR = GpioDataRegs.GPADAT.bit.GPIO31;
        state.flag.bits.GPIO48_CUR = GpioDataRegs.GPBDAT.bit.GPIO48;
        state.flag.bits.GPIO49_CUR = GpioDataRegs.GPBDAT.bit.GPIO49;

        if((state.flag.bits.GPIO30_CUR == 0) && (state.flag.bits.GPIO30_PRE == 1))
        {
            state.freq = 1.0f + state.freq;
            if(state.freq > 100.0f)
            {
                state.freq = 20.0f;
            }

        }
        else if((state.flag.bits.GPIO31_CUR == 0) && (state.flag.bits.GPIO31_PRE == 1))
        {
            state.freq = state.freq - 1.0f;
            if(state.freq < 20.0f)
            {
                state.freq = 100.0f;
            }
        }
        else if((state.flag.bits.GPIO49_CUR == 0) && (state.flag.bits.GPIO49_PRE == 1))
        {
            state.freq = 100.0f;
        }
        else if((state.flag.bits.GPIO48_CUR == 0) && (state.flag.bits.GPIO48_PRE == 1))
        {
            state.freq = 20.0f;
        }

        Ramp_Set(&ramp, state.freq, 20000);
        PR_Set(&pr_vol_A,0.1f,1000.0f,0.1*2*PI,state.freq * 2 * PI,50,20000);
        PR_Set(&pr_vol_B,0.1f,1000.0f,0.1*2*PI,state.freq * 2 * PI,50,20000);
        PR_Set(&pr_vol_C,0.1f,1000.0f,0.1*2*PI,state.freq * 2 * PI,50,20000);

        OLED_ShowFloat(31,0,state.freq,4,1);

        state.flag.bits.GPIO30_PRE = state.flag.bits.GPIO30_CUR;
        state.flag.bits.GPIO31_PRE = state.flag.bits.GPIO31_CUR;
        state.flag.bits.GPIO48_PRE = state.flag.bits.GPIO48_CUR;
        state.flag.bits.GPIO49_PRE = state.flag.bits.GPIO49_CUR;

    }
}
