#include "global.h"
#include "uart.h"



UART_DATA cmd;

/**
 * @brief Init UART-SCIc
 * 
 * @param baud 
 */
void UART_Init(Uint32 baud)
{
    InitSci();
    InitSciGpio();

    EALLOW;

    //set LSPCLK to 150MHz
    SysCtrlRegs.LOSPCP.bit.LSPCLK = 2;

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
    BRR = 37500000.0f / (baud * 8) -1 ;
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
 * @brief VOFA support , sent float in string format
 * 
 * @param data      data to be sent
 * @param Length    total length
 * @param precision decimal length
 */
void UART_VOFA_ShowFloat(float data)
{
    cmd.data.data = data;
    cmd.send_float[0] = cmd.data.dat[0];
    cmd.send_float[1] = (cmd.data.dat[0]>>8);
    cmd.send_float[2] = cmd.data.dat[1];
    cmd.send_float[3] = (cmd.data.dat[1]>>8);
    cmd.send_float[4] = 0x00;
    cmd.send_float[5] = 0x00;
    cmd.send_float[6] = 0x80;
    cmd.send_float[7] = 0x7f;

    UART_SendString(cmd.send_float,8);

}
