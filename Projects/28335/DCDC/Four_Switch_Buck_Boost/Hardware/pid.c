#include "global.h"
#include "pid.h"

/**
 * @brief Init struct to default value
 * 
 * @param pid struct pointer
 */
void PID_Init(PID* pid)
{
    //input values
    pid->ActuralValue       = 0.0f;     //actual value of output
    pid->GiveValue          = 0.0f;     //set value of output
    pid->FeedforwardValue   = 0.0f;     //system feedforward decouple

    //template values
    pid->err        = 0.0f;             //error
    pid->err_last   = 0.0f;             //error of previout cycle
    pid->err_anti   = 0.0f;             //error between nolimit and voltage
    pid->integral   = 0.0f;             //intigrate value of error
    pid->derivative = 0.0f;             //derivitive value of error
    pid->saturation = 0.0f;             //saturation feedback value
    
    //calculation parameter for PID
    pid->Kp = 1.0f;
    pid->Ki = 1.0f;
    pid->Kd = 1.0f;
    pid->Ka = 1.0f;

    //clamp value
    pid->int_max        = 1.0f;        //clamp for integration
    pid->out_max_p      = 1.0f;        //positive clamp for output
    pid->out_max_n      = 0.0f;        //negetive clamp for output

    //output value
    pid->nolimit    = 0.0f;            //before clamp
    pid->voltage    = 0.0f;            //after clamp
}

/**
 * @brief set parameters for PID calculate
 * 
 * @param pid struct pointer
 * @param Kp Proportion parameter
 * @param Ki Integration parameter
 * @param Kd Derivative parameter
 * @param Ka Anti-wingup parameter
 * @param out_max_p positive output clamp
 * @param out_max_n negetive output clamp
 * @param int_max integration clamp
 */
void PID_Set(PID* pid, float Kp, float Ki, float Kd, float Ka, float out_max_n, float out_max_p,float int_max)
{
    pid->Kp         = Kp;
    pid->Ki         = Ki;
    pid->Kd         = Kd;
    pid->Ka         = Ka;
    pid->int_max    = int_max;
    pid->out_max_n  = out_max_n;
    pid->out_max_p  = out_max_p;
}

/**
 * @brief perform pid control
 * 
 * @param pid           struct pointer
 * @param target        target value
 * @param actual        actual value
 * @param feedforward   feedfordward value
 */
void PID_Run(PID* pid, float target, float actual, float feedforward)
{
    // set actual value
    pid->ActuralValue = actual;
    pid->GiveValue = target;
    pid->FeedforwardValue = feedforward;

    // get error
    pid->err = pid->GiveValue - pid->ActuralValue;

    //calculate integral and derivative
    pid->integral = pid->integral + pid->err;
    pid->derivative = pid->err - pid->err_last;

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
    pid->nolimit = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * pid->derivative + pid->FeedforwardValue;

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
}
