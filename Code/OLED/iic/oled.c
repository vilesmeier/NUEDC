#include "oled_font.h"
#include <oled_i2c.h>
#include "global.h"

/**
 *  @brief              IICģ�鷢��׼��OK
 *  @parameter              ��
 *  @return_value           ��
 */
Uint16  I2C_xrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.XRDY;   //IICģ�鷢��׼��OK
    return t;
}

/**
 *  @brief              IICģ�����׼��OK
 *  @parameter              ��
 *  @return_value           ��
 */
Uint16  I2C_rrdy()
{
    Uint16  t;
    t = I2caRegs.I2CSTR.bit.RRDY;   //IICģ�����׼��OK
    return t;
}

/**
 *  @brief                  IICд����
 *  @parameter              SlaveAddress���ӻ���ַ��  RomAddress���Ĵ�����ַ�� number��д�����ݵ��ֽ�����  *Wdata��д�����ݵĵ�ַ
 *  @return_value           ״̬��־
 */
Uint16 WriteData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint8 *Wdata)
{

    Uint16 i;
    while(!I2C_xrdy());      //�ȴ����ݷ��;�����XRDY=1,�������ͼĴ����Ѿ�׼���ý����µ�����
    DELAY_US(10);

    I2caRegs.I2CSAR = SlaveAddress&0xff;    //�豸�ӵ�ַ
    I2caRegs.I2CCNT = number + 1;   //��Ҫ���͵��ֽ���
    I2caRegs.I2CDXR = RomAddress&0xff;    //��һ�������ֽ�Ϊ�������ݵ�Ŀ��Ĵ�����ַ
    DELAY_US(1);//�ȴ�������ȫ��ֵ��I2CDXR����ʹ��IIC������Ҫ���ϣ�
    I2caRegs.I2CMDR.all = 0x6E20;    //������ʼ�źţ��ڲ����ݼ���������0ʱ������ֹͣ�źţ���������ģʽ��ʹ��IICģʽ��
    for (i=0; i<number; i++)
    {
        while(!I2C_xrdy());   //�ȴ����ݷ��;�����������һ������
        DELAY_US(10);

        I2caRegs.I2CDXR = *Wdata&0xFF;
        Wdata++;
    }
    DELAY_US(1);
    return I2C_SUCCESS;         //���ͳɹ�
}


/**
 *  @brief                  IICд����
 *  @parameter              SlaveAddress���ӻ���ַ��  RomAddress���Ĵ�����ַ�� number��д�����ݵ��ֽ�����  *RamAddr���������ݴ�ŵĵ�ַ
 *  @return_value           ״̬��־
 */
Uint16 ReadData( Uint16 SlaveAddress, Uint16 RomAddress,Uint16 number, Uint16  *RamAddr)
{
    Uint16  i,Temp;

    if (I2caRegs.I2CSTR.bit.BB == 1)  //��������æ����״̬
    {
        return I2C_BUS_BUSY_ERROR;
    }

    while(!I2C_xrdy());
    DELAY_US(10);

    I2caRegs.I2CSAR = SlaveAddress&0xff;      //�豸�ӵ�ַ
    I2caRegs.I2CCNT = 1;        //����һ���ֽ�ΪҪ��ȡ���ݵļĴ�����ַ
    I2caRegs.I2CDXR = RomAddress&0xff;
    DELAY_US(1);//�ȴ�������ȫ��ֵ��I2CDXR����ʹ��IIC������Ҫ���ϣ�
   I2caRegs.I2CMDR.all = 0x6620;   //��������ģʽ���Զ����ֹͣλ������ֹͣ�ź�

   if (I2caRegs.I2CSTR.bit.NACK == 1)    //�͵���Ӧ���źŷ��ش���
        return  I2C_BUS_BUSY_ERROR;

    DELAY_US(50);

   while(!I2C_xrdy());                  //���Թ�������ʱ�Ῠ����һ��,��ʾͨѶ������
    DELAY_US(10);

    I2caRegs.I2CSAR = SlaveAddress&0xff;
    I2caRegs.I2CCNT = number;
    DELAY_US(1);//�ȴ�������ȫ��ֵ��I2CDXR����ʹ��IIC������Ҫ���ϣ�
    I2caRegs.I2CMDR.all = 0x6C20;   //��������ģʽ��������ֹͣ�ź�

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
   return I2C_SUCCESS;    //�������ݳɹ�
}

