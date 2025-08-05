/*****************************************************
 * 文件名：main.c
 * 说明：通过按键1进行模式设定，实现自动移相和按键手动移相。
 * 作者：曾新贵
 * 时间：2013年2月28日
 ****************************************************/
#include <MSp430G2553.h>
#include "dac7512.h"
/******端口定义**********/
#define		LED			BIT0
#define		KEY1		BIT1
#define		KEY2		BIT2
/******LED端口操作******/
#define		LED_ON		P1OUT |= LED		//点亮LED
#define		LED_OFF		P1OUT &= ~LED		//熄灭LED
/******函数声明**********/
void clock_init();							//系统时钟初始化
void sys_init();							//系统初始化
unsigned char key2_scanf();					//按键扫描
void set_phase(unsigned char phase_val);	//程控相位

/******全局变量**********/
unsigned char Mode = 0, Phase = 0;
/*******************************
 * 函数名：main
 * 入口参数：无
 * 出口参数：无
 * 说明：通过按键设定移相模式，可实现自动移相和按键手动移相。
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
		if(Mode == 0)				//自动移相
		{
			LED_ON;
			set_phase(Phase);
			Phase += 1;
			Phase %= 60;
			for(i = 0; i< 20; i++)
				_delay_cycles(6000);
		}
		else						//按键手动移相
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
 * 函数名：sys_init
 * 入口参数：无
 * 出口参数：无
 * 说明：对系统的时钟、端口以及DA进行初始化。
********************************/
void sys_init()
{
	clock_init();

	P1DIR = 0xff;
	P1DIR &= ~KEY2;

	P1DIR &= ~KEY1;
	P1IE |= KEY1;			//允许中断
	P1IES |= KEY1;			//下降沿中断
	P1REN |= KEY1;
	P1OUT |= KEY1;

	dac7512_init();
}
/*******************************
 * 函数名：clock_init
 * 入口参数：无
 * 出口参数：无
 * 说明：设置系统时钟
********************************/
void clock_init()
{
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	if(CALBC1_12MHZ==0XFF||CALDCO_12MHZ==0XFF)
	{
		while(1);
	}
	BCSCTL1 = CALBC1_1MHZ;                    // Set range
	DCOCTL = CALDCO_1MHZ;                     // 设置DCLK=1M
}
/*******************************
 * 函数名：key2_scanf
 * 入口参数：无
 * 出口参数：is_press
 * 说明：扫描按键是否按下，按下则返回true，否则返回false。
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
 * 函数名：set_phase
 * 入口参数：phase_val,即目标相位值。
 * 出口参数：无
 * 说明：设置目标相位值，通过计算得出DA的输出电压，控制变容二极管实现程控移相。
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
 * 函数名：PORT1_ISR
 * 入口参数：无
 * 出口参数：无
 * 说明：按键1的中断服务函数，进行模式设定。
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
