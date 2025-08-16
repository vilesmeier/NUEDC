#include "SPI.h"
#include "DSP28x_Project.h"

/* 
    GPIO模拟SPI通信
    encoding：GBK

    名称对应：
    DI —— MOSI
    DO —— MISO
    
    寄存器说明：
    CLK_PHASE       0——不延后(第一边沿采样)     1——延后半个周期(第二边沿采样)
    CLKPOLARITY     0——空闲低电平   1——空闲高电平

    mode setting:
    mode 0
        CLK_PHASE       0
        CLKPOLARITY     0
    mode 1
        CLK_PHASE       0
        CLKPOLARITY     1    
    mode 2
        CLK_PHASE       1
        CLKPOLARITY     0    
    mode 3
        CLK_PHASE       1
        CLKPOLARITY     1    
*/



//初始化
void SPI_GPIO_Init(void)
{
    //gpio模拟
    //D0为输入，其余为输出

    EALLOW;

    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;
    

    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;

    if(_SPI_TRANSFER_MODE_ == 0 || _SPI_TRANSFER_MODE_ == 2)
    {
        SPI_W_SCK(SPI_RESET);
    }
    else if(_SPI_TRANSFER_MODE_ == 1 || _SPI_TRANSFER_MODE_ == 3)
    {
        SPI_W_SCK(SPI_SET);
    }
    SPI_W_DI(SPI_RESET);
    SPI_W_CS(SPI_SET);

    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 1;

    EDIS;
}

//CS设置
void SPI_W_CS(unsigned char BitValue)
{
    if(BitValue)
    {
        SPI_CS = 1;
    }
    else
    {
        SPI_CS = 0;
    }
    
}

//SCK设置
void SPI_W_SCK(unsigned char BitValue)
{
    if(BitValue)
    {
        SPI_CLK = 1;
    }
    else
    {
        SPI_CLK = 0;
    }
}

//MOSI模拟
void SPI_W_DI(unsigned char BitValue)
{
    if(BitValue)
    {
        SPI_DI = 1;
    }
    else
    {
        SPI_DI = 0;
    }
}

//MISO模拟
unsigned char SPI_R_DO(void)
{
    return SPI_DO ;
}

//传输开始
void SPI_Start(void)
{
    SPI_W_CS(SPI_RESET);
    DELAY_US(SPI_DELAY_TIME);
}

//传输结束
void SPI_Stop(void)
{
    DELAY_US(SPI_DELAY_TIME);
    SPI_W_CS(SPI_SET);
}

//8位交换，不改变ByteSend本身数值
#if _SPI_TRANSFER_MODE_ == 0
//无延时，空闲低电平
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char i;
    unsigned char ByteReseive = 0x00;
    for(i=0;i < 8;i++)
    {
        //上升沿
        SPI_W_SCK(1);
        //移出数据
        SPI_W_DI(ByteSend & (0x80 >> i));
        DELAY_US(SPI_DELAY_TIME);
        //下降沿
        SPI_W_SCK(0);
        DELAY_US(SPI_DELAY_TIME);
        //移入数据
        if(SPI_R_DO() == 1)
        {
            ByteReseive |= (0x80 >> i);
        }
        DELAY_US(SPI_DELAY_TIME);
    }
    return ByteReseive;
}

#elif _SPI_TRANSFER_MODE_ == 1
//无延时，空闲高电平
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char i;
    unsigned char ByteReseive = 0x00;
    for(i=0;i < 8;i++)
    {
        //下降沿
        SPI_W_SCK(0);
        //移出数据
        SPI_W_DI(ByteSend & (0x80 >> i));
        DELAY_US(SPI_DELAY_TIME);
        //上升沿
        SPI_W_SCK(1);
        DELAY_US(SPI_DELAY_TIME);
        //移入数据
        if(SPI_R_DO() == 1)
        {
            ByteReseive |= (0x80 >> i);
        }
        DELAY_US(SPI_DELAY_TIME);
    }
    return ByteReseive;
}

#elif _SPI_TRANSFER_MODE_ == 2
//有延时,空闲低电平
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char receivedData = 0x00;
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        // 发送数据位
        SPI_W_DI(ByteSend & (0x80 >> i));
        // 产生时钟上升沿
        SPI_W_SCK(1);
        DELAY_US(SPI_DELAY_TIME);
        // 读取MISO上的数据
        if(SPI_R_DO() == 1)
        {
            receivedData |= (0x80 >> i);
        }
        // 产生时钟下降沿
        SPI_W_SCK(0);  // SCLK = 0
        DELAY_US(SPI_DELAY_TIME);
        // 准备发送下一位
    }
    return receivedData;
}

#elif _SPI_TRANSFER_MODE_ == 3
//有延时,空闲高电平
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char receivedData = 0x00;
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        // 发送数据位
        SPI_W_DI(ByteSend & (0x80 >> i));
        // 产生时钟下降沿
        SPI_W_SCK(0);
        DELAY_US(SPI_DELAY_TIME);
        // 读取MISO上的数据
        if(SPI_R_DO() == 1)
        {
            receivedData |= (0x80 >> i);
        }
        // 产生时钟上升沿
        SPI_W_SCK(1);  // SCLK = 0
        DELAY_US(SPI_DELAY_TIME);
        // 准备发送下一位
    }
    return receivedData;
}

#endif

//交换2byte,16位
uint16_t SPI_SwapWord(uint16_t SwapWord)
{
    uint16_t Receive_Temp;
    unsigned char Send_Temp_1,Send_Temp_2;
    Send_Temp_2 = SwapWord;
    Send_Temp_1 = SwapWord >> 8;
    Receive_Temp = SPI_SwapByte(Send_Temp_1);
    Receive_Temp = Receive_Temp << 8;
    Receive_Temp =Receive_Temp | SPI_SwapByte(Send_Temp_2);
    return Receive_Temp;
}
