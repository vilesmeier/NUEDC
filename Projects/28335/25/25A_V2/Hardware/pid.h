#ifndef PID_H
#define PID_H

//struct definition
typedef struct PID
{
    //input values
    float ActuralValue;     //actual value of output
    float GiveValue;        //set value of output
    float FeedforwardValue; //system feedforward decouple

    //interrupt frequency
    float delta_t;
    float isr_freq;

    //paramets
    float Kp, Ki, Kd, Ka;   //calculation parameter for PID

    //clamp value
    float out_max_n;        //negetive clamp for output
    float out_max_p;        //positive clamp for output
    float int_max;          //clamp for integration

    //output value
    float nolimit;          //before clamp
    float voltage;          //after clamp
    float out;              //to PWM modulate

    //template values
    float err;              //error
    float err_last;         //error of previout cycle
    float err_anti;         //error between nolimit and voltage
    float integral;         //intigrate value of error
    float derivative;       //derivitive value of error
    float saturation;       //saturation feedback value
    float int_last;
    float div_last;

    //parameters in difference equation
    float Ap, Ai, Ad;
}PID;

//function declaration
void PID_Init(PID* pid);
void PID_Set(PID *pid, float Kp, float Ki, float Kd, float Ka, float out_max_p, float out_max_n,float int_max, float isr_freq);
void PID_Change_Kp(PID *pid, float kp);
void PID_Change_Ki(PID *pid, float ki);
void PID_Change_Kd(PID *pid, float kd);
void PID_Change_Output_Clamp(PID *pid, float upper, float lower);
void PID_Change_Int_Clamp(PID *pid, float limit);
void PID_Run(PID* pid, float target, float actual, float feedforward);


#endif // !PID_H
