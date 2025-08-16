#ifndef STATE_H
#define STATE_H

typedef struct STATE
{
    //State Flag
    Uint8 Flag_Input_Enable;
    Uint8 Flag_Output_Enable;
    Uint8 Flag_Output_SoftStart;
    Uint8 Flag_Input_SoftStart;

    //Actual Value
    float V_Input;
    float I_Input;
    float V_DC;
    float I_DC;
    float V_Output;
    float I_output;

    //Set Value
    Uint8 Load_Type;
    float PF;
    float Theta;
    float I_Input_Set_RMS;
    float V_Output_Set_RMS;
    float V_DC_Set;
    float V_Output_Softstart;
    float I_Input_Softstart;

    //key select
    Uint8 Key_Set;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
