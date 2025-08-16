#ifndef ADDA_H
#define ADDA_H

// Select definition
#define DEVICE_ENABLE   0
#define DEVICE_DISABLE  1
#define SELECT_DAC      0
#define SELECT_ADC      1

//ADC definitions
//convert channel select
#define CHANNEL_0 0
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3
#define CHANNEL_4 4
#define CHANNEL_5 5
#define CHANNEL_6 6
#define CHANNEL_7 7

//DAC definitions
//channel select
#define DAC1 0
#define DAC2 1
#define DAC3 2
#define DAC4 3

//update mode
#define UPDATE_END           0
#define UPDATE_IMMEDIATE     1
#define UPDATE_IMMEDIATE_ALL 2
#define POWER_DOWN           3

//power down inpendent select
#define HIGH_Z      0
#define LOW_Z       1
#define MIDIUM_Z    2

//Hardware driver for SPIa
extern void SPI_Init(void);
extern Uint16 SPI_SwitchByte(Uint16 data);
extern void SPI_Reset(void);

//device control
void ADDA_Init(void);
void ADDA_Enable(void);
void ADDA_Disable(void);
void ADDA_Select(Uint8 device);

//Device driver for ADC
Uint16 ADC128S102_Convert(Uint16 channle);

//Device driver for DAC
void DAC124S085_PowerDown(Uint8 mode);
void DAC124S085_Output_Update(Uint8 dac, Uint8 mode, Uint16 value);

#endif // !ADDA_H
