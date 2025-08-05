#include "DSP28x_Project.h"
#include "key.h"

/*
    对应硬件： 4x4矩阵键盘
    encoding： GBK(GB2312)
    author: SDUEE mcb
    C1    11
    C2    13
    C3    15
    C4    17
    R1    19
    R2    21  
    R3    23      
    R4    25
*/

//按键映射表
char keys[KEYPAD_ROWS][KEYPAD_COLS] = 
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//接口初始化
void keypad_init(void) 
{
    EALLOW;

    //C1
    GpioCtrlRegs.GPAMUX1.bit.GPIO11  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;
    GpioDataRegs.GPADAT.bit.GPIO11 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
    
    //C2
    GpioCtrlRegs.GPAMUX1.bit.GPIO13  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;
    GpioDataRegs.GPADAT.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
    
    //C3
    GpioCtrlRegs.GPAMUX1.bit.GPIO15  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;
    GpioDataRegs.GPADAT.bit.GPIO15 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;
    
    //C4
    GpioCtrlRegs.GPAMUX1.bit.GPIO17  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;
    GpioDataRegs.GPADAT.bit.GPIO17 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;
    
    //R1
    GpioCtrlRegs.GPAMUX1.bit.GPIO19  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;
    GpioDataRegs.GPADAT.bit.GPIO19 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;
    
    //R2
    GpioCtrlRegs.GPAMUX1.bit.GPIO21  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 0;
    GpioDataRegs.GPADAT.bit.GPIO21 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;

    //R3
    GpioCtrlRegs.GPAMUX1.bit.GPIO23  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;
    GpioDataRegs.GPADAT.bit.GPIO23 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;
    
    //R4
    GpioCtrlRegs.GPAMUX1.bit.GPIO25  = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;
    GpioDataRegs.GPADAT.bit.GPIO25 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;
    
    EDIS;
}

//获取按键
char keypad_getkey(void) 
{
    //扫描法确定按键
    {
        KEYPAD_COL_1_ON;
        if (KEYPAD_ROW_1)
        {
            while(KEYPAD_ROW_1)
            return keys[0][0];
        }
        else if (KEYPAD_ROW_2)
        {
            while(KEYPAD_ROW_2)
            return keys[0][1]
        }
        else if(KEYPAD_ROW_3)
        {
            while(KEYPAD_ROW_3);  
            return keys[0][2]
        }
        else if(KEYPAD_ROW_4)
        {
            while(KEYPAD_ROW_4);
            return keys[0][3]
        }
        KEYPAD_COL_1_OFF;
    }
    {
        KEYPAD_COL_2_ON;
        if (KEYPAD_ROW_1)
        {
            while(KEYPAD_ROW_1)
            return keys[1][0];
        }
        else if (KEYPAD_ROW_2)
        {
            while(KEYPAD_ROW_2)
            return keys[1][1]
        }
        else if(KEYPAD_ROW_3)
        {
            while(KEYPAD_ROW_3);
            return keys[1][2]
        }
        else if(KEYPAD_ROW_4)
        {
            while(KEYPAD_ROW_4); 
            return keys[1][3]
        }
        KEYPAD_COL_2_OFF;
    }
    {
        KEYPAD_COL_3_ON;
        if (KEYPAD_ROW_1)
        {
            while(KEYPAD_ROW_1)
            return keys[2][0];
        }
        else if (KEYPAD_ROW_2)
        {
            while(KEYPAD_ROW_2)
            return keys[2][1]
        }
        else if(KEYPAD_ROW_3)
        {
            while(KEYPAD_ROW_3);
            return keys[2][2]
        }
        else if(KEYPAD_ROW_4)
        {
            while(KEYPAD_ROW_4);
            return keys[2][3]
        }
        KEYPAD_COL_3_OFF;
    }
    {
        KEYPAD_COL_4_ON;
        if (KEYPAD_ROW_1)
        {
            while(KEYPAD_ROW_1)
            return keys[3][0];
        }
        else if (KEYPAD_ROW_2)
        {
            while(KEYPAD_ROW_2)
            return keys[3][1]
        }
        else if(KEYPAD_ROW_3)
        {
            while(KEYPAD_ROW_3);
            return keys[3][2]
        }
        else if(KEYPAD_ROW_4)
        {
            while(KEYPAD_ROW_4);
            return keys[3][3]
        }
        KEYPAD_COL_4_OFF;
    }
    //没有键被按下
    return '\0';
}
