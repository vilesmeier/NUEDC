#ifndef STATE_H
#define STATE_H

typedef struct STATE
{
    //working mode
    Uint8 MODE;
    //input value
    float V_input,I_input;
    //output value
    float V_output,I_output;
    //target value
    float V_set_output,I_set_output;
    //paramater set
    float Xint_select;
    float kp,ki;
    //graph value
    Uint8 ZeroCross;
    float pi_V_GiveValue;
    float slope_INV;
    float I0;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
