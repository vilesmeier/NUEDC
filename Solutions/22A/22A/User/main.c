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

    //SOGI
    SOGI_Init(&sogi_Input);
    SOGI_Set(&sogi_Input,FREQ_GRID,FREQ_ISR,20.05f,-19.95f);
    SOGI_Init(&sogi_Output);
    SOGI_Set(&sogi_Output,FREQ_GRID,FREQ_ISR,20.05f,-19.95f);

    //DQ
    DQ_Init(&dq_Input);
    DQ_Init(&dq_output);

    //PID
    PID_Init(&pi_Input_Cur_D);
    PID_Set(&pi_Input_Cur_D,1.0f,10.0f,0,40.0f);
    PID_Init(&pi_Input_Cur_Q);
    PID_Set(&pi_Input_Cur_Q,1.0f,10.0f,0,40.0f);
    PID_Init(&pi_DC);
    PID_Set(&pi_DC,10.0f,20.0f,0,40.0f);

    //PR
    PR_Init(&pr_Input);
    PR_Set(&pr_Input,0.001f,200.0f,5*PI,100*PI,48.0f);
    PR_Init(&pr_Output_Cur);
    PR_Set(&pr_Output_Cur,0.001f,200.0f,5*PI,100*PI,48.0f);

    //RAMP
    RAMP_Init(&ramp);
    RAMP_Set(&ramp, FREQ_GRID, FREQ_ISR);

    //GUI Init
    OLED_Init();
    OLED_Clear();

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

    }
}
