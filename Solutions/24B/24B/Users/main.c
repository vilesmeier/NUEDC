#include <Hardware/Global.h>

void main(void)
{
    //ϵͳ��ʼ��
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
    //�͹���
    System_Clock_Close();
    //ģ���ʼ��
    lcdInit();
    LCD_ShowString(0 ,0  ,32, 16, "IRMS", BLACK, WHITE);//������Чֵ
    LCD_ShowString(0 ,16 ,32, 16, "VRMS", BLACK, WHITE);//��ѹ��Чֵ
    LCD_ShowString(0 ,32 ,8 , 16, "n"   , BLACK, WHITE);//������
    LCD_ShowString(0 ,48 ,8 , 16, "P"   , BLACK, WHITE);//�й�����
    LCD_ShowString(0 ,64 ,8 , 16, "K"   , BLACK, WHITE);//��������
    LCD_ShowString(0 ,80 ,24, 16, "THD" , BLACK, WHITE);//THD
    LCD_ShowString(0 ,96 ,8 , 16, "2"  , BLACK, WHITE);//г������1
    LCD_ShowString(0 ,112,8 , 16, "3"  , BLACK, WHITE);//г������2
    LCD_ShowString(0 ,128,8 , 16, "4"  , BLACK, WHITE);//г������3
    LCD_ShowString(0 ,144,8 , 16, "5"  , BLACK, WHITE);//г������4
    LCD_ShowString(68,32 ,8 , 16, "6"  , BLACK, WHITE);//г������5
    LCD_ShowString(68,96 ,8 , 16, "7"  , BLACK, WHITE);//г������6
    LCD_ShowString(68,112,8 , 16, "8"  , BLACK, WHITE);//г������7
    LCD_ShowString(68,128,8 , 16, "9"  , BLACK, WHITE);//г������8
    LCD_ShowString(68,144,16, 16, "10" , BLACK, WHITE);//г������9
    XINT_Init();
    Power_Measure_Init();
    RFFT_ADC_Init();
    Sample_Init();
    EPWM1_Init(36621);
    while(1)
    {
    }
}
