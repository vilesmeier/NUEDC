#include "global.h"

/**
 * @brief Driver code for ADC/DAC
 * ADC:ADC128S102
 * DAC:DAC124S085
 * MUX:TS3A27518
 * Pherial: SPIa GPIO26(ENABLE) GPIO27(SEL)
 */

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

    return data;
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
    SPI_SwitchByte(data);
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
}
