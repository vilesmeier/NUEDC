#include "global.h"

void tz(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;//TZ1
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;//TZ2
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;//TZ3
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;//TZ4
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;//TZ5
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;//TZ6

    //Ӳ������
    EPwm1Regs.TZSEL.bit.OSHT1 = 1;//ʹ��TZ1���δ��������Ӳ��
    EPwm1Regs.TZSEL.bit.CBC1 = 1;//ʹ��TZ1���ڴ��������Ӳ��
    EPwm1Regs.TZCTL.bit.TZA = TZ_HIZ;//����ʱ��1A��Ϊ����̬
    EPwm1Regs.TZCTL.bit.TZB = TZ_HIZ;//����ʱ��1B��Ϊ����̬

    EPwm2Regs.TZSEL.bit.OSHT2 = 1;
    EPwm2Regs.TZSEL.bit.CBC2 = 1;
    EPwm2Regs.TZCTL.bit.TZA = TZ_HIZ;
    EPwm2Regs.TZCTL.bit.TZB = TZ_HIZ;

    EPwm3Regs.TZSEL.bit.OSHT3 = 1;
    EPwm3Regs.TZSEL.bit.CBC3 = 1;
    EPwm3Regs.TZCTL.bit.TZA = TZ_HIZ;
    EPwm3Regs.TZCTL.bit.TZB = TZ_HIZ;

    //�������
    EPwm1Regs.TZFRC.bit.OST = 1;//�ֶ���������TZ
    EPwm1Regs.TZFRC.bit.CBC = 1;//�ֶ���������TZ
    EPwm2Regs.TZFRC.bit.OST = 1;
    EPwm2Regs.TZFRC.bit.CBC = 1;
    EPwm3Regs.TZFRC.bit.OST = 1;
    EPwm3Regs.TZFRC.bit.CBC = 1;






    //���TZ��־
    EPwm1Regs.TZCLR.bit.OST = 1;
    EPwm1Regs.TZCLR.bit.CBC = 1;
    EPwm2Regs.TZCLR.bit.OST = 1;
    EPwm2Regs.TZCLR.bit.CBC = 1;
    EPwm3Regs.TZCLR.bit.OST = 1;
    EPwm3Regs.TZCLR.bit.CBC = 1;

    EDIS;
}




