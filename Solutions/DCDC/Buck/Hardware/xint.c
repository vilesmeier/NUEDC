#include "global.h"

void XINT_Init(void)
{

    EALLOW;
    DINT;

    //PIE Config
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;  //XINT1
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;  //XINT2
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; //XINT3
    PieCtrlRegs.PIEIER12.bit.INTx2 = 1; //XINT4
    PieCtrlRegs.PIEIER12.bit.INTx3 = 1; //XINT5
    PieCtrlRegs.PIEIER12.bit.INTx4 = 1; //XINT6
    PieCtrlRegs.PIEIER12.bit.INTx5 = 1; //XINT7

    IER |= M_INT1;
    IER |= M_INT12;

    PieVectTable.XINT1 = &XINT1_isr;
    PieVectTable.XINT2 = &XINT2_isr;
    PieVectTable.XINT3 = &XINT3_isr;
    PieVectTable.XINT4 = &XINT4_isr;
    PieVectTable.XINT5 = &XINT5_isr;
    PieVectTable.XINT6 = &XINT6_isr;
    PieVectTable.XINT7 = &XINT7_isr;

    //GPIO Config
    //Sample Windows Lenth Set
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0xF0;
    GpioCtrlRegs.GPBCTRL.bit.QUALPRD2 = 0xF0;

    //XINT1
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO30 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 30;

    //XINT2
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO31 = 3;
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 31;

    //XINT3
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO49 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 49;

    //XINT4
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT4SEL.bit.GPIOSEL = 48;

    //XINT5
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO51 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT5SEL.bit.GPIOSEL = 51;

    //XINT6
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO50 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT6SEL.bit.GPIOSEL = 50;

    //XINT7
    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO53 = 3;
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT7SEL.bit.GPIOSEL = 53;


    //XINT Config
    //XINT1
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //negedge

    //XINT2
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // Enable XINT2
    XIntruptRegs.XINT2CR.bit.POLARITY = 0;      //negedge

    //XINT3
    XIntruptRegs.XINT3CR.bit.ENABLE = 1;        // Enable XINT3
    XIntruptRegs.XINT3CR.bit.POLARITY = 0;      //negedge

    //XINT4
    XIntruptRegs.XINT4CR.bit.ENABLE = 1;        // Enable XINT4
    XIntruptRegs.XINT4CR.bit.POLARITY = 0;      //negedge

    //XINT5
    XIntruptRegs.XINT5CR.bit.ENABLE = 1;        // Enable XINT5
    XIntruptRegs.XINT5CR.bit.POLARITY = 0;      //negedge

    //XINT6
    XIntruptRegs.XINT6CR.bit.ENABLE = 1;        // Enable XINT6
    XIntruptRegs.XINT6CR.bit.POLARITY = 0;      //negedge

    //XINT7
    XIntruptRegs.XINT7CR.bit.ENABLE = 1;        // Enable XINT7
    XIntruptRegs.XINT7CR.bit.POLARITY = 0;      //negedge

    EDIS;
    EINT;
    ERTM;
}


interrupt void XINT1_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


interrupt void XINT2_isr(void)
{
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void XINT3_isr(void)
{
    state.Xint_select = !state.Xint_select;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void XINT4_isr(void)
{
    if(state.Xint_select == SET_KP)
    {
        state.kp = state.kp + 0.01f;
        PID_Set(&pid_vol, state.kp, state.ki, 0, 0, 0, 50, 500);
    }
    else
    {
        state.ki = state.ki + 0.01f;
        PID_Set(&pid_vol, state.kp, state.ki, 0, 0, 0, 50, 500);
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void XINT5_isr(void)
{
    if(state.Xint_select == SET_KP)
    {
        state.kp = state.kp - 0.01f;
        if(state.kp <= 0.0f)
        {
            state.kp = 0.0f;
        }
        PID_Set(&pid_vol, state.kp, state.ki, 0, 0, 0, 50, 80);
    }
    else
    {
        state.ki = state.ki - 0.01f;
        if(state.ki <= 0.0f)
        {
            state.ki = 0.0f;
        }
        PID_Set(&pid_vol, state.kp, state.ki, 0, 0, 0, 50, 80);
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void XINT6_isr(void)
{


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void XINT7_isr(void)
{


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}
