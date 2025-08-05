#include "Global.h"




//工程内部全局变量定义=============================================================================================

//Sample-----------------------------------------------------------------------------------------------------
//采样值存储
Uint16 Input_Ptr;
Uint8 Flag_RFFT;
Uint32 Flag_OLED_Refresh;
Uint8 N_zashu;

//power_meas_sine_analyzer-----------------------------------------------------------------------------------

POWER_MEAS_SINE_ANALYZER  Power_Measure_Global;
POWER_MEAS_SINE_ANALYZER* Power_Measure_Global_Addr = &Power_Measure_Global;

//FFT--------------------------------------------------------------------------------------------------------

float FFT_input[FFT_SIZE];
float FFT_twiddleFactors[FFT_SIZE];
float FFT_output[FFT_SIZE];
float FFT_magnitude_phase[(FFT_SIZE >> 1) + 1];
float Current_THD;

RFFT_F32_STRUCT rfft;
RFFT_F32_STRUCT_Handle hnd_rfft = &rfft;
RFFT_Analyse_Struct rfft_analyse;

float myabs;

#pragma DATA_SECTION(FFT_input, "FFT_buffer1")
#pragma DATA_SECTION(FFT_output, "FFT_buffer2")
#pragma DATA_SECTION(FFT_magnitude_phase, "FFT_buffer3")
#pragma DATA_SECTION(FFT_twiddleFactors, "FFT_buffer4")

//------------------------------------------------------------------------------------------------------------

/*
 * End_of_File
 */


