#include "global.h"

/**
 * @brief init struct
 * 
 * @param ramp 
 */
void Ramp_Init(RAMP* ramp)
{
    ramp->grad_freq = 0;
    ramp->isr_freq = 0;
    ramp->ref_angle = 0;
    ramp->step_length = 0;
}

/**
 * @brief set parameters,frequent in Hz
 * 
 * @param ramp 
 * @param grad_freq 
 * @param isr_freq 
 */
void Ramp_Set(RAMP* ramp, float grad_freq, float isr_freq)
{
    ramp->grad_freq = grad_freq;
    ramp->isr_freq = isr_freq;
    ramp->step_length = ramp->grad_freq * 2 * 3.1415926f / ramp->isr_freq ;
}

/**
 * @brief calculate refence angle when system have no refence angle(like main grid)
 * 
 * @param ramp 
 */
void Ramp_Run(RAMP* ramp)
{
    ramp->ref_angle = ramp->ref_angle + ramp->step_length;
    if (ramp->ref_angle > 2 * 3.1415926f)
    {
        ramp->ref_angle = 0;
    }
}
