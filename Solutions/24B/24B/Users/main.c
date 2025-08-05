#include <Hardware/Global.h>

void main(void)
{
    //系统初始化
    InitSysCtrl();
    //MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    //InitFlash();
    InitGpio();
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
    EINT;
    ERTM;
    //低功耗
    System_Clock_Close();
    //模块初始化
    lcdInit();
    LCD_ShowString(0 ,0  ,32, 16, "IRMS", BLACK, WHITE);//电流有效值
    LCD_ShowString(0 ,16 ,32, 16, "VRMS", BLACK, WHITE);//电压有效值
    LCD_ShowString(0 ,32 ,8 , 16, "n"   , BLACK, WHITE);//匝数比
    LCD_ShowString(0 ,48 ,8 , 16, "P"   , BLACK, WHITE);//有功功率
    LCD_ShowString(0 ,64 ,8 , 16, "K"   , BLACK, WHITE);//功率因数
    LCD_ShowString(0 ,80 ,24, 16, "THD" , BLACK, WHITE);//THD
    LCD_ShowString(0 ,96 ,8 , 16, "2"  , BLACK, WHITE);//谐波电流1
    LCD_ShowString(0 ,112,8 , 16, "3"  , BLACK, WHITE);//谐波电流2
    LCD_ShowString(0 ,128,8 , 16, "4"  , BLACK, WHITE);//谐波电流3
    LCD_ShowString(0 ,144,8 , 16, "5"  , BLACK, WHITE);//谐波电流4
    LCD_ShowString(68,32 ,8 , 16, "6"  , BLACK, WHITE);//谐波电流5
    LCD_ShowString(68,96 ,8 , 16, "7"  , BLACK, WHITE);//谐波电流6
    LCD_ShowString(68,112,8 , 16, "8"  , BLACK, WHITE);//谐波电流7
    LCD_ShowString(68,128,8 , 16, "9"  , BLACK, WHITE);//谐波电流8
    LCD_ShowString(68,144,16, 16, "10" , BLACK, WHITE);//谐波电流9
    XINT_Init();
    Power_Measure_Init();
    RFFT_ADC_Init();
    Sample_Init();
    EPWM1_Init(36621);
    while(1)
    {
    }
}
