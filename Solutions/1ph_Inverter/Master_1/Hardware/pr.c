#include "global.h"
#include "pr.h"

/**
 * @brief Init Struct
 * 
 * @param pr struct pointer
 */
void PR_Init(PR* pr)
{
    //input values
    pr->ActuralValue = 0;             //actual value of output
    pr->GiveValue = 0;                //set value of output
    pr->FeedforwardValue = 0;         //system feedforward decouple

    //template values
    pr->err = 0;                      //error
    pr->e_last_1 = 0;
    pr->e_last_2 = 0;        //error of previous cycle
    pr->Res_last_1 = 0;
    pr->Res_last_2 = 0;    //output of previous cycle

    //parameters
    pr->Kp = 0;
    pr->Kr = 0;                    //parameters
    pr->Omega_c = 0;
    pr->Omega_0 = 0;         //centeral frequency and bandwidth
    pr->T_isr = 0.00005f;

    //transfer function factors
    pr->A0 = 0;
    pr->A1 = 0;
    pr->A2 = 0;
    pr->B0 = 0;
    pr-> B1 = 0;
    pr-> B2 = 0;
    pr->inv_A0 = 0;

    //clamp value
    pr->max = 0;                      //clamp for output
    pr->max_inv = 0;                  //inverse clamp value for output

    //output value
    pr->nolimit = 0;                  //output before clamp
    pr->voltage = 0;                  //output after clamp
    pr->out = 0;                      //output after multiply
}

/**
 * @brief Set QPR Transfer Fun
 * 
 * @param pr        struct pointer
 * @param Kp        proportion factor
 * @param Kr        resonance factor
 * @param Omega_c   centeral frequency
 * @param Omega_0   Bandwidth in rad/s
 * @param max       Clamp for output
 * @param isr_freq  interrput frequent in Hz
 */
void PR_Set(PR* pr, float Kp, float Kr, float Omega_c, float Omega_0,float max,float isr_freq)
{
    pr->Kp = Kp;
    pr->Kr = Kr;
    pr->Omega_c = Omega_c;
    pr->Omega_0 = Omega_0;
    pr->T_isr = 1 / isr_freq;
    pr->B0 = 4 * pr->Kp + 4 * pr->T_isr * (pr->Kp + pr->Kr) * pr->Omega_c + pr->Kp * pr->Omega_0 * pr->Omega_0 * pr->T_isr * pr->T_isr;
    pr->B1 = pr->Kp * (2 * pr->Omega_0 * pr->Omega_0 * pr->T_isr * pr->T_isr - 8);
    pr->B2 = 4 * pr->Kp - 4 * pr->T_isr * (pr->Kp + pr->Kr) * pr->Omega_c + pr->Kp * pr->Omega_0 * pr->Omega_0 * pr->T_isr * pr->T_isr;
    pr->A0 = 4 + 4 * pr->T_isr * pr->Omega_c + pr->Omega_0 * pr->Omega_0 * pr->T_isr * pr->T_isr;
    pr->A1 = 2 * pr->Omega_0 * pr->Omega_0 * pr->T_isr * pr->T_isr - 8;
    pr->A2 = 4 - 4 * pr->T_isr * pr->Omega_c + pr->Omega_0 * pr->Omega_0 * pr->T_isr * pr->T_isr;
    pr->max = max;
    pr->max_inv = 1 / pr->max;
    pr->inv_A0 = 1 / pr->A0;
}

/**
 * @brief perform PR control
 * 
 * @param pr            struct pointer
 * @param target        target value
 * @param actual        actual value
 * @param feedforward   feedfordward value
 */
void PR_Run(PR* pr, float target, float actual, float feedforward)
{
    //set input value
    pr->ActuralValue     = actual;
    pr->GiveValue        = target;
    pr->FeedforwardValue = feedforward;

    // get error
    pr->err = pr->GiveValue - pr->ActuralValue;

    //transfer function
    pr->nolimit = (pr->B0 * pr->err + pr->B1 * pr->e_last_1 + pr->B2 * pr->e_last_2 - pr->A1 * pr->Res_last_1 - pr->A2 * pr->Res_last_2) * pr->inv_A0;

    //error update
    pr->e_last_2 = pr->e_last_1;
    pr->e_last_1 = pr->err;

    //resule update
    pr->Res_last_2 = pr->Res_last_1;
    pr->Res_last_1 = pr->nolimit;

    //feedforward
    pr->nolimit = pr->nolimit + pr->FeedforwardValue;

    //clamp
    if (pr->nolimit > pr->max)
    {
        pr->voltage = pr->max;
    }
    else if(pr->nolimit < -pr->max)
    {
        pr->voltage = -pr->max;
    }
    else
    {
        pr->voltage = pr->nolimit;
    }

    //multiply
    pr->out = pr->voltage * pr->max_inv * 0.95f;
}
