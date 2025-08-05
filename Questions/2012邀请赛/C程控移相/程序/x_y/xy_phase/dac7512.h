/*****************************************************
 * �ļ�����dac7512.h
 * ���ߣ����¹�
 * ʱ�䣺2013��2��28��
 ****************************************************/

#ifndef DAC7512_H_
#define DAC7512_H_
/******�˿ڶ���**********/
#define		CLK			BIT3
#define		DAT			BIT4
#define		SYNC		BIT5
/******�˿ڲ���**********/
#define		DAT_0		P1OUT &= ~DAT
#define		DAT_1		P1OUT |= DAT
#define		CLK_0		P1OUT &= ~CLK
#define		CLK_1		P1OUT |= CLK
#define		SYNC_0		P1OUT &= ~SYNC
#define		SYNC_1		P1OUT |= SYNC
/******��������**********/
void dac7512_init();
void dac7512_out(unsigned int dat);

#endif /* DAC7512_H_ */
