#ifndef __IIC_H
#define __IIC_H

//接口封装
#define IIC_SCL_PIN     GpioDataRegs.GPADAT.bit.GPIO13
#define IIC_SDA_PIN     GpioDataRegs.GPADAT.bit.GPIO11
#define IIC_OPER_READ   1
#define IIC_OPER_WRITE  0
//功能函数
unsigned int I2C_R_SDA(void);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendData(unsigned int Byte);
unsigned int I2C_ReadData(void);
void I2C_SendAck(unsigned int AckBit);
unsigned int I2C_ReadAck(void);
unsigned char I2C_SendAddr(unsigned char Addr,unsigned char oper);

#endif // !__IIC_H
