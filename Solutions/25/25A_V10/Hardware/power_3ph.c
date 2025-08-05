#include "global.h"
#include "power_3ph.h"

/**
 * @brief struct init   
 * 
 * @param power struct pointer
 */
void POWER_3ph_Init(POWER_3PH *power)
{
    power->Va = 0.0f;
    power->Vb = 0.0f;
    power->Vc = 0.0f;
    power->Ia = 0.0f;
    power->Ib = 0.0f;
    power->Ic = 0.0f;

    power->Varms = 0.0f;
    power->Vbrms = 0.0f;
    power->Vcrms = 0.0f;
    power->Iarms = 0.0f;
    power->Ibrms = 0.0f;
    power->Icrms = 0.0f;
    power->P = 0.0f;
    power->Q = 0.0f;
    power->S = 0.0f;
    power->PF = 0.0f;

    power->isr_freq = 0.0f;
    power->grid_freq = 0.0f;
    power->nsample = 0.0f;
    power->nsample_inv = 0.0f;

    power->va_sum = 0.0f;
    power->vb_sum = 0.0f;
    power->vc_sum = 0.0f;
    power->ia_sum = 0.0f;
    power->ib_sum = 0.0f;
    power->ic_sum = 0.0f;
    power->sample_num = 0.0f;
    power->p_sum = 0.0f;
}

/**
 * @brief   set struct 
 * 
 * @param power         srruct pointer
 * @param isr_freq      call freq, in Hz
 * @param grid_freq     grid freq, in Hz
 */
void POWER_3ph_Set(POWER_3PH *power, float isr_freq, float grid_freq)
{
    power->grid_freq = grid_freq;
    power->isr_freq = isr_freq;
    power->nsample = isr_freq / grid_freq * 0.5f;
    power->nsample_inv = (1.0f) / power->nsample;
}

/**
 * @brief set input voltage , phase value
 * 
 * @param power     struct pointer
 * @param Va        phase A voltage,in V
 * @param Vb        phase B voltage,in V
 * @param Vc        phase C voltage,in V
 */
void POWER_3ph_Set_Voltage(POWER_3PH *power, float Va, float Vc)
{
    power->Va = Va;
    power->Vb = -Va-Vc;
    power->Vc = Vc;
}

/**
 * @brief set input current , phase value
 * 
 * @param power struct pointer
 * @param Ia    phase A current, in A
 * @param Ib    phase B current, in A
 * @param Ic    phase C current, in A
 */
void POWER_3ph_Set_Current(POWER_3PH *power, float Ia, float Ic)
{
    power->Ia = Ia;
    power->Ib = -Ia-Ic;
    power->Ic = Ic;
}

/**
 * @brief   calculate power, update at grid freq
 * 
 * @param power struct pointer
 */
void POWER_3ph_Run(POWER_3PH *power)
{
    // getting current sample number
    power->sample_num = power->sample_num + 1;

    // rms for volate and current
    power->va_sum = power->Va * power->Va + power->va_sum;
    power->vb_sum = power->Vb * power->Vb + power->vb_sum;
    power->vc_sum = power->Vc * power->Vc + power->vc_sum;
    power->ia_sum = power->Ia * power->Ia + power->ia_sum;
    power->ib_sum = power->Ib * power->Ib + power->ib_sum;
    power->ic_sum = power->Ic * power->Ic + power->ic_sum;
    power->p_sum = power->Va * power->Ia + power->Vb * power->Ib + power->Vc * power->Ic + power->p_sum;

    // when sample == nsample, indecating get a sine period
    // update output
    if (power->nsample == power->sample_num)
    {
        // voltage rms
        power->Varms = sqrtf(power->va_sum * power->nsample_inv);
        power->Vbrms = sqrtf(power->vb_sum * power->nsample_inv);
        power->Vcrms = sqrtf(power->vc_sum * power->nsample_inv);

        // current rms
        power->Iarms = sqrtf(power->ia_sum * power->nsample_inv);
        power->Ibrms = sqrtf(power->ib_sum * power->nsample_inv);
        power->Icrms = sqrtf(power->ic_sum * power->nsample_inv);

        // power
        power->P = power->nsample_inv * power->p_sum;
        power->S = power->Varms * power->Iarms + power->Vbrms * power->Ibrms + power->Vcrms * power->Icrms;
        power->PF = power->P / power->S;
        power->Q = power->S * power->S - power->P *power->P;
        if (power->Q < 0)
        {
            power->Q = 0;
        }
        power->Q = sqrtf(power->Q);

        // reset integrator
        power->va_sum = 0;
        power->vb_sum = 0;
        power->vc_sum = 0;
        power->ia_sum = 0;
        power->ib_sum = 0;
        power->ic_sum = 0;
        power->p_sum = 0;

        // reset sample counter
        power->sample_num = 0;
    }
}

/**
 * @brief calculate power when 6 RMS value is known
 * 
 * @param power     struct pointer
 * @param Varms     phase A voltage RMS, in V, phase value
 * @param Vbrms     phase B voltage RMS, in V, phase value
 * @param Vcrms     phase C voltage RMS, in V, phase value
 * @param Iarms     phase A current RMS, in A, phase value
 * @param Ibrms     phase B current RMS, in A, phase value
 * @param Icrms     phase C current RMS, in A, phase value
 * @param PF        power fector
 */
void POWER_3ph_RMS_Run(POWER_3PH *power, float Varms, float Vbrms, float Vcrms, float Iarms, float Ibrms, float Icrms, float PF)
{
    power->Varms = Varms;
    power->Vbrms = Vbrms;
    power->Vcrms = Vcrms;
    power->Iarms = Iarms;
    power->Ibrms = Ibrms;
    power->Icrms = Icrms;

    power->S = power->Varms * power->Iarms + power->Vbrms * power->Ibrms + power->Vcrms * power->Icrms;
    power->P =  power->PF * power->S;
    power->Q = power->S * power->S - power->P *power->P;
    if (power->Q < 0)
    {
        power->Q = 0;
    }
    power->Q = sqrtf(power->Q);
}
