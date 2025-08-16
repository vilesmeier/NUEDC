#include "global.h"

/**
 * @brief Init struct
 * 
 * @param pid 
 */
void PID_Init(PID* pid)
{
    pid->ActuralValue = 0;
    pid->err = 0;
    pid->err_last = 0;
    pid->Kp = 1;
    pid->Ki = 1;
    pid->Kd = 1;
    pid->integral = 0;
    pid->derivative = 0;
    pid->voltage = 0;
    pid->pid_max = 1000;
    pid->pid_max_inv = 0.0001;
    pid->pid_out = 0;
}

/**
 * @brief Set PID Transfer Fun
 * 
 * @param pid 
 * @param Kp 
 * @param Ki 
 * @param Kd 
 * @param max 
 */
void PID_Set(PID* pid, float Kp, float Ki, float Kd, float max)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->pid_max = max;
    pid->pid_max_inv = 1 / pid->pid_max;
}

/**
 * @brief Perform PID
 * 
 * @param pid 
 */
void PID_Run(PID* pid)
{
    pid->err = pid->GiveValue - pid->ActuralValue;//e=r-x
    pid->integral = pid->integral + pid->err;
    pid->derivative = (pid->err - pid->err_last);
    pid->voltage = pid->Kp * pid->err + pid->Ki * pid->integral + pid->Kd * pid->derivative;
    pid->err_last = pid->err;
    if(pid->voltage > pid->pid_max)
    {
        pid->voltage = pid->pid_max;
    }
    if(pid->voltage < -pid->pid_max)
    {
        pid->voltage = -pid->pid_max;
    }
    pid->voltage = pid->voltage * pid->pid_max_inv;
    pid->pid_out = pid->voltage * pid->pid_max_inv * 0.9f * 0.5f + 0.5;
}
