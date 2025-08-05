#include "oled_font.h"
#include "oled.h"
#include "global.h"

/**
  * @brief OLED初始化
  * 
  */
void OLED_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    //使能(SDAA)上拉
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //使能 (SCLA)上拉
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  //同步 (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  //同步 (SCLA)
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   //配置 GPIO32为 SDAA
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   //配置GPIO33 为SCLA
    I2caRegs.I2CMDR.all = 0x0000;   //复位IIC

    // 预分频——时钟模块的频率
    I2caRegs.I2CPSC.all = 14;       //预分频   IIC模块时钟需设置为7-12MHz，本实验设置为 (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 50;   //时钟低电平时间值
    I2caRegs.I2CCLKH = 50;   //时钟高电平时间值
    I2caRegs.I2CIER.all = 0x00;      //Enable SCD & ARDY interrupts
    I2caRegs.I2CFFTX.all = 0x6000;   //启用，设置深度为16(0x6 << 12)
    I2caRegs.I2CFFRX.all = 0x6000;
    I2caRegs.I2CMDR.bit.IRS = 1;
    I2caRegs.I2CMDR.bit.MST = 1;
    I2caRegs.I2CMDR.bit.TRX = 1;
    EDIS;

    DELAY_US(200000);
    WriteData(0xAE,OLED_CMD);
    WriteData(0x00,OLED_CMD);
    WriteData(0x10,OLED_CMD);
    WriteData(0x40,OLED_CMD);
    WriteData(0xB0,OLED_CMD);
    WriteData(0x81,OLED_CMD);
    WriteData(0xFF,OLED_CMD);
    WriteData(0xA1,OLED_CMD);
    WriteData(0xA6,OLED_CMD);
    WriteData(0xA8,OLED_CMD);
    WriteData(0x3F,OLED_CMD);
    WriteData(0xC8,OLED_CMD);
    WriteData(0xD3,OLED_CMD);
    WriteData(0x00,OLED_CMD);
    WriteData(0xD5,OLED_CMD);
    WriteData(0x80,OLED_CMD);
    WriteData(0xD8,OLED_CMD);
    WriteData(0x05,OLED_CMD);
    WriteData(0xD9,OLED_CMD);
    WriteData(0xF1,OLED_CMD);
    WriteData(0xDA,OLED_CMD);
    WriteData(0x12,OLED_CMD);
    WriteData(0xDB,OLED_CMD);
    WriteData(0x30,OLED_CMD);
    WriteData(0x8D,OLED_CMD);
    WriteData(0x14,OLED_CMD);
    WriteData(0xAF,OLED_CMD);
}

/**
 * @brief 
 * @param mode 写入数据还是命令
 * @param Wdata 写入数据的地址
 */
void WriteData(Uint8 Wdata, Uint8 mode)
{
    while(!I2caRegs.I2CSTR.bit.XRDY);      //等待数据发送就绪，XRDY=1,表明发送寄存器已经准备好接受新的数据
    I2caRegs.I2CSAR = OLED_ADDR;    //设备从地址
    I2caRegs.I2CCNT = 2;   //需要发送的字节数
    //第一个发送字节为发送数据的目标寄存器地址
    if(mode)
        {I2caRegs.I2CDXR = 0x40&0xff;}    //写数据
    else
        {I2caRegs.I2CDXR = 0x00&0xff;}    //写命令

    I2caRegs.I2CMDR.all = 0x6E20;    //发送起始信号，内部数据计数器减到0时，发送停止信号，主机发送模式，使能IIC模式，

    while(!I2caRegs.I2CSTR.bit.XRDY);   //等待数据发送就绪，发送下一个数据
    I2caRegs.I2CDXR = Wdata;
}

/**
 * @brief 指数显示
 * 
 * @param m 底数
 * @param n 指数
 * @return Uint32 结果
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
 * @brief 设置当前编辑像素坐标
 * 
 * @param x 像素坐标
 * @param y 像素坐标
 */
