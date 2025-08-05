#include "Global.h"

//总谐波畸变率
void Current_THD_Calculate(void)
{
    char i ;
    float sum = 0;
    for (i = 1; i < RFFT_Analyse_MaxNum; i++)
    {
        sum = sum + rfft_analyse.Amp[i] * rfft_analyse.Amp[i];
    }
    Current_THD = sqrtf(sum) / rfft_analyse.Amp[0];
}

//FFT结构体初始化
void RFFT_ADC_Init(void)
{
    RFFT_f32_setInputPtr(hnd_rfft, FFT_input);
    RFFT_f32_setOutputPtr(hnd_rfft, FFT_output);
    RFFT_f32_setStages(hnd_rfft,  FFT_STAGES);
    RFFT_f32_setFFTSize(hnd_rfft, FFT_SIZE);
    RFFT_f32_setMagnitudePtr(hnd_rfft, FFT_magnitude_phase);
    RFFT_f32_setPhasePtr(hnd_rfft, FFT_magnitude_phase);

    RFFT_f32_setTwiddlesPtr(hnd_rfft, FFT_twiddleFactors);
    RFFT_f32_sincostable(hnd_rfft);
}

void RFFT_Analyse(void)
{
    Uint16 i;
    for(i=0;i<RFFT_Analyse_MaxNum;i++)
    {
        //幅度转幅值转有效值
        rfft_analyse.Amp[i] = hnd_rfft->MagBuf[25*(i+1)] * Sample_Time * 4;
    }
    rfft_analyse.DC_Amp=hnd_rfft->MagBuf[0] * Sample_Time *2;

}

void RFFT_ADC_Cal(void)
{
    RFFT_f32(hnd_rfft);
    RFFT_f32_mag(hnd_rfft);
    RFFT_Analyse();
}

