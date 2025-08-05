#ifndef __SPI_H
#define __SPI_H

#include <sys/stdint.h>

//宏定义
//命令定义
#define SPI_RESET 0
#define SPI_SET   1
//SPI传输模式设置
#define _SPI_TRANSFER_MODE_ 3       
//接口封装
#define SPI_DI          GpioDataRegs.GPADAT.bit.GPIO11
#define SPI_DO          GpioDataRegs.GPADAT.bit.GPIO13
#define SPI_CS          GpioDataRegs.GPADAT.bit.GPIO15
#define SPI_CLK         GpioDataRegs.GPADAT.bit.GPIO9
#define SPI_DELAY_TIME  30

//结构体定义

//函数实现
//CS设置
void SPI_W_CS(unsigned char BitValue);

//SCK设置
void SPI_W_SCK(unsigned char BitValue);

//MOSI模拟
void SPI_W_DI(unsigned char BitValue);

//MISO模拟
unsigned char SPI_R_DO(void);

//初始化
void SPI_GPIO_Init(void);

//传输开始
void SPI_Start(void);

//传输结束
void SPI_Stop(void);

//8位交换，不改变ByteSend本身数值
unsigned char SPI_SwapByte(unsigned char ByteSend);

//交换2byte,16位
uint16_t SPI_SwapWord(uint16_t SwapWord);

#endif // 
