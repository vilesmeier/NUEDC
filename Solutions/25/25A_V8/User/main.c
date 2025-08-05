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

    //RMS
    RMS_Init(&rms_A);
    RMS_Init(&rms_C);
    RMS_Init(&rms_Test);

    //FILTER
    Filter_RC_LPF_Init(&rc_rec_a);
    Filter_RC_LPF_Set(&rc_rec_a, 1000, 20000);
    Filter_RC_LPF_Init(&rc_rec_c);
    Filter_RC_LPF_Set(&rc_rec_c, 1000, 20000);

    //PR
    PR_Init(&pr_vol_A);
    PR_Set(&pr_vol_A,0.1f,65.0f,1.5*2*PI,100*PI,VDC,20000);
    PR_Init(&pr_vol_C);
    PR_Set(&pr_vol_C,0.1f,65.0f,1.5*2*PI,100*PI,VDC,20000);

    PR_Init(&pr_rec_a);
    PR_Set(&pr_rec_a,2.0f,75.0f,5.0f,100*PI,VDC,20000);//3, 38
    PR_Init(&pr_rec_c);
    PR_Set(&pr_rec_c,2.0f,75.0f,5.0f,100*PI,VDC,20000);

    // SVPWM
    SVPWM_Init(&svpwm);
    SVPWM_Init(&svpwm_rec);

    //GUI
    OLED_Init();
    OLED_Clear();
    OLED_ShowString(0,0,"freq=");
    OLED_ShowString(0,1,"Mode=");

    UART_Init();
    UART_Set(3000000);


    //Pherial Init
    XINT_GPIO_Init();
    ADC_Init();
    EPWM_Init();
    EPWM_AQ_Rectifier_SoftForce_LOW();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    while(1)
    {
        // get key state
        state.flag.bits.GPIO30_CUR = GpioDataRegs.GPADAT.bit.GPIO30;
        state.flag.bits.GPIO31_CUR = GpioDataRegs.GPADAT.bit.GPIO31;
        state.flag.bits.GPIO48_CUR = GpioDataRegs.GPBDAT.bit.GPIO48;
        state.flag.bits.GPIO49_CUR = GpioDataRegs.GPBDAT.bit.GPIO49;

        // clear flag
        state.flag.bits.FLAG_FREQ_CHANGE = FREQ_NO_CHANGE;

        // set period
        if((state.flag.bits.GPIO30_CUR == 0) && (state.flag.bits.GPIO30_PRE == 1))
        {
            state.freq = 1.0f + state.freq;
            if(state.freq > 100.0f)
            {
                state.freq = 20.0f;
            }
            state.flag.bits.FLAG_FREQ_CHANGE = FREQ_CHANGE;
        }

        if((state.flag.bits.GPIO31_CUR == 0) && (state.flag.bits.GPIO31_PRE == 1))
        {
            state.freq = state.freq - 1.0f;
            if(state.freq < 20.0f)
            {
                state.freq = 100.0f;
            }
            state.flag.bits.FLAG_FREQ_CHANGE = FREQ_CHANGE;
        }

        if((state.flag.bits.GPIO49_CUR == 0) && (state.flag.bits.GPIO49_PRE == 1))
        {
            state.freq = 100.0f;
            state.flag.bits.FLAG_FREQ_CHANGE = FREQ_CHANGE;
        }

        if((state.flag.bits.GPIO48_CUR == 0) && (state.flag.bits.GPIO48_PRE == 1))
        {
            state.freq = 20.0f;
            state.flag.bits.FLAG_FREQ_CHANGE = FREQ_CHANGE;
        }

        // reconfig control loop
//        if(state.flag.bits.FLAG_FREQ_CHANGE == FREQ_CHANGE)
//        {
//            Ramp_Set(&ramp, state.freq, 20000);
//
//            PR_Set(&pr_vol_A,0.1f,  70.0f, 0.1*2*PI, state.freq * 2 * PI,VDC,20000);
//            PR_Set(&pr_vol_C,0.1f,  70.0f, 0.1*2*PI, state.freq * 2 * PI,VDC,20000);
//            PR_Set(&pr_rec_a,1.0f,  10.0f, 0.1*2*PI, state.freq * 2 * PI,VDC,20000);
//            PR_Set(&pr_rec_c,1.0f,  10.0f, 0.1*2*PI, state.freq * 2 * PI,VDC,20000);
//        }

        // OLED
        OLED_ShowFloat(31,0,state.freq,4,1);
        if(state.flag.bits.FLAG_REC_EN == MODE_ENABLE)
        {
            OLED_ShowString(31,1,"ENA");
        }
        else
        {
            OLED_ShowString(31,1,"DIS");
        }

        // vofa debug
        UART_VOFA_3Ch_ShowFloat(state.V_ab, state.I_In_c, pr_rec_a.out);

        // update state
        state.flag.bits.GPIO30_PRE = state.flag.bits.GPIO30_CUR;
        state.flag.bits.GPIO31_PRE = state.flag.bits.GPIO31_CUR;
        state.flag.bits.GPIO48_PRE = state.flag.bits.GPIO48_CUR;
        state.flag.bits.GPIO49_PRE = state.flag.bits.GPIO49_CUR;
    }
}
