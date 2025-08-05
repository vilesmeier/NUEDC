/*****************************************************
 * �ļ�����main.c
 * ˵����ͨ������1����ģʽ�趨��ʵ���Զ�����Ͱ����ֶ����ࡣ
 * ���ߣ����¹�
 * ʱ�䣺2013��2��28��
 ****************************************************/
#include <MSp430G2553.h>
#include "dac7512.h"
/******�˿ڶ���**********/
#define		LED			BIT0
#define		KEY1		BIT1
#define		KEY2		BIT2
/******LED�˿ڲ���******/
#define		LED_ON		P1OUT |= LED		//����LED
#define		LED_OFF		P1OUT &= ~LED		//Ϩ��LED
/******��������**********/
void clock_init();							//ϵͳʱ�ӳ�ʼ��
void sys_init();							//ϵͳ��ʼ��
unsigned char key2_scanf();					//����ɨ��
void set_phase(unsigned char phase_val);	//�̿���λ

/******ȫ�ֱ���**********/
unsigned char Mode = 0, Phase = 0;
/*******************************
 * ��������main
 * ��ڲ�������
 * ���ڲ�������
 * ˵����ͨ�������趨����ģʽ����ʵ���Զ�����Ͱ����ֶ����ࡣ
********************************/
void main(void)
{
	unsigned char i = 0;
	_EINT();
	sys_init();
/*	dac7512_out(400);
	da_val = 400;
	while(1)
	{
		if(key2_scanf())
			da_val += 200;
		dac7512_out(da_val);
	}*/
	while(1)
	{
		if(Mode == 0)				//�Զ�����
		{
			LED_ON;
			set_phase(Phase);
			Phase += 1;
			Phase %= 60;
			for(i = 0; i< 20; i++)
				_delay_cycles(6000);
		}
		else						//�����ֶ�����
		{
			LED_OFF;
			if(key2_scanf())
				Phase += 5;
			Phase %= 60;
			set_phase(Phase);
		}
	}
}

/*******************************
 * ��������sys_init
 * ��ڲ�������
 * ���ڲ�������
 * ˵������ϵͳ��ʱ�ӡ��˿��Լ�DA���г�ʼ����
********************************/
void sys_init()
{
	clock_init();

	P1DIR = 0xff;
	P1DIR &= ~KEY2;

	P1DIR &= ~KEY1;
	P1IE |= KEY1;			//�����ж�
	P1IES |= KEY1;			//�½����ж�
	P1REN |= KEY1;
	P1OUT |= KEY1;

	dac7512_init();
}
/*******************************
 * ��������clock_init
 * ��ڲ�������
 * ���ڲ�������
 * ˵��������ϵͳʱ��
********************************/
void clock_init()
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if(CALBC1_12MHZ==0XFF||CALDCO_12MHZ==0XFF)
	{
		while(1);
	}
	BCSCTL1 = CALBC1_1MHZ;                    // Set range
	DCOCTL = CALDCO_1MHZ;                     // ����DCLK=1M
}
/*******************************
 * ��������key2_scanf
 * ��ڲ�������
 * ���ڲ�����is_press
 * ˵����ɨ�谴���Ƿ��£������򷵻�true�����򷵻�false��
********************************/
unsigned char key2_scanf()
{
	unsigned char is_press = 0;
	if((P1IN & KEY2) == 0)
	{
		_delay_cycles(6000);
		if((P1IN & KEY2) == 0)
		{
			while((P1IN & KEY2) == 0);
			is_press = 1;
			_delay_cycles(6000);
		}
	}
	return is_press;
}
/*******************************
 * ��������set_phase
 * ��ڲ�����phase_val,��Ŀ����λֵ��
 * ���ڲ�������
 * ˵��������Ŀ����λֵ��ͨ������ó�DA�������ѹ�����Ʊ��ݶ�����ʵ�ֳ̿����ࡣ
********************************/
void set_phase(unsigned char phase_val)
{
	unsigned int da_val =0;
	da_val = 4*phase_val * phase_val /10;
	da_val += 3192;
	da_val -= 672/10*phase_val;
	dac7512_out(da_val);
}
/*******************************
 * ��������PORT1_ISR
 * ��ڲ�������
 * ���ڲ�������
 * ˵��������1���жϷ�����������ģʽ�趨��
********************************/
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void)
{
	_delay_cycles(6000);
	Mode^=1;
	_delay_cycles(6000);
	while((P1IN & KEY1) == 0);
	P1IFG &=0;
}
