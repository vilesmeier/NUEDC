#include "cap.h"
#include "DSP28x_Project.h"

/**
 * @brief Use eCAP to get Duty Cycle of input signal,duty cycle ¡À0.01
 * 
 */

extern Uint32 k;

/**
 * @brief init Ecap1 pherial
 * 
 */
void ECAP1_Pherial_Init(void)
{
    //gpio init
    InitECap1Gpio();

    //pherial init
    InitECap();

    EALLOW;
    // register clear
    ECap1Regs.ECEINT.all = 0x00;          // Disable all capture interrupts
    ECap1Regs.ECCLR.all = 0xFF;           // Clear all CAP interrupt flags
    ECap1Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped

    //config interrupt
    ECap1Regs.ECEINT.bit.CEVT1 = 0x1;       //enable interrupt at posedge

    //time base
    ECap1Regs.TSCTR  = 0x00000000;
    ECap1Regs.CTRPHS = 0x00000000;
    ECap1Regs.CAP1   = 0x00000000;
    ECap1Regs.CAP2   = 0x00000000;
    ECap1Regs.CAP3   = 0x00000000;
    ECap1Regs.CAP4   = 0x00000000;

    //prescale
    ECap1Regs.ECCTL1.bit.PRESCALE = CAP_DISABLE;
    //event set
    ECap1Regs.ECCTL1.bit.CAP1POL = EVENT_POSEDGE;
    ECap1Regs.ECCTL1.bit.CAP2POL = EVENT_NEGEDGE;
    ECap1Regs.ECCTL1.bit.CAP3POL = EVENT_POSEDGE;
    ECap1Regs.ECCTL1.bit.CAP4POL = EVENT_NEGEDGE;
    //time base reset
    ECap1Regs.ECCTL1.bit.CTRRST1 = EVENT_RESET;
    ECap1Regs.ECCTL1.bit.CTRRST2 = EVENT_NOACTION;
    ECap1Regs.ECCTL1.bit.CTRRST3 = EVENT_RESET;
    ECap1Regs.ECCTL1.bit.CTRRST4 = EVENT_NOACTION;
    //enable load CAP1-4
    ECap1Regs.ECCTL1.bit.CAPLDEN = CAP_ENABLE;

    //mode set
    ECap1Regs.ECCTL2.bit.CONT_ONESHT = MODE_COUNT;
    ECap1Regs.ECCTL2.bit.TSCTRSTOP =   CAP_DISABLE;
    ECap1Regs.ECCTL2.bit.CAP_APWM =    MODE_CAP;

    //enable Ecap
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = CAP_ENABLE;
    EDIS;
}

/**
 * @brief inti struct
 * 
 * @param cap struct pointer
 */
void ECAP_Struct_Init(CAP_DUTY *cap)
{
    cap->cap1  = 0;
    cap->cap2  = 0;
    cap->cap3  = 0;
    cap->cap4  = 0;
    cap->duty1 = 0;
    cap->duty2 = 0;
}

/**
 * @brief module init
 * 
 * @param cap struct pointer
 */
void ECAP_Init(CAP_DUTY *cap)
{
    //global interrupt config
    EALLOW;
    DINT;
    PieVectTable.ECAP1_INT = &ecap1_isr;
    IER |= M_INT4;
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
    EDIS;
    EINT;
    ERTM;

    ECAP1_Pherial_Init();
    ECAP_Struct_Init(cap);
}

/**
 * @brief Ecap1 enable
 * 
 */
void ECAP_Enable(void)
{
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = CAP_ENABLE;
}

/**
 * @brief Ecap disable
 * 
 */
void ECAP_Disable(void)
{
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = CAP_DISABLE;
}

/**
 * @brief get duty cycle of input signal
 * 
 * @param cap 
 */
void ECAP_Get_Duty(CAP_DUTY *cap)
{
    cap->cap1 = ECap1Regs.CAP1;
    cap->cap2 = ECap1Regs.CAP2;
    cap->cap3 = ECap1Regs.CAP3;
    cap->cap4 = ECap1Regs.CAP4;
    if((cap->cap3) != 0)
    {
        cap->duty1 = ((float)(cap->cap2)) / (cap->cap3);
    }
    if((cap->cap1) != 0)
    {
        cap->duty2 = ((float)(cap->cap4)) / (cap->cap1);
    }
}

interrupt void ecap1_isr(void)
{
    k++;
    if( k > 300000)
    {
        k = 0;
    }

    ECap1Regs.ECCLR.bit.CEVT1 = 1;
    ECap1Regs.ECCLR.bit.CEVT2 = 1;
    ECap1Regs.ECCLR.bit.CEVT3 = 1;
    ECap1Regs.ECCLR.bit.CEVT4 = 1;
    ECap1Regs.ECCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}
