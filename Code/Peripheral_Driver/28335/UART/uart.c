#include "global.h"
#include "uart.h"

/**
 * @brief Init SCI-c as debug port,connect to VOFA
 * 
 */
void UART_Init()
{
    InitSci();
    InitSciGpio();

    EALLOW;

    //set LSPCLK to 150MHz
    SysCtrlRegs.LOSPCP.bit.LSPCLK = 0;

    // out of reset
    ScicRegs.SCICTL1.bit.SWRESET = 1;

    //set data format
    ScicRegs.SCICCR.bit.STOPBITS = 0;
    ScicRegs.SCICCR.bit.PARITYENA = 0;
    ScicRegs.SCICCR.bit.SCICHAR = 7;

    //enable data transfer
    ScicRegs.SCICTL1.bit.RXENA = 1;
    ScicRegs.SCICTL1.bit.TXENA = 1;

    // Free run mode for debug
    ScicRegs.SCIPRI.bit.FREE = 2;

    //Set baud rate
    Uint16 BRR = 0;
    BRR = 150000000.0f / (115200 * 8) -1 ;
    ScicRegs.SCIHBAUD = (BRR & 0xFF00) >> 8;
    ScicRegs.SCILBAUD = BRR & 0x00FF;

    //fifo reset
    ScicRegs.SCIFFTX.bit.SCIRST = 0;
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 0;

    //out of reset and config fifo
    ScicRegs.SCIFFTX.bit.SCIRST = 1;

    ScicRegs.SCIFFTX.bit.SCIFFENA = 1;
    ScicRegs.SCIFFTX.bit.TXFFIL = 0x1F;
    ScicRegs.SCIFFCT.bit.FFTXDLY = 0x0;
    ScicRegs.SCIFFRX.bit.RXFFIL = 0x1F;
    ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
    ScicRegs.SCIFFTX.bit.TXFIFOXRESET = 1;

    EDIS;
}

/**
 * @brief config baud rate
 * 
 * @param baud baud rate
 */
void UART_Set(Uint32 baud)
{
    EALLOW;

    //Set baud rate
    Uint16 BRR = 0;
    BRR = 150000000.0f / (baud * 8) -1 ;
    ScicRegs.SCIHBAUD = (BRR & 0xFF00) >> 8;
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
    while(ScicRegs.SCIFFTX.bit.TXFFST != 0);
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
