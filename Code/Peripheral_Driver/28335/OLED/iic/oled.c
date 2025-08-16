#include "oled_font.h"
#include <oled_i2c.h>
#include "global.h"

/**
 *  @brief              IIC模块发送准备OK
 *  @parameter              无
 *  @return_value           无
 */
Uint16  I2C_xrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;   //IIC模块发送准备OK
    return t;
}

/**
 *  @brief              IIC模块接收准备OK
 *  @parameter              无
 *  @return_value           无
 */
Uint16  I2C_rrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;   //IIC模块接收准备OK
    return t;
}

/**
 *  @brief                  IIC写数据
 *  @parameter              SlaveAddress：从机地址；  RomAddress：寄存器地址； number：写入数据的字节数；  *Wdata：写入数据的地址
 *  @return_value           状态标志
 */
Uint16 WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint8 *Wdata)
{

    Uint16 i;
    while(!I2C_xrdy());      //等待数据发送就绪，XRDY=1,表明发送寄存器已经准备好接受新的数据
    DELAY_US(10);

    I2caRegs.I2CSAR = SlaveAddress&0xff;    //设备从地址
    I2caRegs.I2CCNT = number + 1;   //需要发送的字节数
    I2caRegs.I2CDXR = RomAddress&0xff;    //第一个发送字节为发送数据的目标寄存器地址
    DELAY_US(1);//等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
    I2caRegs.I2CMDR.all = 0x6E20;    //发送起始信号，内部数据计数器减到0时，发送停止信号，主机发送模式，使能IIC模式，
    for (i=0; i<number; i++)
    {
        while(!I2C_xrdy());   //等待数据发送就绪，发送下一个数据
        DELAY_US(10);

        I2caRegs.I2CDXR = *Wdata&0xFF;
        Wdata++;
    }
    DELAY_US(1);
    return I2C_SUCCESS;         //发送成功
}


/**
 *  @brief                  IIC写数据
 *  @parameter              SlaveAddress：从机地址；  RomAddress：寄存器地址； number：写入数据的字节数；  *RamAddr：读出数据存放的地址
 *  @return_value           状态标志
 */
Uint16 ReadData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint16  *RamAddr)
{
    Uint16  i,Temp;

    if (I2caRegs.I2CSTR.bit.BB == 1)  //返回总线忙错误状态
    {
        return I2C_BUS_BUSY_ERROR;
    }

    while(!I2C_xrdy());
    DELAY_US(10);

    I2caRegs.I2CSAR = SlaveAddress&0xff;      //设备从地址
    I2caRegs.I2CCNT = 1;        //发送一个字节为要读取数据的寄存器地址
    I2caRegs.I2CDXR = RomAddress&0xff;
    DELAY_US(1);//等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
   I2caRegs.I2CMDR.all = 0x6620;   //主机发送模式，自动清除停止位不产生停止信号

   if (I2caRegs.I2CSTR.bit.NACK == 1)    //送到无应答信号返回错误
        return  I2C_BUS_BUSY_ERROR;

    DELAY_US(50);

   while(!I2C_xrdy());                  //调试过程中有时会卡在这一步,表示通讯不正常
    DELAY_US(10);

    I2caRegs.I2CSAR = SlaveAddress&0xff;
    I2caRegs.I2CCNT = number;
    DELAY_US(1);//等待数据完全赋值到I2CDXR，再使能IIC，必须要加上！
    I2caRegs.I2CMDR.all = 0x6C20;   //主机接受模式，包含有停止信号

    if (I2caRegs.I2CSTR.bit.NACK == 1)
        return  I2C_BUS_BUSY_ERROR;

    for(i=0;i<number;i++)
    {
        while(!I2C_rrdy());
        DELAY_US(10);

        Temp = I2caRegs.I2CDRR;

        if (I2caRegs.I2CSTR.bit.NACK == 1)
            return    I2C_BUS_BUSY_ERROR;

        *RamAddr = Temp;
        RamAddr++;
    }
   return I2C_SUCCESS;    //接受数据成功
}

/**
 *  @brief                 IIC向寄存器的某一位写数据
 *  @parameter              SlaveAddress：从机地址；  regaddress：寄存器地址； bitNum：某一位；  data：写入数据
 *  @return_value           无
 */
void IICwriteBit(Uint16 slaveaddress, Uint16 regaddress, Uint8 bitNum, Uint8 data)
{
    Uint16 a;
    Uint8 b;
    DELAY_US(50);
    ReadData(slaveaddress,regaddress,1,&a);
    b=(Uint8 )(a&0xff);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    DELAY_US(50);
    WriteData(slaveaddress,regaddress,1,&b);
}

/**
 *  @brief                 IIC向寄存器的某几位写数据
 *  @parameter              SlaveAddress：从机地址；  regaddress：寄存器地址； bitStart：开始位；  length：长度；data：写入数据
 *  @return_value           无
 */
void IICwriteBits(Uint16 slaveaddress,Uint16 regaddress,Uint8 bitStart,Uint8 length,Uint8 data)
{

    Uint8 b,mask;
    Uint16 a;
    DELAY_US(50);
    ReadData(slaveaddress,regaddress,1,&a);
    b=(Uint8 )(a&0xff);
    mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
    data <<= (8 - length);
    data >>= (7 - bitStart);
    b &= mask;
    b |= data;
    DELAY_US(50);
    WriteData(slaveaddress,regaddress,1, &b);
}


/**
 *  @brief                   写入一个字节
 *  @parameter                  dat：写入的数据 ； rom：准备写入数据的寄存器地址
 *  @return_value               无
 */
void Send_Byte(Uint8 dat,Uint16 rom)
{
    while(WriteData(devAddr,rom,1,&dat));//等待发送成功
}

