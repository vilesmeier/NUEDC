#ifndef RAMP_H
#define RAMP_H

typedef struct RAMP
{
    float   isr_freq;
    float   grad_freq;
    float   step_length;
    float   ref_angle;
}RAMP;

void Ramp_Init(RAMP* ramp);
void Ramp_Set(RAMP* ramp, float grad_freq, float isr_freq);
void Ramp_Run(RAMP* ramp);

//#pragma CODE_SECTION(Ramp_Init, ".TI.ramfunc");
#pragma CODE_SECTION(Ramp_Set, ".TI.ramfunc");
#pragma CODE_SECTION(Ramp_Run, ".TI.ramfunc");

#endif // !RAMP_H
