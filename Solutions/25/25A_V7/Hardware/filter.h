#ifndef FILTER_H
#define FILTER_H

//sturct definition
typedef struct RC_LPF
{
    //discrete frequency
    float isr_freq;
    float delta_t;

    //time constant
    float t;
    float fstop;

    //difference equation parameter
    float B0, A0, A1;

    //result and input storage
    float input[2], result[2];  //use [0] as current data, use [1] as previous data

} RC_LPF;

//function declaration
void Filter_RC_LPF_Init(RC_LPF *rc);
void Filter_RC_LPF_Set(RC_LPF *rc, float fstop, float isr_freq);
void Filter_RC_LPF_Run(RC_LPF *rc, float input);

#endif // !FILTER_H=
