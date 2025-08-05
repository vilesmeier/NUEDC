#ifndef _KEY_H
#define _KEY_H

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

//接口封装
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

#define KEYPAD_COL_1_ON     GpioDataRegs.GPADAT.bit.GPIO11=1
#define KEYPAD_COL_1_OFF    GpioDataRegs.GPADAT.bit.GPIO11=0

#define KEYPAD_COL_2_ON     GpioDataRegs.GPADAT.bit.GPIO13=1
#define KEYPAD_COL_2_OFF    GpioDataRegs.GPADAT.bit.GPIO13=0

#define KEYPAD_COL_3_ON     GpioDataRegs.GPADAT.bit.GPIO15=1
#define KEYPAD_COL_3_OFF    GpioDataRegs.GPADAT.bit.GPIO15=0

#define KEYPAD_COL_4_ON     GpioDataRegs.GPADAT.bit.GPIO17=1
#define KEYPAD_COL_4_OFF    GpioDataRegs.GPADAT.bit.GPIO17=0

#define KEYPAD_ROW_1        GpioDataRegs.GPADAT.bit.GPIO19
#define KEYPAD_ROW_2        GpioDataRegs.GPADAT.bit.GPIO21
#define KEYPAD_ROW_3        GpioDataRegs.GPADAT.bit.GPIO23
#define KEYPAD_ROW_4        GpioDataRegs.GPADAT.bit.GPIO25

//函数功能

//初始化
void keypad_init(void);

//获取按键
char keypad_getkey(void);

#endif
