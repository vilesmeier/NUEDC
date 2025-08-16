#ifndef STATE_H
#define STATE_H

// flages , 16 Flag bits max
typedef struct FLAG_BIT
{
    Uint16 GPIO30_CUR;
    Uint16 GPIO30_PRE;

    Uint16 GPIO31_CUR;
    Uint16 GPIO31_PRE;

    Uint16 GPIO49_CUR;
    Uint16 GPIO49_PRE;

    Uint16 GPIO48_CUR;
    Uint16 GPIO48_PRE;


}FLAG_BIT;

typedef union FLAG
{
    Uint16 all;
    FLAG_BIT bits;
} FLAG;

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

    FLAG flag;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
