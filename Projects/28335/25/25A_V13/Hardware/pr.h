#ifndef PR_H
#define PR_H

typedef struct PR
{
    //input values
    float ActuralValue;             //actual value of output
    float GiveValue;                //set value of output
    float FeedforwardValue;         //system feedforward decouple

    //template values
    float err;                      //error
    float e_last_1,e_last_2;        //error of previous cycle
    float Res_last_1,Res_last_2;    //output of previous cycle

    //parameters
    float Kp,Kr;                    //parameters
    float Omega_c, Omega_0;         //centeral frequency and bandwidth
    float T_isr;                    //frequent of function called

    //transfer function factors
    float A0, A1, A2;           
    float B0, B1, B2;
    
    //clamp value
    float max;                      //clamp for output
    float max_inv;                  //inverse clamp value for output

    //output value
    float nolimit;                  //output before clamp
    float voltage;                  //output after clamp
    float out;                      //output after multiply
}PR;

void PR_Init(PR* pr);
void PR_Set(PR* pr, float Kp, float Kr, float Omega_c, float Omega_0,float max,float isr_freq);
void PR_Run(PR* pr, float target, float actual, float feedforward);

//#pragma CODE_SECTION(PR_Init, ".TI.ramfunc");
#pragma CODE_SECTION(PR_Set, ".TI.ramfunc");
#pragma CODE_SECTION(PR_Run, ".TI.ramfunc");

#endif /* PR_H */
