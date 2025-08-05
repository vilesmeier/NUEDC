#include <Global.h>
/*
    继电器驱动
    继电器高电平导通
    未初始化时常关
*/

void Switch_Init(void)
{      
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   
    GpioDataRegs.GPADAT.bit.GPIO8 = Switch_Off;   
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;
}
