#ifndef _Global_H_
#define _Global_H_

//================================================================================================
#include <Device/DSP28x_Project.h>
#include <Library/c28x_fpu_fastrts.h>
#include <Library/power_meas_sine_analyzer.h>
#include <Library/fpu_rfft.h>
#include <Library/dsp.h>
#include <Library/fpu.h>
#include <complex.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "LCD_ST7735S.h"


//�궨��============================================================================================

//Sample------------------------------------------------------------------------------------------
//ƫ��
#define Current_Bias    1.244
#define Voltage_Bias    2.500
//����
#define Current_Multiply 2.335483233
#define Voltage_Multiply 622.2539674

//���ҹ��ʷ�����
#define INV_220     0.00321412173
#define INV_5       0.1414213562

//����ʱ��
#define Sample_Time 0.00048828125
//�����ź�Ƶ��
#define Grid_Freq 50
#define pi 3.1415926535

//PWM--------------------------------------------------------------------------------------------------
#define System_Freq 150000000

//FFT--------------------------------------------------------------------------------------------------
#define FFT_SIZE             1024
#define FFT_STAGES           10
#define Fs                   2048    //����Ƶ��
#define RFFT_Analyse_MaxNum  10
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))


//�ṹ�嶨��==============================================================================================
//Data_Type--------------------------------------------------------------------------------------------

typedef char            int8;
typedef unsigned char   Uint8;

//RFFT-------------------------------------------------------------------------------------------------
typedef struct RFFT_Analyse_Struct
{
    Uint16 Wave_Num;
    float DC_Amp;
    float Amp[RFFT_Analyse_MaxNum];
} RFFT_Analyse_Struct;

//����ʵ��=================================================================================================

//PWM--------------------------------------------------------------------------------------------------

//PWM��ʼ��
void EPWM1_Init(Uint16 tbprd);

//PWM�жϷ�����
interrupt void epwm1_timer_isr(void);

//Sample-------------------------------------------------------------------------------------------------

extern void ADC_Cal(void);

//ȫ�ֱ�����ʼ��
void ADC_Global_Variable_Init(void);

//ADC��ʼ��
void ADC_Init(void);

//ģ���ʼ��
void Sample_Init(void);

void Power_Measure_Init(void);

//ADCת��ֵ����
void ADC_Get_Value(void);

//ADC�жϷ�����
interrupt void ADC_int_isr(void);


//XINT_GPIO---------------------------------------------------------------------------------------------
//�ж�����
void XINT_Init(void);

//�жϷ�����
interrupt void xint1_isr(void);
interrupt void xint2_isr(void);
interrupt void xint3_isr(void);
interrupt void xint4_isr(void);
interrupt void xint5_isr(void);
interrupt void xint6_isr(void);
interrupt void xint7_isr(void);

//FFT--------------------------------------------------------------------------------------------------

void Current_THD_Calculate(void);

//FFT�ṹ���ʼ��
void RFFT_ADC_Init(void);

void RFFT_ADC_Cal(void);

void RFFT_Analyse(void);


//Clock----------------------------------------------------------------------------------------------------
void System_Clock_Close(void);



//�����ڲ�ȫ�ֱ�������============================================================================================
//Sample----------------------------------------------------------------------------------------------------
//����ֵ�洢
extern Uint16 Input_Ptr;
extern Uint8 Flag_RFFT;
extern Uint8 N_zashu;
extern Uint32 Flag_OLED_Refresh;

//power_meas_sine_analyzer-----------------------------------------------------------------------------------

extern POWER_MEAS_SINE_ANALYZER  Power_Measure_Global;
extern POWER_MEAS_SINE_ANALYZER* Power_Measure_Global_Addr;


//FFT--------------------------------------------------------------------------------------------------------
extern float FFT_input[FFT_SIZE];
extern float FFT_twiddleFactors[FFT_SIZE];
extern float FFT_output[FFT_SIZE];
extern float FFT_magnitude_phase[(FFT_SIZE >> 1) + 1];
extern float Current_THD;

extern RFFT_F32_STRUCT rfft;
extern RFFT_F32_STRUCT_Handle hnd_rfft;
extern RFFT_Analyse_Struct rfft_analyse;

extern float myabs;

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);


#endif /* _Global_H_ */

/*
 * End_of_File
 */