void OLED_Set_Pos(Uint8 x, Uint8 y)
{
    WriteData(0xb0+y,OLED_CMD);
    WriteData(((x&0xf0)>>4)|0x10,OLED_CMD);
    WriteData((x&0x0f),OLED_CMD);
}

/**
 * @brief 开启显示
 * 
 */
void OLED_DisPlay_On(void)
{
    WriteData(0x8D,OLED_CMD);//电荷泵使能
    WriteData(0x14,OLED_CMD);//开启电荷泵
    WriteData(0xAF,OLED_CMD);//开启屏幕
}

/**
 * @brief 关闭显示
 * 
 */
void OLED_DisPlay_Off(void)
{
    WriteData(0x8D,OLED_CMD);//电荷泵使能
    WriteData(0x10,OLED_CMD);//关闭电荷泵
    WriteData(0xAE,OLED_CMD);//关闭屏幕
}

/**
 * @brief 清屏函数
 * 
 */
void OLED_Clear(void)
{
    Uint8 i,n;
    for(i=0;i<8;i++)
    {
        WriteData(0xb0+i,OLED_CMD);
        WriteData(0x00,OLED_CMD);
        WriteData(0x10,OLED_CMD);
        for(n=0;n<128;n++)
        {
            WriteData(0,OLED_DATA);
        }
    }
}

/**
 * @brief 在指定位置显示一个字符，包括部分字符
 * 
 * @param x 坐标，0~127
 * @param y 坐标 0~63
 * @param chr 显示字符 
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
        WriteData(F8x16[c*16+i],OLED_DATA);
    }
    OLED_Set_Pos(x,y+1);
    for(i=0;i<8;i++)
    {
        WriteData(F8x16[c*16+i+8],OLED_DATA);
    }
}

/**
 * @brief 显示字符串
 * 
 * @param x 起点坐标
 * @param y 起点坐标
 * @param chr* 字符串起始地址
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
 * @brief 
 * 
 * @param x 起点坐标
 * @param y 起点坐标
 * @param num 要显示的数字
 * @param len 数字的位数
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
                OLED_ShowChar(x+(8)*t,y,'0');
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
 * @brief 
 * 
 * @param x 起点坐标
 * @param y 起点坐标
 * @param num 显示数字
 * @param len 数字长度
 */
void OLED_ShowSignedNum(Uint8 x,Uint8 y,int16 num,Uint8 len)
{
    if(num >= 0)
    {
        OLED_ShowChar(x,y,' ');
        OLED_ShowNum(x+8,y,(Uint32)num,len);
    }
    else
    {
        num = -num;
        OLED_ShowChar(x,y,'-');
        OLED_ShowNum(x+8,y,(Uint32)num,len);
    }
}

/**
 * @brief 
 * 
 * @param x 起点坐标
 * @param y 起点坐标
 * @param num 要显示的数字
 * @param len 数字的长度，不包括小数点和负号
 * @param Flen 小数的位数
 */
void OLED_ShowFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen)
{
    float32 num1;
    Uint8 num2, num3;
    num1 = num;
    num2 = (Uint8)num1;
    num3 = (num - num2) * OLED_Pow(10, Flen);

    OLED_ShowNum(x,y,num2,len-Flen);
    OLED_ShowChar(x+8*(len-Flen),y,'.');

    OLED_ShowNum(x+8*(len-Flen+1),y,num3,Flen);

}

/**
 * @brief 显示有符号浮点数
 * 
 * @param x 起点坐标
 * @param y 起点坐标
 * @param num 要显示的数字
 * @param len 数字的长度，不包括小数点和负号
 * @param Flen 小数的位数
 */
void OLED_ShowSignedFloat(Uint8 x,Uint8 y,float32 num,Uint8 len,Uint8 Flen)
{
    if(num >= 0)
    {
        OLED_ShowChar(x,y,' ');
        OLED_ShowFloat(x+8,y,num,len,Flen);
    }
    else
    {
        num = -num;
        OLED_ShowChar(x,y,'-');
        OLED_ShowFloat(x+8,y,num,len,Flen);
    }
}
