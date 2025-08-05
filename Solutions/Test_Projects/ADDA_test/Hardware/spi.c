#include "global.h"

void spi_init(void)
{
    InitSpiaGpio();

    SpiaRegs.SPICCR.bit.SPICHAR = 0x0f;//16Î»Êý¾Ý
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;//output on rising edge and input on falling edge.
    SpiaRegs.SPICCR.bit.SPILBK = 0;//loopback mode disabled
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;//reset on

    SpiaRegs.SPICTL.all = 0x0006;
    SpiaRegs.SPIBRR =0x007F;
    SpiaRegs.SPICCR.all =0x009F;   // Relinquish SPI from Reset
    SpiaRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission

    //fifo init
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;
}

void spi_send(Uint16 send)
{
    SpiaRegs.SPITXBUF = send;
}

void spi_receive(Uint16 receive)
{
    while(SpiaRegs.SPIFFRX.bit.RXFFST !=1){}// Wait until data is received

    receive = SpiaRegs.SPIRXBUF;
}




