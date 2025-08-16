#include "oled_font.h"
#include "oled.h"
#include "global.h"

/**
  * @brief OLED��ʼ��
  * 
  */
void OLED_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    //ʹ��(SDAA)����
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //ʹ�� (SCLA)����
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  //ͬ�� (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  //ͬ�� (SCLA)
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   //���� GPIO32Ϊ SDAA
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   //����GPIO33 ΪSCLA
    I2caRegs.I2CMDR.all = 0x0000;   //��λIIC

    // Ԥ��Ƶ����ʱ��ģ���Ƶ��
    I2caRegs.I2CPSC.all = 14;       //Ԥ��Ƶ   IICģ��ʱ��������Ϊ7-12MHz����ʵ������Ϊ (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 50;   //ʱ�ӵ͵�ƽʱ��ֵ
    I2caRegs.I2CCLKH = 50;   //ʱ�Ӹߵ�ƽʱ��ֵ
    I2caRegs.I2CIER.all = 0x00;      //Enable SCD & ARDY interrupts
    I2caRegs.I2CFFTX.all = 0x6000;   //���ã��������Ϊ16(0x6 << 12)
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
 * @param mode д�����ݻ�������
 * @param Wdata д�����ݵĵ�ַ
 */
void WriteData(Uint8 Wdata, Uint8 mode)
{
    while(!I2caRegs.I2CSTR.bit.XRDY);      //�ȴ����ݷ��;�����XRDY=1,�������ͼĴ����Ѿ�׼���ý����µ�����
    I2caRegs.I2CSAR = OLED_ADDR;    //�豸�ӵ�ַ
    I2caRegs.I2CCNT = 2;   //��Ҫ���͵��ֽ���
    //��һ�������ֽ�Ϊ�������ݵ�Ŀ��Ĵ�����ַ
    if(mode)
        {I2caRegs.I2CDXR = 0x40&0xff;}    //д����
    else
        {I2caRegs.I2CDXR = 0x00&0xff;}    //д����

    I2caRegs.I2CMDR.all = 0x6E20;    //������ʼ�źţ��ڲ����ݼ���������0ʱ������ֹͣ�źţ���������ģʽ��ʹ��IICģʽ��

    while(!I2caRegs.I2CSTR.bit.XRDY);   //�ȴ����ݷ��;�����������һ������
    I2caRegs.I2CDXR = Wdata;
}

/**
 * @brief ָ����ʾ
 * 
 * @param m ����
 * @param n ָ��
 * @return Uint32 ���
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
 * @brief ���õ�ǰ�༭��������
 * 
 * @param x ��������
 * @param y ��������
 */
void OLED_Set_Pos(Uint8 x, Uint8 y)
{
    WriteData(0xb0+y,OLED_CMD);
    WriteData(((x&0xf0)>>4)|0x10,OLED_CMD);
    WriteData((x&0x0f),OLED_CMD);
}

/**
 * @brief ������ʾ
 * 
 */
void OLED_DisPlay_On(void)
{
    WriteData(0x8D,OLED_CMD);//��ɱ�ʹ��
    WriteData(0x14,OLED_CMD);//������ɱ�
    WriteData(0xAF,OLED_CMD);//������Ļ
}

/**
 * @brief �ر���ʾ
 * 
 */
void OLED_DisPlay_Off(void)
{
    WriteData(0x8D,OLED_CMD);//��ɱ�ʹ��
    WriteData(0x10,OLED_CMD);//�رյ�ɱ�
    WriteData(0xAE,OLED_CMD);//�ر���Ļ
}

/**
 * @brief ��������
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
 * @brief ��ָ��λ����ʾһ���ַ������������ַ�
 * 
 * @param x ���꣬0~127
 * @param y ���� 0~63
 * @param chr ��ʾ�ַ� 
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
 * @brief ��ʾ�ַ���
 * 
 * @param x �������
 * @param y �������
 * @param chr* �ַ�����ʼ��ַ
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
 * @param x �������
 * @param y �������
 * @param num Ҫ��ʾ������
 * @param len ���ֵ�λ��
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
 * @param x �������
 * @param y �������
 * @param num ��ʾ����
 * @param len ���ֳ���
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
 * @param x �������
 * @param y �������
 * @param num Ҫ��ʾ������
 * @param len ���ֵĳ��ȣ�������С����͸���
 * @param Flen С����λ��
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
 * @brief ��ʾ�з��Ÿ�����
 * 
 * @param x �������
 * @param y �������
 * @param num Ҫ��ʾ������
 * @param len ���ֵĳ��ȣ�������С����͸���
 * @param Flen С����λ��
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
