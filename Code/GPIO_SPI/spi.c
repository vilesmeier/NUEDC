#include "SPI.h"
#include "DSP28x_Project.h"

/* 
    GPIOģ��SPIͨ��
    encoding��GBK

    ���ƶ�Ӧ��
    DI ���� MOSI
    DO ���� MISO
    
    �Ĵ���˵����
    CLK_PHASE       0�������Ӻ�(��һ���ز���)     1�����Ӻ�������(�ڶ����ز���)
    CLKPOLARITY     0�������е͵�ƽ   1�������иߵ�ƽ

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



//��ʼ��
void SPI_GPIO_Init(void)
{
    //gpioģ��
    //D0Ϊ���룬����Ϊ���

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

//CS����
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

//SCK����
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

//MOSIģ��
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

//MISOģ��
unsigned char SPI_R_DO(void)
{
    return SPI_DO ;
}

//���俪ʼ
void SPI_Start(void)
{
    SPI_W_CS(SPI_RESET);
    DELAY_US(SPI_DELAY_TIME);
}

//�������
void SPI_Stop(void)
{
    DELAY_US(SPI_DELAY_TIME);
    SPI_W_CS(SPI_SET);
}

//8λ���������ı�ByteSend������ֵ
#if _SPI_TRANSFER_MODE_ == 0
//����ʱ�����е͵�ƽ
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char i;
    unsigned char ByteReseive = 0x00;
    for(i=0;i < 8;i++)
    {
        //������
        SPI_W_SCK(1);
        //�Ƴ�����
        SPI_W_DI(ByteSend & (0x80 >> i));
        DELAY_US(SPI_DELAY_TIME);
        //�½���
        SPI_W_SCK(0);
        DELAY_US(SPI_DELAY_TIME);
        //��������
        if(SPI_R_DO() == 1)
        {
            ByteReseive |= (0x80 >> i);
        }
        DELAY_US(SPI_DELAY_TIME);
    }
    return ByteReseive;
}

#elif _SPI_TRANSFER_MODE_ == 1
//����ʱ�����иߵ�ƽ
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char i;
    unsigned char ByteReseive = 0x00;
    for(i=0;i < 8;i++)
    {
        //�½���
        SPI_W_SCK(0);
        //�Ƴ�����
        SPI_W_DI(ByteSend & (0x80 >> i));
        DELAY_US(SPI_DELAY_TIME);
        //������
        SPI_W_SCK(1);
        DELAY_US(SPI_DELAY_TIME);
        //��������
        if(SPI_R_DO() == 1)
        {
            ByteReseive |= (0x80 >> i);
        }
        DELAY_US(SPI_DELAY_TIME);
    }
    return ByteReseive;
}

#elif _SPI_TRANSFER_MODE_ == 2
//����ʱ,���е͵�ƽ
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char receivedData = 0x00;
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        // ��������λ
        SPI_W_DI(ByteSend & (0x80 >> i));
        // ����ʱ��������
        SPI_W_SCK(1);
        DELAY_US(SPI_DELAY_TIME);
        // ��ȡMISO�ϵ�����
        if(SPI_R_DO() == 1)
        {
            receivedData |= (0x80 >> i);
        }
        // ����ʱ���½���
        SPI_W_SCK(0);  // SCLK = 0
        DELAY_US(SPI_DELAY_TIME);
        // ׼��������һλ
    }
    return receivedData;
}

#elif _SPI_TRANSFER_MODE_ == 3
//����ʱ,���иߵ�ƽ
unsigned char SPI_SwapByte(unsigned char ByteSend)
{
    unsigned char receivedData = 0x00;
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        // ��������λ
        SPI_W_DI(ByteSend & (0x80 >> i));
        // ����ʱ���½���
        SPI_W_SCK(0);
        DELAY_US(SPI_DELAY_TIME);
        // ��ȡMISO�ϵ�����
        if(SPI_R_DO() == 1)
        {
            receivedData |= (0x80 >> i);
        }
        // ����ʱ��������
        SPI_W_SCK(1);  // SCLK = 0
        DELAY_US(SPI_DELAY_TIME);
        // ׼��������һλ
    }
    return receivedData;
}

#endif

//����2byte,16λ
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
