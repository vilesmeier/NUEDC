#include "Global.h"

float voltage;
//全局变量初始化
void ADC_Global_Variable_Init(void)
{
    Input_Ptr = 0;
    Flag_RFFT = 0;
    Flag_OLED_Refresh = 0;
    N_zashu = 1;
}

//ADC初始化
void ADC_Init(void)
{   
    //全局中断配置
    EALLOW;

    PieVectTable.ADCINT = &ADC_int_isr;         //中断向量编辑

    IER |= M_INT1;                              // 启用CPU中断
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;          // 启用PIE中断

                                                //ADC设置
    InitAdc();                                  //ADC初始化
                                                //ADC控制设置
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0x1;           //采样时间 5个时钟周期
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x3;         //ADC时钟分频
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;             //开始转换信号源
    AdcRegs.ADCREFSEL.bit.REF_SEL = 0;          //内部参考
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;           //0——uncontinuous   1——continuous run
    AdcRegs.ADCTRL3.bit.SMODE_SEL=1;            //设置同步采样模式
    AdcRegs.ADCTRL1.bit.SEQ_CASC=1;             //设置单排序操作方式
                                                //转换顺序设定，SELSEQx寄存器4个一组
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1= 0x0;      //设置最大转换通道数，8对转换，共16个通道
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;      //设置A0和B0作为第一个变换
                                                //ADC中断设置
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;     //打开SEQ1中断
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0x0;     //设置中断产生时间
                                                //ADC触发模式选择
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 0x1;   //使能 PWMA SOC触发

    EINT;                                       // 启用全局中断
    EDIS;
}

//模块初始化
void Sample_Init(void)
{
    ADC_Global_Variable_Init();
    ADC_Init();
}

//ADC转换值计算
void ADC_Get_Value(void)
{
    Power_Measure_Global_Addr -> v = (AdcRegs.ADCRESULT0 >> 4) * 0.00073242187;
    Power_Measure_Global_Addr -> i = ((AdcRegs.ADCRESULT1 >> 4) * 0.00073242187 - Current_Bias) / ((float)N_zashu)*Current_Multiply;
    FFT_input[Input_Ptr] =           ((AdcRegs.ADCRESULT1 >> 4) * 0.00073242187 - Current_Bias)*Current_Multiply/((float)N_zashu);
    Input_Ptr++;
    if(Input_Ptr >= FFT_SIZE)
    {
        Input_Ptr = 0;
        Flag_RFFT = 1;
    }
}

void Power_Measure_Init(void)
{
    POWER_MEAS_SINE_ANALYZER_reset(Power_Measure_Global_Addr);
    POWER_MEAS_SINE_ANALYZER_config(Power_Measure_Global_Addr,Fs,0,Grid_Freq + 5,Grid_Freq - 5);
}

//ADC中断服务函数
interrupt void ADC_int_isr(void)
{
    ADC_Get_Value();
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;                   //清除中断响应标志位
}
