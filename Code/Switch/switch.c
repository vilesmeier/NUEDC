#include <Global.h>
/*
    �̵�������
    �̵����ߵ�ƽ��ͨ
    δ��ʼ��ʱ����
*/

void Switch_Init(void)
{      
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   
    GpioDataRegs.GPADAT.bit.GPIO8 = Switch_Off;   
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;
}
