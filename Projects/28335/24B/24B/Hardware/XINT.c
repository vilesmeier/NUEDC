#include "Global.h"

//�ж�����
void XINT_Init(void)
{
    //�ж�ȫ������
    DINT;
    EALLOW;

    IER |= M_INT1;

    //GPIO����
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO30 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 2;
    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 30;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0x3; //�����˲���������Ϊ2*1 Tsysclk(ϵͳʱ��)
    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;

    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 0;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO31 = 2;
    GpioIntRegs.GPIOXINT2SEL.bit.GPIOSEL = 31;
    GpioCtrlRegs.GPACTRL.bit.QUALPRD3 = 0x3; //�����˲���������Ϊ2*1 Tsysclk(ϵͳʱ��)
    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;

    PieVectTable.XINT1 = & xint1_isr;
    PieVectTable.XINT2 = & xint2_isr;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
    PieCtrlRegs.PIEIER1.bit.INTx4 = 1;
    PieCtrlRegs.PIEIER1.bit.INTx5 = 1;


    //�жϴ�������
    XIntruptRegs.XINT1CR.bit.POLARITY = 0;
    XIntruptRegs.XINT2CR.bit.POLARITY = 0;

    //�ж�ʹ��
    XIntruptRegs.XINT1CR.bit.ENABLE = 1;
    XIntruptRegs.XINT2CR.bit.ENABLE = 1;


    EDIS;
    EINT;
    ERTM;
}

//����+1
interrupt void xint1_isr(void)
{

    N_zashu++;
    if(N_zashu > 10)
    {
        N_zashu = 10;
    }
    while(GpioDataRegs.GPADAT.bit.GPIO30 == 0);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

//����-1
interrupt void xint2_isr(void)
{
    N_zashu--;
    if(N_zashu <= 1 || N_zashu > 10)
    {
        N_zashu = 1;
    }
    while(GpioDataRegs.GPADAT.bit.GPIO31 == 0);
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