/**
 *  @brief                   向OLED发送一个字节
 *  @parameter                  dat： 写入的数据； mode：数据/命令 0表示命令，1表示数据
 *  @return_value               无
 */
void OLED_WR_Byte(Uint8 dat,Uint8 mode)
{
    DELAY_US(10);
    if(mode)
        Send_Byte(dat,0x40);//写数据
    else
        Send_Byte(dat,0x00);//写命令
    DELAY_US(10);
}

void OLED_Set_Pos(Uint8 x, Uint8 y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

/**
 *  @brief                   开启显示
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x14,OLED_CMD);//开启电荷泵
    OLED_WR_Byte(0xAF,OLED_CMD);//开启屏幕
}

/**
 *  @brief                   关闭显示
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//电荷泵使能
    OLED_WR_Byte(0x10,OLED_CMD);//关闭电荷泵
    OLED_WR_Byte(0xAE,OLED_CMD);//关闭屏幕
}

/**
 *  @brief                   清屏函数
 *  @parameter                 无
 *  @return_value               无
 */
void OLED_Clear(void)
{
    Uint8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);
        OLED_WR_Byte (0x00,OLED_CMD);
        OLED_WR_Byte (0x10,OLED_CMD);
        for(n=0;n<128;n++)
        {
            OLED_WR_Byte(0,OLED_DATA);
        }
    }
}

/**
 *  @brief                   在指定位置显示一个字符，包括部分字符
 *  @parameter              x:0~127； y:0~63；
 *  @return_value           无
 */
void OLED_ShowChar(Uint8 x,Uint8 y,Uint8 chr)
{
    Uint8 c=0,i=0;
    c=chr-' ';
    if(x>Max_Column-1)
    {
        x=0;
        y=y+2;
    }
    OLED_Set_Pos(x,y);
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(F8x16[c*16+i],OLED_DATA);
    }
    OLED_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(F8x16[c*16+i+8],OLED_DATA);
    }
}

/**
 *  @brief                显示字符串
 *  @parameter            x,y:起点坐标；  size1:选择字体大小 ； *chr:字符串起始地址；   mode:0,反色显示,1,正常显示
 *  @return_value           无
 */
void OLED_ShowString(Uint8 x,Uint8 y,Uint8 *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>120)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}

/**
 *  @brief                指数显示
 *  @parameter            m:底数； n:指数
 *  @return_value           结果
 */
Uint32 OLED_Pow(Uint8 m,Uint8 n)
{
    Uint32 result=1;
    while(n--)
    {
        result*=m;
    }
    return result;
}

/**
 *  @brief                显示数字
 *  @parameter            x,y:起点坐标；  num：要显示的数字； len：数字的位数；  size1:选择字体大小；   mode:0,反色显示,1,正常显示
 *  @return_value           无
 */
void OLED_ShowNum(Uint8 x,Uint8 y,Uint32 num,Uint8 len)
{
    Uint8 t,temp;
    Uint8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
        if(enshow==0 && t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(8)*t,y,' ');
                continue;
            }
            else
            {
                enshow=1;
            }
        }
        OLED_ShowChar(x+8*t,y,temp+'0');
    }
}

/**
 *  @brief               OLED初始化
 *  @parameter            无
 *  @return_value           无
 */
void OLED_Init(void)
{
    I2caRegs.I2CMDR.all = 0x0000;   //复位IIC

    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    //使能(SDAA)上拉
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //使能 (SCLA)上拉
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  //同步 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  //同步 (SCLA)
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   //配置 GPIO32为 SDAA
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   //配置GPIO33 为SCLA

    // 预分频——时钟模块的频率
    I2caRegs.I2CPSC.all = 14;       //预分频   IIC模块时钟需设置为7-12MHz，本实验设置为 (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 500;   //时钟低电平时间值
    I2caRegs.I2CCLKH = 500;   //时钟高电平时间值
    I2caRegs.I2CIER.all = 0x00;      //Enable SCD & ARDY interrupts

    I2caRegs.I2CFFTX.all = 0x0000;   //Disable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x0000;   //Disable RXFIFO, clear RXFFINT

//    I2caRegs.I2CMDR.all = 0x0020;
    I2caRegs.I2CMDR.bit.IRS = 1;
    I2caRegs.I2CMDR.bit.MST = 1;
    I2caRegs.I2CMDR.bit.TRX = 1;

    EDIS;

    DELAY_US(200000);

    OLED_WR_Byte(0xAE,OLED_CMD);
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0x10,OLED_CMD);
    OLED_WR_Byte(0x40,OLED_CMD);
    OLED_WR_Byte(0xB0,OLED_CMD);
    OLED_WR_Byte(0x81,OLED_CMD);
    OLED_WR_Byte(0xFF,OLED_CMD);
    OLED_WR_Byte(0xA1,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0xA8,OLED_CMD);
    OLED_WR_Byte(0x3F,OLED_CMD);
    OLED_WR_Byte(0xC8,OLED_CMD);
    OLED_WR_Byte(0xD3,OLED_CMD);
    OLED_WR_Byte(0x00,OLED_CMD);
    OLED_WR_Byte(0xD5,OLED_CMD);
    OLED_WR_Byte(0x80,OLED_CMD);
    OLED_WR_Byte(0xD8,OLED_CMD);
    OLED_WR_Byte(0x05,OLED_CMD);
    OLED_WR_Byte(0xD9,OLED_CMD);
    OLED_WR_Byte(0xF1,OLED_CMD);
    OLED_WR_Byte(0xDA,OLED_CMD);
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD);
    OLED_WR_Byte(0x30,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD);
    OLED_WR_Byte(0x14,OLED_CMD);
    OLED_WR_Byte(0xAF,OLED_CMD);
}
