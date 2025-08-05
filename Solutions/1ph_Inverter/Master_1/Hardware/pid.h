#ifndef PID_H
#define PID_H

typedef struct PID
{
    float ActuralValue;
    float GiveValue;
    float err;
    float err_last;
    float Kp, Ki, Kd;
    float voltage;
    float integral;
    float derivative;
    float pid_max;
    float pid_max_inv;
    float pid_out;
}PID;

void PID_Init(PID* pid);
void PID_Set(PID* pid, float Kp, float Ki, float Kd, float max);
void PID_Run(PID* pid);

#endif /* PID_H_ */
