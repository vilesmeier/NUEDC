#ifndef RAMP_H
#define RAMP_H

typedef struct RAMP
{
    float   isr_freq;
    float   grad_freq;
    float   step_length;
    float   ref_angle;
}RAMP;

void RAMP_Init(RAMP* ramp);
void RAMP_Set(RAMP* ramp, float grad_freq, float isr_freq);
void RAMP_Run(RAMP* ramp);

#endif // !RAMP_H
