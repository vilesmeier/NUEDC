#include "Global.h"

float voltage;
//ȫ�ֱ�����ʼ��
void ADC_Global_Variable_Init(void)
{
    Input_Ptr = 0;
    Flag_RFFT = 0;
    Flag_OLED_Refresh = 0;
    N_zashu = 1;
}

//ADC��ʼ��
void ADC_Init(void)
{   
    //ȫ���ж�����
    EALLOW;

    PieVectTable.ADCINT = &ADC_int_isr;         //�ж������༭

    IER |= M_INT1;                              // ����CPU�ж�
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;          // ����PIE�ж�

                                                //ADC����
    InitAdc();                                  //ADC��ʼ��
                                                //ADC��������
    AdcRegs.ADCTRL1.bit.ACQ_PS = 0x1;           //����ʱ�� 5��ʱ������
    AdcRegs.ADCTRL3.bit.ADCCLKPS = 0x3;         //ADCʱ�ӷ�Ƶ
    AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;             //��ʼת���ź�Դ
    AdcRegs.ADCREFSEL.bit.REF_SEL = 0;          //�ڲ��ο�
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;           //0����uncontinuous   1����continuous run
    AdcRegs.ADCTRL3.bit.SMODE_SEL=1;            //����ͬ������ģʽ
    AdcRegs.ADCTRL1.bit.SEQ_CASC=1;             //���õ����������ʽ
                                                //ת��˳���趨��SELSEQx�Ĵ���4��һ��
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1= 0x0;      //�������ת��ͨ������8��ת������16��ͨ��
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;      //����A0��B0��Ϊ��һ���任
                                                //ADC�ж�����
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;     //��SEQ1�ж�
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0x0;     //�����жϲ���ʱ��
                                                //ADC����ģʽѡ��
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 0x1;   //ʹ�� PWMA SOC����

    EINT;                                       // ����ȫ���ж�
    EDIS;
}

//ģ���ʼ��
void Sample_Init(void)
{
    ADC_Global_Variable_Init();
    ADC_Init();
}

//ADCת��ֵ����
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

//ADC�жϷ�����
interrupt void ADC_int_isr(void)
{
    ADC_Get_Value();
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;                         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;                       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;                   //����ж���Ӧ��־λ
}
