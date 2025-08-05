#include "Global.h"

/*
    模块：规则采样法生成SPWM波形
    输入：频率、占空比、相位
    输出：SPWM波形
    周期:TBPRD/150(us)
    频率:150/TBPRD(MHz)
*/

//EPWM1初始化
void EPWM1_Init(Uint16 tbprd)
{
    //gpio config
    InitEPwm1Gpio();
    EALLOW;
    DINT;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC  = 1;
    //INT config
    PieVectTable.EPWM1_INT =&epwm1_timer_isr;
    IER |= M_INT3;
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;                  //使能第三组中断的第1个小中断
    EDIS;

    EALLOW;
    //EPWM config
    //时基模块控制
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;     //选择ePWMxSYNCO信号输出源
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;             //初始相位寄存器失能
    EPwm1Regs.TBPHS.half.TBPHS = 0;                     //初始相位寄存器值
    EPwm1Regs.TBCTR = 0x0000;                           //时基模块计数器
    EPwm1Regs.TBPRD = tbprd;                            //周期寄存器
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_DOWN;          //周期计数模式
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;              //TB模块分频控制
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV2;                 //TB模块分频控制

    //事件触发模块控制
    EPwm1Regs.ETSEL.bit.INTEN   = 1;                    //启用中断
    EPwm1Regs.ETSEL.bit.INTSEL  = ET_CTR_ZERO;          //中断触发模式
    EPwm1Regs.ETPS.bit.INTPRD   = ET_1ST;               //设置事件触发间隔
    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;                    // 启用SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;          // 选择CTR = 0作为触发事件
    EPwm1Regs.ETPS.bit.SOCAPRD  = ET_1ST;               // 设置事件触发间隔

    EDIS;
    EINT;                                               //开CPU级中断响应
    ERTM;
}

//中断服务程序
interrupt void epwm1_timer_isr(void)
{

    Flag_OLED_Refresh++;
    //功率分析
    POWER_MEAS_SINE_ANALYZER_run(Power_Measure_Global_Addr);
    //频率分析
    //FFT
    if(Flag_RFFT == 1)
    {
        RFFT_ADC_Cal();
        Current_THD_Calculate();
        Flag_RFFT = 0;
    }
    if(Flag_OLED_Refresh == 4000)
    {

        LCD_ShowFloat(32,0  ,Power_Measure_Global.iRms,5,3,BLACK, WHITE);//电流有效值
        LCD_ShowFloat(32,16 ,Power_Measure_Global.vRms,5,2,BLACK, WHITE);//电压有效值
        LCD_ShowFloat(8 ,48 ,Power_Measure_Global.pRms,5,2,BLACK, WHITE);//有功功率
        LCD_ShowFloat(8 ,64 ,Power_Measure_Global.powerFactor,5,4,BLACK, WHITE);//功率因数
        LCD_ShowFloat(24,80 ,Current_THD,5,4,BLACK, WHITE);//THD

        LCD_ShowFloat(8 ,32 ,N_zashu,2,0,BLACK, WHITE);//匝数比
        LCD_ShowFloat(16,96 ,rfft_analyse.Amp[1],4,3,BLACK, WHITE);//谐波电流1
        LCD_ShowFloat(16,112,rfft_analyse.Amp[2],4,3, BLACK, WHITE);//谐波电流2
        LCD_ShowFloat(16,128,rfft_analyse.Amp[3],4,3, BLACK, WHITE);//谐波电流3
        LCD_ShowFloat(16,144,rfft_analyse.Amp[4],4,3, BLACK, WHITE);//谐波电流4
        LCD_ShowFloat(80,32 ,rfft_analyse.Amp[5],4,3, BLACK, WHITE);//谐波电流5
        LCD_ShowFloat(80,96 ,rfft_analyse.Amp[6],4,3, BLACK, WHITE);//谐波电流6
        LCD_ShowFloat(80,112,rfft_analyse.Amp[7],4,3, BLACK, WHITE);//谐波电流7
        LCD_ShowFloat(80,128,rfft_analyse.Amp[8],4,3, BLACK, WHITE);//谐波电流8

        LCD_ShowFloat(88,144,rfft_analyse.Amp[9],3,2, BLACK, WHITE);//谐波电流9
        Flag_OLED_Refresh = 0;
    }
    EPwm1Regs.ETCLR.bit.INT = 1;                         //中断标志位清零才能进入下一次的中断，在PWM的ET模块中
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP3;             //使能中断   PIEACK_GROUP3
}
