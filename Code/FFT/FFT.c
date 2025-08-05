#include "FFT.h"

//FFT部分需要对齐
#pragma DATA_SECTION(FFT_input, "FFT_buffer1")
#pragma DATA_SECTION(FFT_output, "FFT_buffer2")
#pragma DATA_SECTION(FFT_magnitude_phase, "FFT_buffer3")
#pragma DATA_SECTION(FFT_twiddleFactors, "FFT_buffer4")


//谐波畸变率
void THD_Calculate(RFFT_Analyser* rfft)
{
    char i ;
    float sum = 0;
    for (i = 1; i < rfft->hnd_rfft->FFTStages; i++)
    {
        sum = sum + rfft->Amp[i] * rfft->Amp[i];
    }
    rfft->Signal_THD = sqrtf(sum) / rfft -> Amp[0];
}

//FFT结构体初始化
void RFFT_Init(RFFT_Analyser* rfft,uint16_t stage)
{
    //指针关联
    rfft->hnd_rfft = &(rfft->rfft);
    RFFT_f32_setInputPtr(rfft->hnd_rfft, rfft->input);
    RFFT_f32_setOutputPtr(rfft->hnd_rfft, rfft->output);
    RFFT_f32_setStages(rfft->hnd_rfft,  stage);
    RFFT_f32_setFFTSize(rfft->hnd_rfft, (1<<stage));
    RFFT_f32_setMagnitudePtr(rfft->hnd_rfft, rfft->magnitude_phase);
    RFFT_f32_setPhasePtr(rfft->hnd_rfft, rfft->magnitude_phase);
    RFFT_f32_setTwiddlesPtr(rfft->hnd_rfft, rfft->twiddleFactors);
    RFFT_f32_sincostable(rfft->hnd_rfft);

    //后处理块初始化
    int i = 0;
    for (i = 0; i < rfft->hnd_rfft->FFTSize; i++)
    {
        rfft->hnd_rfft->InBuf[i] = 0;
        rfft->hnd_rfft->OutBuf = 0;
    }
    for (i = 0; i < ((rfft->hnd_rfft->FFTSize >> 1) + 1); i++)
    {
        rfft->hnd_rfft->MagBuf[i] = 0;
    }
    for (i = 0; i < (rfft->hnd_rfft->FFTSize >> 1); i++)
    {
        rfft->hnd_rfft->PhaseBuf[i] = 0;
    }
}

////幅度转幅值转有效值
void RFFT_Analyse(RFFT_Analyser* rfft)
{
    uint8_t i;
    for(i = 0 ; i < FFT_STAGES ; i++)
    {
        rfft->Amp[i] = rfft->hnd_rfft->MagBuf[25*(i+1)] / FFT_SIZE * 2;
    }
    rfft->DC_Amp=rfft->hnd_rfft->MagBuf[0] / FFT_SIZE ;
}


//完整的FFT运算，操作打包
void RFFT_Cal(RFFT_Analyser* rfft)
{
    RFFT_f32(rfft->hnd_rfft);
    RFFT_f32_mag(rfft->hnd_rfft);
    RFFT_Analyse(rfft);
    THD_Calculate(rfft);
}

