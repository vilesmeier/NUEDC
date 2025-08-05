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
    //XINT1
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = 2;
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 12;

    //XINT2
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 2;
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 13;

    //XINT3
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO54 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 2;
    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 54;

    //XINT4
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 2;
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT4SEL.bit.GPIOSEL = 55;

    //XINT5
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 2;
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT5SEL.bit.GPIOSEL = 56;

    //XINT6
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 2;
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT6SEL.bit.GPIOSEL = 57;

    //XINT7
    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO62 = 0;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 2;
    GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;      //Enable Pullup Resistor
    GpioIntRegs.GPIOXINT7SEL.bit.GPIOSEL = 62;


    //XINT Config
    //XINT1
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;      //negedge

    //XINT2
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;        // Enable XINT2
    XIntruptRegs.XINT2CR.bit.POLARITY = 0;      //negedge

    //XINT3
    XIntruptRegs.XINT3CR.bit.ENABLE = 1;        // Enable XINT2
    XIntruptRegs.XINT3CR.bit.POLARITY = 0;      //negedge

    //XINT4
    XIntruptRegs.XINT4CR.bit.ENABLE = 1;        // Enable XINT2
    XIntruptRegs.XINT4CR.bit.POLARITY = 0;      //negedge

    //XINT5
    XIntruptRegs.XINT5CR.bit.ENABLE = 1;        // Enable XINT2
    XIntruptRegs.XINT5CR.bit.POLARITY = 0;      //negedge

    //XINT6
    XIntruptRegs.XINT6CR.bit.ENABLE = 1;        // Enable XINT2
    XIntruptRegs.XINT6CR.bit.POLARITY = 0;      //negedge

    //XINT7
    XIntruptRegs.XINT7CR.bit.ENABLE = 1;        // Enable XINT2
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


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void XINT4_isr(void)
{


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

interrupt void XINT5_isr(void)
{


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
