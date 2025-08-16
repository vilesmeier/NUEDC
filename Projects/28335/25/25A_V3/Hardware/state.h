#ifndef STATE_H
#define STATE_H

// flages , 16 Flag bits max
typedef struct FLAG_BIT
{
    // get posedge for keys
    Uint16 GPIO30_CUR : 1;
    Uint16 GPIO30_PRE : 1;

    Uint16 GPIO31_CUR : 1;
    Uint16 GPIO31_PRE : 1;

    Uint16 GPIO49_CUR : 1;
    Uint16 GPIO49_PRE : 1;

    Uint16 GPIO48_CUR : 1;
    Uint16 GPIO48_PRE : 1;

    // feedback unit status
    Uint16 FLAG_REC_EN : 1;
    // freq change status
    Uint16 FLAG_FREQ_CHANGE : 1;
    //Softstart
    Uint16 I_softstart : 1;

}FLAG_BIT;

typedef union FLAG
{
    Uint16 all;
    FLAG_BIT bits;
} FLAG;

typedef struct STATE
{
    // Inverter output Voltage
    float V_A,V_B,V_C;
    float V_ab,V_bc,V_ca;

    // Rectifier input Current
    float I_In_a,I_In_b,I_In_c;

    // system reference angle
    float ref_a,ref_b,ref_c;

    // Rectifier decect
    float V_test;

    //target value
    float V_set_output,I_set_output;

    //Softstart
    float I_softstart;
    float pr_I_set;

    //paramater set
    float freq;

    FLAG flag;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
