#include "global.h"

/**
 * @brief Driver code for ADC/DAC
 * ADC:ADC128S102
 * DAC:DAC124S085
 * MUX:TS3A27518
 * Pherial: SPIa GPIO26(ENABLE) GPIO27(SEL)
 */

/**
 * @brief Init SPIa for 28335,enable FIFO,disable interrupt
 * 
 */
void SPI_Init(void)
{
    InitSpiaGpio();

    EALLOW;
    SpiaRegs.SPICCR.bit.SPICHAR = 0x000f;//16Î»Êý¾Ý
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 1;//output on falling edge and input on rising edge.
    SpiaRegs.SPICCR.bit.SPILBK = 0;//loopback mode disabled

    SpiaRegs.SPICTL.all = 0x0006;
    SpiaRegs.SPIBRR =0x000E;

    //fifo init
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;

    SpiaRegs.SPICCR.bit.SPISWRESET = 1;   // Relinquish SPI from Reset
    SpiaRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission
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
    while(SpiaRegs.SPIFFTX.bit.TXFFST != 0){}
    while(SpiaRegs.SPIFFRX.bit.RXFFST == 0){}
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

/**
 * @brief init control GPIO
 * 
 */
void ADDA_Control_Init(void)
{
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;

    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;

    GpioDataRegs.GPADAT.bit.GPIO26 = 1;
    GpioDataRegs.GPADAT.bit.GPIO27 = 1;

    GpioCtrlRegs.GPADIR.bit.GPIO26 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;
    EDIS;
}

/**
 * @brief Init whole module
 * 
 */
void ADDA_Init(void)
{
    SPI_Init();
    ADDA_Control_Init();
    ADDA_Enable();
}


/**
 * @brief enable input MUX on adda board
 * 
 */
void ADDA_Enable(void)
{
    EALLOW;
    GpioDataRegs.GPADAT.bit.GPIO26 = DEVICE_ENABLE;
    EDIS;
}

/**
 * @brief disable input MUX on adda board
 * 
 */
void ADDA_Disable(void)
{
    EALLOW;
    GpioDataRegs.GPADAT.bit.GPIO26 = DEVICE_DISABLE;
    EDIS;
}

/**
 * @brief select device on ADDA board by input MUX on board
 * 
 * @param device    select ADC or ADC
 */
void ADDA_Select(Uint8 device)
{
    EALLOW;
    GpioDataRegs.GPADAT.bit.GPIO27 = device;
    EDIS;
}

/**
 * @brief start convert and select channel for next convert
 * 
 * @return Uint16 
 */
Uint16 ADC128S102_Convert(Uint16 channle)
{
    Uint16 data = 0;
    data = channle << 11;

    ADDA_Select(SELECT_ADC);

    EALLOW;
    SpiaRegs.SPITXBUF = data;
    while(SpiaRegs.SPIFFTX.bit.TXFFST != 0){}
    while(SpiaRegs.SPIFFRX.bit.RXFFST == 0){}
    data = SpiaRegs.SPIRXBUF;
    EDIS;

    return(data);
}   

/**
 * @brief Device power off
 * 
 * @param mode  select power off input independent
 */
void DAC124S085_PowerDown(Uint8 mode)
{
    ADDA_Select(SELECT_DAC);
    Uint16 data = 0;
    data = (((mode << 2) | 0x3) << 12);
    volatile Uint16 dummy1 = SPI_SwitchByte(data);
}

/**
 * @brief select dac and update output
 * 
 * @param dac   channel select
 * @param mode  update mode select
 * @param value output value
 */
void DAC124S085_Output_Update(Uint8 dac, Uint8 mode, Uint16 value)
{
    if(mode != POWER_DOWN)
    {
        value = ((mode | (dac << 2)) << 12) | value;
    }
    else
    {
        value = mode << 12;
    }

    ADDA_Select(SELECT_DAC);

    volatile Uint16 dummy2 = SPI_SwitchByte(value);

}
