#ifndef PIR_H
#define PIR_H

// struct definition
typedef struct PIR
{
    // input value
    float set;
    float actual;
    float feedforward;
    float err;

    //output
    float nolimit;
    float output;

    // discrete step length
    float delta_t;

    // control parameter
    float Kp,Kr,Ki,Ka;
    float w_0, w_c;

    // clamp
    float clamp_int;
    float clamp_out_upper;
    float clamp_out_lower;

    // calculate paramters
    float A1,A2,B0,B1,B2;   // different equation
    float integral;
    float err_anti;
    float saturation;

    // input storage
    float e_last_1, e_last_2;

    // output storage
    float Res_last_1, Res_last_2;

} PIR;

void PIR_Init(PIR *pir);
void PIR_Set(PIR *pir, float kp, float ki, float kr, float ka, float wc, float w0, float int_clamp, float out_up, float out_low, float isr_freq)
void PIR_Run(PIR *pir, float target, float actual, float feedforward);




#endif // !PIR_H
