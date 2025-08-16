#ifndef STATE_H
#define STATE_H

typedef struct STATE
{
    Uint8 Flag_Output_Mode;
    Uint8 Flag_Output_Enable;
    Uint8 Flag_Soft_Start;
    float V_set_rms;
    float V_output;
    float I_set_rms;
    float I_softstart;
    float I_output;
    float I_load;
    float I_total_set_rms;
    float I_distribute_rate;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
