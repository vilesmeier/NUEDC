/*****************************************************
 * 文件名：dac7512.h
 * 作者：曾新贵
 * 时间：2013年2月28日
 ****************************************************/

#ifndef DAC7512_H_
#define DAC7512_H_
/******端口定义**********/
#define		CLK			BIT3
#define		DAT			BIT4
#define		SYNC		BIT5
/******端口操作**********/
#define		DAT_0		P1OUT &= ~DAT
#define		DAT_1		P1OUT |= DAT
#define		CLK_0		P1OUT &= ~CLK
#define		CLK_1		P1OUT |= CLK
#define		SYNC_0		P1OUT &= ~SYNC
#define		SYNC_1		P1OUT |= SYNC
/******函数声明**********/
void dac7512_init();
void dac7512_out(unsigned int dat);

#endif /* DAC7512_H_ */
