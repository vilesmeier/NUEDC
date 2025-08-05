#ifndef STATE_H
#define STATE_H

typedef struct STATE
{
    //3PH Voltage
    float V_A,V_B,V_C;
    float V_ab,V_bc,V_ca;
    //3PH Current
    float I_A,I_B,I_C;
    //target value
    float V_set_output,I_set_output;
    //paramater set
    float freq;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
