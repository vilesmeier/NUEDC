#ifndef __SPI_H
#define __SPI_H

#include <sys/stdint.h>

//�궨��
//�����
#define SPI_RESET 0
#define SPI_SET   1
//SPI����ģʽ����
#define _SPI_TRANSFER_MODE_ 3       
//�ӿڷ�װ
#define SPI_DI          GpioDataRegs.GPADAT.bit.GPIO11
#define SPI_DO          GpioDataRegs.GPADAT.bit.GPIO13
#define SPI_CS          GpioDataRegs.GPADAT.bit.GPIO15
#define SPI_CLK         GpioDataRegs.GPADAT.bit.GPIO9
#define SPI_DELAY_TIME  30

//�ṹ�嶨��

//����ʵ��
//CS����
void SPI_W_CS(unsigned char BitValue);

//SCK����
void SPI_W_SCK(unsigned char BitValue);

//MOSIģ��
void SPI_W_DI(unsigned char BitValue);

//MISOģ��
unsigned char SPI_R_DO(void);

//��ʼ��
void SPI_GPIO_Init(void);

//���俪ʼ
void SPI_Start(void);

//�������
void SPI_Stop(void);

//8λ���������ı�ByteSend������ֵ
unsigned char SPI_SwapByte(unsigned char ByteSend);

//����2byte,16λ
uint16_t SPI_SwapWord(uint16_t SwapWord);

#endif // 
