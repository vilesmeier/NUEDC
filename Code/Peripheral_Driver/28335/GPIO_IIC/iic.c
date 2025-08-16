#include "DSP28x_Project.h"
#include "IIC.h"

unsigned char AckBit;

//总线反转电平函数
void I2C_W_SCL(unsigned char BitValue)
{
	if(BitValue)
    {
        IIC_SCL_PIN = 1;
    }
    else
    {
        IIC_SCL_PIN = 0;
    }
	DELAY_US(10);
}

void I2C_W_SDA(unsigned char BitValue)
{
	if(BitValue)
    {
        IIC_SDA_PIN = 1;
    }
    else
    {
        IIC_SDA_PIN = 0;
    }
	DELAY_US(10);
}

//获取SDA总线电平
unsigned char I2C_R_SDA(void)
{
	return IIC_SDA_PIN;
}

//初始化
void I2C_Init(void)
{
    EALLOW;

    //IIC_SDA
    GpioDataRegs.GPADAT.bit.GPIO11  = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO11  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO11  = 1;
    
    //IIC_SCL
    GpioDataRegs.GPADAT.bit.GPIO12  = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO12  = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO12  = 1;

    EDIS;
}

//传输开始
void I2C_Start(void)
{
    I2C_W_SDA(1);
    I2C_W_SCL(1);
    I2C_W_SDA(0);
    I2C_W_SCL(0);
}

//传输结束
void I2C_Stop(void)
{
    I2C_W_SDA(0);
    I2C_W_SCL(1);
    I2C_W_SDA(1);
}

//发送数据
void I2C_SendData(unsigned char Byte)
{
    unsigned char i;
    for(i=0; i<8; i++ )
    {
        I2C_W_SDA(Byte & (0x80 >> i));
        I2C_W_SCL(1);
        I2C_W_SCL(0);
    }
}

//接收数据
unsigned char I2C_ReadData(void)
{
    unsigned char i,byte=0x00;
    I2C_W_SDA(1);
    for ( i = 0; i < 8 ; i++)
    {
        I2C_W_SCL(1);
        if (I2C_R_SDA() == 1)
        {
            byte = byte | (0x80 >> i);
        }
        I2C_W_SCL(0);
    }
    return byte;
}

//发送应答
void I2C_SendAck(unsigned char AckBit)
{
    I2C_W_SDA(AckBit);
    I2C_W_SCL(1);
    I2C_W_SCL(0);

}

//接收应答
unsigned char I2C_ReadAck(void)
{
    
    I2C_W_SDA(1);
    I2C_W_SCL(1);
    AckBit = I2C_R_SDA();
    I2C_W_SCL(0);
    return AckBit;
}

//发送地址及命令
void I2C_SendAddr(unsigned char Addr,unsigned char oper)
{
    Addr = Addr << 1;
    Addr = Addr | oper;
    I2C_SendData(Addr);
}
