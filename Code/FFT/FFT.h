#ifndef _FFT_H_
#define _FFT_H_

#include "fpu_rfft.h"
#include "C28x_FPU_FastRTS.h"


#define FFT_SIZE             (1<<FFT_STAGES)
#define FFT_STAGES           10U
#define Fs                   2048U


typedef struct RFFT_Analyser
{
    float                   DC_Amp;
    float                   Signal_THD;
    float                   Amp[FFT_STAGES];
    float                   input[FFT_SIZE];
    float                   output[FFT_SIZE];
    float                   twiddleFactors[FFT_SIZE];
    float                   magnitude_phase[(FFT_SIZE >> 1) + 1];
    RFFT_F32_STRUCT         rfft;
    RFFT_F32_STRUCT_Handle  hnd_rfft;
} RFFT_Analyser;



void RFFT_Cal(RFFT_Analyser* rfft);
void RFFT_Analyse(RFFT_Analyser* rfft);
void THD_Calculate(RFFT_Analyser* rfft);
void RFFT_Init(RFFT_Analyser* rfft,uint16_t stage);


extern RFFT_Analyser rfft_analyse;


#endif // !_FFT_H_
