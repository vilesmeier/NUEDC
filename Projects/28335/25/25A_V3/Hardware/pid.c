#include "pid.h"
#include "global.h"

/**
 * @brief Discrete time PID ,realized by tustin transform
 * 
 */

void PID_Init(PID *pid)
{
    pid->ActuralValue = 0;
    pid->GiveValue = 0;
    pid->FeedforwardValue = 0;
    pid->Ad = 0;
    pid->Ai = 0;
    pid->Ap = 0;
    pid->delta_t = 0;
    pid->isr_freq = 0;
    pid->derivative = 0;
    pid->div_last = 0;
    pid->err = 0;
    pid->err_anti = 0;
    pid->err_last = 0;
    pid->int_max = 0;
    pid->integral = 0;
    pid->int_last = 0;
    pid->integral = 0;
    pid->Ka = 0;
    pid->Kd = 0;
    pid->Ki = 0;
    pid->Kp = 0;
    pid->nolimit = 0;
    pid->out = 0;
    pid->out_max_n = 0;
    pid->out_max_p = 0;
    pid->saturation = 0;
    pid->voltage = 0;
}

/**
 * @brief set struct parameters
 * 
 * @param pid   struct pointer
 * @param Kp    Kp
 * @param Ki    Ki = Kp/Ti
 * @param Kd    Kd = Kp * Td
 * @param Ka    anti-wandup
 * @param out_max_p output upper limit
 * @param out_max_n output lower limit
 * @param int_max   intergate limit
 * @param isr_freq  PWM frequency or frequency function called
 */
void PID_Set(PID *pid, float Kp, float Ki, float Kd, float Ka, float out_max_p, float out_max_n,float int_max, float isr_freq)
{
    //parameter input
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->Ka = Ka;
    if (out_max_n > out_max_p)
    {
        pid->out_max_n  = out_max_p;
        pid->out_max_p  = out_max_n;
    }
    else
    {
        pid->out_max_n  = out_max_n;
        pid->out_max_p  = out_max_p;
    }
    pid->int_max = int_max;
    pid->isr_freq = isr_freq;
    pid->delta_t = 1 / isr_freq;

    // parameter cal
    pid->Ap = pid->Kp;
    pid->Ai = pid->Ki * pid->delta_t * 0.5f;
    pid->Ad = pid->Kd * 2.0f * pid->isr_freq;
}

/**
 * @brief change kp
 * 
 * @param pid struct pointer
 * @param kp  value
 */
void PID_Change_Kp(PID *pid, float kp)
{
    pid->Kp = kp;
    pid->Ap = pid->Kp;
}

/**
 * @brief change ki
 * 
 * @param pid struct pointer
 * @param ki  value
 */
void PID_Change_Ki(PID *pid, float ki)
{
    pid->Ki = ki;
    pid->Ai = pid->Ki * pid->delta_t * 0.5f;
}

/**
 * @brief change kd
 * 
 * @param pid struct pointer
 * @param kd  value
 */
void PID_Change_Kd(PID *pid, float kd)
{
    pid->Kd = kd;
    pid->Ad = pid->Kd * 2.0f * pid->isr_freq;
}

/**
 * @brief set output clamp
 * 
 * @param pid   struct pointer
 * @param upper upper limit
 * @param lower lower limit
 */
void PID_Change_Output_Clamp(PID *pid, float upper, float lower)
{
    if (lower > upper)
    {
        pid->out_max_n  = upper;
        pid->out_max_p  = lower;
    }
    else
    {
        pid->out_max_n  = lower;
        pid->out_max_p  = upper;
    }
}

/**
 * @brief set integral limit
 * 
 * @param pid struct pointer
 * @param limit integral limit
 */
void PID_Change_Int_Clamp(PID *pid, float limit)
{
    pid->int_max = limit;
}

/**
 * @brief perform PID control with feedforward and anti-windup
 * 
 * @param pid           struct pointer
 * @param target        target value of PID
 * @param actual        actual value of output
 * @param feedforward   feedforward decouple input
 */
void PID_Run(PID* pid, float target, float actual, float feedforward)
{
    // set PI input
    pid->ActuralValue = actual;
    pid->GiveValue = target;
    pid->FeedforwardValue = feedforward;

    // get error
    pid->err = pid->GiveValue - pid->ActuralValue;

    //calculate integral and derivative
    pid->integral = pid->int_last + pid->Ai * (pid->err + pid->err_last);
    pid->derivative = -pid->div_last + pid->Ad * (pid->err - pid->err_last);

    //clamp for integration
    if(pid->integral > pid->int_max)
    {
        pid->integral = pid->int_max;
    }
    else if(pid->integral < -pid->int_max)
    {
        pid->integral = -pid->int_max;
    }
    else
    {
        pid->integral = pid->integral;
    }

    //calculate output
    pid->nolimit = pid->Ap * pid->err + pid->integral + pid->derivative;

    //clamp for output
    if(pid->nolimit > pid->out_max_p)
    {
        pid->voltage = pid->out_max_p;
    }
    else if(pid->nolimit < pid->out_max_n)
    {
        pid->voltage = pid->out_max_n;
    }
    else
    {
        pid->voltage = pid->nolimit;
    }

    //output feedback for anti-windup
    pid->err_anti = pid->nolimit - pid->voltage;
    pid->saturation = pid->Ka * pid->err_anti;
    pid->integral = pid->integral - pid->saturation;

    //calculate output after anti-windup
    pid->voltage  = pid->voltage - pid->saturation;

    //result storge
    pid->err_last = pid->err;
    pid->int_last = pid->integral;
    pid->div_last = pid->derivative; 
}
