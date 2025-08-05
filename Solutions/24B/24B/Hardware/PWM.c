#include "Global.h"

/*
    ģ�飺�������������SPWM����
    ���룺Ƶ�ʡ�ռ�ձȡ���λ
    �����SPWM����
    ����:TBPRD/150(us)
    Ƶ��:150/TBPRD(MHz)
*/

//EPWM1��ʼ��
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
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;                  //ʹ�ܵ������жϵĵ�1��С�ж�
    EDIS;

    EALLOW;
    //EPWM config
    //ʱ��ģ�����
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;     //ѡ��ePWMxSYNCO�ź����Դ
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;             //��ʼ��λ�Ĵ���ʧ��
    EPwm1Regs.TBPHS.half.TBPHS = 0;                     //��ʼ��λ�Ĵ���ֵ
    EPwm1Regs.TBCTR = 0x0000;                           //ʱ��ģ�������
    EPwm1Regs.TBPRD = tbprd;                            //���ڼĴ���
    EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_DOWN;          //���ڼ���ģʽ
    EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV1;              //TBģ���Ƶ����
    EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV2;                 //TBģ���Ƶ����

    //�¼�����ģ�����
    EPwm1Regs.ETSEL.bit.INTEN   = 1;                    //�����ж�
    EPwm1Regs.ETSEL.bit.INTSEL  = ET_CTR_ZERO;          //�жϴ���ģʽ
    EPwm1Regs.ETPS.bit.INTPRD   = ET_1ST;               //�����¼��������
    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;                    // ����SOCA
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;          // ѡ��CTR = 0��Ϊ�����¼�
    EPwm1Regs.ETPS.bit.SOCAPRD  = ET_1ST;               // �����¼��������

    EDIS;
    EINT;                                               //��CPU���ж���Ӧ
    ERTM;
}

//�жϷ������
interrupt void epwm1_timer_isr(void)
{

    Flag_OLED_Refresh++;
    //���ʷ���
    POWER_MEAS_SINE_ANALYZER_run(Power_Measure_Global_Addr);
    //Ƶ�ʷ���
    //FFT
    if(Flag_RFFT == 1)
    {
        RFFT_ADC_Cal();
        Current_THD_Calculate();
        Flag_RFFT = 0;
    }
    if(Flag_OLED_Refresh == 4000)
    {

        LCD_ShowFloat(32,0  ,Power_Measure_Global.iRms,5,3,BLACK, WHITE);//������Чֵ
        LCD_ShowFloat(32,16 ,Power_Measure_Global.vRms,5,2,BLACK, WHITE);//��ѹ��Чֵ
        LCD_ShowFloat(8 ,48 ,Power_Measure_Global.pRms,5,2,BLACK, WHITE);//�й�����
        LCD_ShowFloat(8 ,64 ,Power_Measure_Global.powerFactor,5,4,BLACK, WHITE);//��������
        LCD_ShowFloat(24,80 ,Current_THD,5,4,BLACK, WHITE);//THD

        LCD_ShowFloat(8 ,32 ,N_zashu,2,0,BLACK, WHITE);//������
        LCD_ShowFloat(16,96 ,rfft_analyse.Amp[1],4,3,BLACK, WHITE);//г������1
        LCD_ShowFloat(16,112,rfft_analyse.Amp[2],4,3, BLACK, WHITE);//г������2
        LCD_ShowFloat(16,128,rfft_analyse.Amp[3],4,3, BLACK, WHITE);//г������3
        LCD_ShowFloat(16,144,rfft_analyse.Amp[4],4,3, BLACK, WHITE);//г������4
        LCD_ShowFloat(80,32 ,rfft_analyse.Amp[5],4,3, BLACK, WHITE);//г������5
        LCD_ShowFloat(80,96 ,rfft_analyse.Amp[6],4,3, BLACK, WHITE);//г������6
        LCD_ShowFloat(80,112,rfft_analyse.Amp[7],4,3, BLACK, WHITE);//г������7
        LCD_ShowFloat(80,128,rfft_analyse.Amp[8],4,3, BLACK, WHITE);//г������8

        LCD_ShowFloat(88,144,rfft_analyse.Amp[9],3,2, BLACK, WHITE);//г������9
        Flag_OLED_Refresh = 0;
    }
    EPwm1Regs.ETCLR.bit.INT = 1;                         //�жϱ�־λ������ܽ�����һ�ε��жϣ���PWM��ETģ����
    PieCtrlRegs.PIEACK.all  = PIEACK_GROUP3;             //ʹ���ж�   PIEACK_GROUP3
}