/**
 *  @brief                 IIC��Ĵ�����ĳһλд����
 *  @parameter              SlaveAddress���ӻ���ַ��  regaddress���Ĵ�����ַ�� bitNum��ĳһλ��  data��д������
 *  @return_value           ��
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
 *  @brief                 IIC��Ĵ�����ĳ��λд����
 *  @parameter              SlaveAddress���ӻ���ַ��  regaddress���Ĵ�����ַ�� bitStart����ʼλ��  length�����ȣ�data��д������
 *  @return_value           ��
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
 *  @brief                   д��һ���ֽ�
 *  @parameter                  dat��д������� �� rom��׼��д�����ݵļĴ�����ַ
 *  @return_value               ��
 */
void Send_Byte(Uint8 dat,Uint16 rom)
{
    while(WriteData(devAddr,rom,1,&dat));//�ȴ����ͳɹ�
}

/**
 *  @brief                   ��OLED����һ���ֽ�
 *  @parameter                  dat�� д������ݣ� mode������/���� 0��ʾ���1��ʾ����
 *  @return_value               ��
 */
void OLED_WR_Byte(Uint8 dat,Uint8 mode)
{
    DELAY_US(10);
    if(mode)
        Send_Byte(dat,0x40);//д����
    else
        Send_Byte(dat,0x00);//д����
    DELAY_US(10);
}

void OLED_Set_Pos(Uint8 x, Uint8 y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

/**
 *  @brief                   ������ʾ
 *  @parameter                 ��
 *  @return_value               ��
 */
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x14,OLED_CMD);//������ɱ�
    OLED_WR_Byte(0xAF,OLED_CMD);//������Ļ
}

/**
 *  @brief                   �ر���ʾ
 *  @parameter                 ��
 *  @return_value               ��
 */
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D,OLED_CMD);//��ɱ�ʹ��
    OLED_WR_Byte(0x10,OLED_CMD);//�رյ�ɱ�
    OLED_WR_Byte(0xAE,OLED_CMD);//�ر���Ļ
}

/**
 *  @brief                   ��������
 *  @parameter                 ��
 *  @return_value               ��
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
 *  @brief                   ��ָ��λ����ʾһ���ַ������������ַ�
 *  @parameter              x:0~127�� y:0~63��
 *  @return_value           ��
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
 *  @brief                ��ʾ�ַ���
 *  @parameter            x,y:������ꣻ  size1:ѡ�������С �� *chr:�ַ�����ʼ��ַ��   mode:0,��ɫ��ʾ,1,������ʾ
 *  @return_value           ��
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
 *  @brief                ָ����ʾ
 *  @parameter            m:������ n:ָ��
 *  @return_value           ���
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
 *  @brief                ��ʾ����
 *  @parameter            x,y:������ꣻ  num��Ҫ��ʾ�����֣� len�����ֵ�λ����  size1:ѡ�������С��   mode:0,��ɫ��ʾ,1,������ʾ
 *  @return_value           ��
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
 *  @brief               OLED��ʼ��
 *  @parameter            ��
 *  @return_value           ��
 */
void OLED_Init(void)
{
    I2caRegs.I2CMDR.all = 0x0000;   //��λIIC

    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    //ʹ��(SDAA)����
    GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;    //ʹ�� (SCLA)����
    GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  //ͬ�� (SDAA)
    GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  //ͬ�� (SCLA)
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   //���� GPIO32Ϊ SDAA
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   //����GPIO33 ΪSCLA

    // Ԥ��Ƶ����ʱ��ģ���Ƶ��
    I2caRegs.I2CPSC.all = 14;       //Ԥ��Ƶ   IICģ��ʱ��������Ϊ7-12MHz����ʵ������Ϊ (150/15 = 10MHz)
    I2caRegs.I2CCLKL = 500;   //ʱ�ӵ͵�ƽʱ��ֵ
    I2caRegs.I2CCLKH = 500;   //ʱ�Ӹߵ�ƽʱ��ֵ
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
