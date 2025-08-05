#include "global.h"

/**
 * @brief Init SPIa for 28335,enable FIFO,disable interrupt
 * 
 */
void SPI_Init(void)
{
    InitSpiaGpio();

    EALLOW;
    //16 bit in one frame
    SpiaRegs.SPICCR.bit.SPICHAR = 0x000f;   
    //output on falling edge and input on rising edge.
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;
    //loopback mode disabled
    SpiaRegs.SPICCR.bit.SPILBK = 0;

    SpiaRegs.SPICTL.all = 0x0006;
    SpiaRegs.SPIBRR =0x000E;

    //fifo init
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;

    // Relinquish SPI from Reset
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;   
    // Set so breakpoints don't disturb xmission
    SpiaRegs.SPIPRI.bit.FREE = 1; 

    EDIS;
}

/**
 * @brief switch data byte of 16 bit unsigned int
 * 
 * @param data      data sent by 28335
 * @return Uint16   data receive by 28335
 */
Uint16 SPI_SwitchByte(Uint16 data)
{
    EALLOW;
    SpiaRegs.SPITXBUF = data;
    while(SpiaRegs.SPIFFTX.bit.TXFFST != 0);
    while(SpiaRegs.SPIFFRX.bit.RXFFST == 0);
    data = SpiaRegs.SPIRXBUF;
    EDIS;
    return(data);
}

/**
 * @brief software reset for SPIa
 * 
 */
void SPI_Reset(void)
{
    EALLOW;
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    DELAY_US(10);
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;
    EDIS;
}
