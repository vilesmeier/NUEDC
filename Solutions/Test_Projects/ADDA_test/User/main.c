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
//    State_Init();
//    //SOGI
//    SOGI_Init(&sogi_Input);
//    SOGI_Set(&sogi_Input,FREQ_GRID,FREQ_ISR,20.05f,-19.95f);
//    PID_Init(&pi_DC);
//    PID_Set(&pi_DC,0.1f,0.05f,0,0.0f,-5.0f,5.0f,25.0f);
//    //PR
//    PR_Init(&pr_Input);
//    //PR_Set(&pr_Input,20.0f,100.0f,2*PI,100*PI,50.0f);
//    PR_Set(&pr_Input,0.5f,20.0f,2*PI,100*PI,250.0f, 20000);
//    //RAMP
//    RAMP_Init(&ramp);
//    RAMP_Set(&ramp, FREQ_GRID, FREQ_ISR);
//    //RMS
//    RMS_Init(&rms_Cur_Input);
//    //GUI Init
//    OLED_Init();
//    OLED_Clear();
//    //Pherial Init
//    ADC_Init();
//    XINT_Init();
//    EPWM_Init();
//    Rectifier_OFF();
//    Inverter_OFF();


    SysCtrlRegs.LOSPCP.bit.LSPCLK = 0;
    ADDA_Init();

    //Enable Interrupt And JTAG Debug
    EINT;
    ERTM;

    // screen control
    while(1)
    {
        data1 = ADC128S102_Convert(CHANNEL_2);
//        DAC124S085_Output_Update(DAC4, UPDATE_IMMEDIATE, data1);

        //loop自测
//        data1++;
//        if(data1 > 4095)
//        {
//            data1 = 0;
//        }
//        DAC124S085_Output_Update(DAC4, UPDATE_IMMEDIATE, data1);
//        receive1 = SPI_SwitchByte(data1);
    }
}
