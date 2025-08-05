#include "global.h"
#include "pir.h"

/**
 * @brief struct init
 * 
 * @param pir   struct pointer
 */
void PIR_Init(PIR *pir)
{
    pir->set = 0;
    pir->actual = 0;
    pir->feedforward = 0;
    pir->err = 0;
    pir->nolimit = 0;
    pir->output = 0;
    pir->delta_t = 0;
    pir->Kp = 0;
    pir->Kr = 0;
    pir->Ki = 0;
    pir->Ka = 0;
    pir->w_0 = 0; 
    pir->w_c = 0;
    pir->clamp_int = 0;
    pir->clamp_out_upper = 0;
    pir->clamp_out_lower = 0;
    pir->A1 = 0;
    pir->A2 = 0;
    pir->B0 = 0;
    pir->B1 = 0;
    pir->B2 = 0;
    pir->integral = 0;
    pir->err_anti = 0;
    pir->saturation = 0;
    pir->e_last_1 = 0; 
    pir->e_last_2 = 0;
    pir->Res_last_1 = 0; 
    pir->Res_last_2 = 0;
}

/**
 * @brief config PIR controller
 * 
 * @param pir           struct pointer
 * @param kp            Kp
 * @param ki            Ki
 * @param kr            Kr
 * @param ka            Ka
 * @param wc            band width
 * @param w0            contral frequency
 * @param int_clamp     integral clamp
 * @param out_up        output upper limit
 * @param out_low       output lower limit
 * @param isr_freq      interrupt frequency
 */
void PIR_Set(PIR *pir, float kp, float ki, float kr, float ka, float wc, float w0, float int_clamp, float out_up, float out_low, float isr_freq)
{
    pir->delta_t = (1.0f) / isr_freq;
    pir->Kp = kp;
    pir->Ki = ki;
    pir->Kr = kr;
    pir->Ka = ka;
    pir->w_0 = w0;
    pir->w_c = wc;
    pir->clamp_int = int_clamp;

    if (out_up > out_low)
    {
        pir->clamp_out_lower = out_low;
        pir->clamp_out_upper = out_up;
    }
    else
    {
        pir->clamp_out_lower = out_up;
        pir->clamp_out_upper = out_low;
    }

    // calculate paramters
    float temp = 0;
    temp = 4 + 4 * pir->delta_t * pir->w_c + pir->w_0 * pir->w_0 * pir->delta_t * pir->delta_t;

    pir->A1 = (2 * pir->w_0 * pir->w_0 * pir->delta_t * pir->delta_t - 8) / temp;
    pir->A2 = (4 - 4 * pir->delta_t * pir->w_c + pir->w_0 * pir->w_0 * pir->delta_t * pir->delta_t) / temp;

    pir->B0 = (4 * pir->delta_t * pir->Kr * pir->w_c) / temp;
    pir->B1 = 0;
    pir->B2 = (4 * pir->delta_t * pir->Kr * pir->w_c) / temp;
}

/**
 * @brief calculate PIR control output
 * 
 * @param pir           struct pointer
 * @param target        set value
 * @param actual        actual value
 * @param feedforward   feedforward decouple
 */
void PIR_Run(PIR *pir, float target, float actual, float feedforward)
{
    // set input data
    pir->actual = actual;
    pir->set = target;
    pir->feedforward = feedforward;

    // calculate err
    pir->err = pir->set - pir->actual;

    // i  integral err
    pir->integral = pir->err * pir->delta_t + pir->integral;

    // integral clamp
    if (pir->integral > pir->clamp_int)
    {
        pir->integral = pir->clamp_int;
    }
    else if (pir->integral < -pir->clamp_int)
    {
        pir->integral = -pir->clamp_int;
    }
    else
    {
        pir->integral = pir->integral;
    }

    // r difference equation
    pir->nolimit = pir->B0 * pir->err + pir->B1 * pir->e_last_1 + pir->B2 * pir->e_last_2 - pir->A1 * pir->Res_last_1 - pir->A2 * pir->Res_last_2;

    // update storage
    pir->e_last_1 = pir->err;
    pir->e_last_2 = pir->e_last_1;
    pir->Res_last_1 = pir->nolimit;
    pir->Res_last_2 = pir->Res_last_1;

    // get output
    pir->nolimit = pir->Kp * pir->err + pir->integral * pir->Ki + pir->nolimit + pir->feedforward;

    //output clamp
    if(pir->nolimit > pir->clamp_out_upper)
    {
        pir->output = pir->clamp_out_upper;
    }
    else if(pir->nolimit < pir->clamp_out_lower)
    {
        pir->output = pir->clamp_out_lower;
    }
    else
    {
        pir->output = pir->nolimit;
    }

    //output feedback for anti-windup
    pir->err_anti = pir->nolimit - pir->output;
    pir->saturation = pir->Ka * pir->err_anti;
    pir->integral = pir->integral - pir->saturation;

    //calculate output after anti-windup
    pir->output  = pir->output - pir->saturation;
}

