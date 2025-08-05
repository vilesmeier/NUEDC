#ifndef PID_H
#define PID_H

typedef struct PID
{
    //input values
    float ActuralValue;     //actual value of output
    float GiveValue;        //set value of output
    float FeedforwardValue; //system feedforward decouple

    //template values
    float err;              //error
    float err_last;         //error of previout cycle
    float err_anti;         //error between nolimit and voltage
    float integral;         //intigrate value of error
    float derivative;       //derivitive value of error
    float saturation;       //saturation feedback value
    
    //parameters
    float Kp, Ki, Kd, Ka;   //calculation parameter for PID
    
    //clamp value
    float out_max_n;        //negetive clamp for output
    float out_max_p;        //positive clamp for output
    float int_max;          //clamp for integration

    //output value
    float nolimit;          //before clamp
    float voltage;          //after clamp
    float out;              //to PWM modulate
}PID;

void PID_Init(PID* pid);
void PID_Set(PID* pid, float Kp, float Ki, float Kd, float Ka, float out_max_n, float out_max_p,float int_max);
void PID_Run(PID* pid, float target, float actual, float feedforward);

#endif /* PID_H_ */
