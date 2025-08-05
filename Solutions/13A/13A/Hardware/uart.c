#include "global.h"
#include "uart.h"

UART_DATA_1Ch cmd_ch1;
UART_DATA_2Ch cmd_ch2;

/**
 * @brief Init UART-SCIc
 * 
 * @param baud 
 */
void UART_Init()
{
    InitSci();
    InitSciGpio();

    EALLOW;

    // out of reset
    ScicRegs.SCICTL1.bit.SWRESET = 1;

    //set data format
    ScicRegs.SCICCR.bit.STOPBITS = 0;
    ScicRegs.SCICCR.bit.PARITYENA = 0;
    ScicRegs.SCICCR.bit.SCICHAR = 7;

    //enable data transfer
    ScicRegs.SCICTL1.bit.RXENA = 1;
    ScicRegs.SCICTL1.bit.TXENA = 1;

    //Set baud rate
    Uint16 BRR = 0;
    BRR = 37500000 / (115200 * 8) -1 ;
    ScicRegs.SCIHBAUD = BRR & 0xFF00;
    ScicRegs.SCILBAUD = BRR & 0x00FF;

    //fifo reset
    ScicRegs.SCIFFTX.bit.SCIRST = 0;
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 0;

    //out of reset and config fifo
    ScicRegs.SCIFFTX.bit.SCIRST = 1;
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    ScicRegs.SCIFFTX.bit.SCIFFENA = 1;
    ScicRegs.SCIFFTX.bit.TXFFIL = 0x1F;
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
    SciaRegs.SCIFFRX.bit.RXFFIL = 0x1F;

    // Free run mode for debug
    ScicRegs.SCIPRI.bit.FREE = 2;

    EDIS;
}

void UART_Set(Uint32 baud)
{
    EALLOW;

    //Set baud rate
    Uint16 BRR = 0;
    BRR = 37500000.0f / (baud * 8) -1 ;
    ScicRegs.SCIHBAUD = BRR & 0xFF00;
    ScicRegs.SCILBAUD = BRR & 0x00FF;

    EDIS;
}


/**
 * @brief Send Char
 * 
 * @param send data to be sent
 */
void UART_SendChar(Uint8 send)
{
    ScicRegs.SCITXBUF = send;
    //waiting for TXBUFF empty
    while(ScicRegs.SCICTL2.bit.TXRDY == 0);
}

/**
 * @brief receive data from UART
 * 
 * @param data pointer to data placed
 */
void UART_ReceiveChar(Uint8 *data)
{
    // waiting for RXBUFF full
    while(ScicRegs.SCIRXST.bit.RXRDY !=1);
    *data = ScicRegs.SCIRXBUF.bit.RXDT;
}

/**
 * @brief send data to txfifo
 * 
 * @param data      data pointer 
 * @param length    length of transmit data
 */
void UART_SendString(Uint8 *data, Uint8 length)
{
    Uint8 i = 0;
    for(i = 0;i < length;i++)
    {
        UART_SendChar(*(data + i));
    }
}

/**
 * @brief get data from rxfifo
 * 
 * @param data      data pointer to where data are placed
 * @param length    receive data length
 */
void UART_ReceiveString(Uint8 *data, Uint8 length)
{
    Uint8 i = 0;
    //choose smaller one between length and RXFIFO status
    if(length >= ScicRegs.SCIFFRX.bit.RXFFST)
    {
        for(i = 0; i < ScicRegs.SCIFFRX.bit.RXFFST; i++)
        {
            UART_ReceiveChar(data + i);
        }
    }
    else
    {
        for(i = 0; i < length; i++)
        {
            UART_ReceiveChar(data + i);
        }
    }
}

/**
 * @brief 实现浮点数转4个8位数据，与VOFA实现通信
 * 
 * @param data 发送数据
 */
void UART_VOFA_1Ch_ShowFloat(float data)
{
    cmd_ch1.data.data = data;
    cmd_ch1.send_float[0] = cmd_ch1.data.dat[0];
    cmd_ch1.send_float[1] = (cmd_ch1.data.dat[0]>>8);
    cmd_ch1.send_float[2] = cmd_ch1.data.dat[1];
    cmd_ch1.send_float[3] = (cmd_ch1.data.dat[1]>>8);
    cmd_ch1.send_float[4] = 0x00;
    cmd_ch1.send_float[5] = 0x00;
    cmd_ch1.send_float[6] = 0x80;
    cmd_ch1.send_float[7] = 0x7f;

    UART_SendString(cmd_ch1.send_float,8);

}

void UART_VOFA_2Ch_ShowFloat(float data1,float data2)
{
    cmd_ch2.data1.data = data1;
    cmd_ch2.data2.data = data2;

    cmd_ch2.send_float[0] = cmd_ch2.data1.dat[0];
    cmd_ch2.send_float[1] = (cmd_ch2.data1.dat[0]>>8);
    cmd_ch2.send_float[2] = cmd_ch2.data1.dat[1];
    cmd_ch2.send_float[3] = (cmd_ch2.data1.dat[1]>>8);

    cmd_ch2.send_float[4] = cmd_ch2.data2.dat[0];
    cmd_ch2.send_float[5] = (cmd_ch2.data2.dat[0]>>8);
    cmd_ch2.send_float[6] = cmd_ch2.data2.dat[1];
    cmd_ch2.send_float[7] = (cmd_ch2.data2.dat[1]>>8);

    cmd_ch2.send_float[8] = 0x00;
    cmd_ch2.send_float[9] = 0x00;
    cmd_ch2.send_float[10] = 0x80;
    cmd_ch2.send_float[11] = 0x7f;

    UART_SendString(cmd_ch2.send_float,12);

}
