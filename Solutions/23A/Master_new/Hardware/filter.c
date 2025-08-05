#include "filter.h"
#include "global.h"

/**
 * @brief init digita RC Low-pass Filter
 * 
 * @param rc struct pointer
 */
void Filter_RC_LPF_Init(RC_LPF *rc)
{
    rc->A0 = 0;
    rc->A1 = 0;
    rc->B0 = 0;
    rc->delta_t = 0;
    rc->isr_freq = 0;
    rc->t = 0;
    rc->fstop = 0;
    rc->input[0] = 0;
    rc->input[1] = 0;
    rc->result[0] = 0;
    rc->result[1] = 0;
}

/**
 * @brief config LPF parameters
 * 
 * @param rc        struct pointer
 * @param fstop     stop frequency
 * @param isr_freq  called frequency
 */
void Filter_RC_LPF_Set(RC_LPF *rc, float fstop, float isr_freq)
{
    rc->fstop = fstop;
    rc->isr_freq = isr_freq;
    rc->delta_t = 1.0f / isr_freq;
    rc->t = 1.0f/ (2.0f * 3.1415926535f * rc->fstop);
    rc->B0 = -(rc->delta_t - 2 * rc->t) / (rc->delta_t + 2 * rc->t);
    rc->A0 = (rc->delta_t) / (rc->delta_t + 2 * rc->t);
    rc->A1 = rc->A0;
}

/**
 * @brief perform digital RC lPF
 * 
 * @param rc        struct pointer
 * @param input     input signal data
 */
void Filter_RC_LPF_Run(RC_LPF *rc, float input)
{
    //storage input
    rc->input[0] = input;

    //get output
    rc->result[0] = rc->result[1] * rc->B0 + rc->input[0] * rc->A0 + rc->input[1] * rc->A1;

    //result input update
    rc->input[1] = rc->input[0];
    rc->result[1] = rc->result[0];
}
