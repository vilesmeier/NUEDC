#include "global.h"
#include "notch.h"

/**
 * @brief Digital Notch Filter
 * @transfer function (s^2-w^2)/(s^2-50s-w^2)
 */

/**
 * @brief Init Struct
 *
 * @param notch struct pointer
 */
void NOTCH_Init(NOTCH* notch)
{
    notch->omega_0 = 0.0f;

    notch->input = 0.0f;
    notch->result = 0.0f;

    notch->in_last_1 = 0.0f;
    notch->in_last_2 = 0.0f;
    notch->res_last_1 = 0.0f;
    notch->res_last_2 = 0.0f;

    notch->A2 = 0;
    notch->A1 = 0;
    notch->A0 = 0;
    notch->B2 = 0;
    notch->B1 = 0;
    notch->B0 = 0;

    notch->mode = MODE_BANDSTOP;
}

/**
 * @brief calculate Notch Filter parameters
 *
 * @param notch     struct pointer
 * @param fstop     stop frequency point in Hz
 * @param fsample   sample frequency in Hz
 * @param mode      filter mode (bandpass/bandstop)
 */
void NOTCH_Set(NOTCH* notch, float fstop, float fsample,Uint8 mode)
{
    //in band stop mode,notch filter
    if(mode == MODE_BANDSTOP)
    {
        notch->omega_0 = 2* 3.1415926f * fstop;
        notch->delta_t = 1 / fsample;
        notch->mode = mode;

        notch->A2 =  4 + notch->omega_0*notch->omega_0 * notch->delta_t * notch->delta_t;
        notch->A1 = -8 + notch->omega_0*notch->omega_0 * notch->delta_t * notch->delta_t * 2;
        notch->A0 = notch->A2;

        notch->B2 = notch->A2 + 100 * notch->delta_t;
        notch->B1 = notch->A1;
        notch->B0 = notch->A0 - 100 * notch->delta_t;
    }

    //in band pass mode
    else
    {
        notch->omega_0 = 2* 3.1415926f * fstop;
        notch->delta_t = 1 / fsample;
        notch->mode = mode;




    }
}

/**
 * @brief perform notch filter
 *
 * @param notch struct poniter
 * @param input input signal data
 */
void NOTCH_Run(NOTCH* notch, float input)
{
    notch->input = input;

    // Get Result
    notch->result = (notch->A2 * notch->input + notch->A1 * notch->in_last_1 + notch->A0 * notch->in_last_2 - notch->B1 * notch->res_last_1 - notch->B0 * notch->res_last_2) / notch->B2;

    //delay,update stroge
    notch->in_last_2 = notch->in_last_1;
    notch->in_last_1 = notch->input;

    notch->res_last_2 = notch->res_last_1;
    notch->res_last_1 = notch->result;
}
