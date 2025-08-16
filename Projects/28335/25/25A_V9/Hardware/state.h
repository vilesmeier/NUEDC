#ifndef STATE_H
#define STATE_H

// flages , 16 Flag bits max
typedef struct FLAG_BIT
{
    // feedback unit status
    Uint16 FLAG_REC_EN : 1;
    // freq change status
    Uint16 FLAG_FREQ_CHANGE : 1;
    // Softstart
    Uint16 I_softstart : 1;
    Uint16 V_softstart : 1;
    // precharge for DC capacitor
    Uint16 FLAG_CAP_PRECHARGE : 1;

} FLAG_BIT;

typedef union FLAG
{
    Uint16 all;
    FLAG_BIT bits;
} FLAG;

typedef struct KEY_BIT
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

    Uint16 GPIO51_CUR : 1;
    Uint16 GPIO51_PRE : 1;

    Uint16 GPIO50_CUR : 1;
    Uint16 GPIO50_PRE : 1;

    Uint16 GPIO53_CUR : 1;
    Uint16 GPIO53_PRE : 1;
} KEY_BIT;

typedef union KEY
{
    Uint16 all;
    KEY_BIT bits;
} KEY;

typedef struct STATE
{
    // Inverter output Voltage
    float V_A,V_C;
    float V_ab,V_bc;
    float I_out_A, I_out_C;

    // Rectifier input Current
    float I_In_a,I_In_c;

    // system reference angle
    float ref_a,ref_c;

    // Rectifier decect
    float V_test;

    // precharge detect
    float V_cap;

    // target value
    float V_set_output,I_set_output;

    // power factor
    float PF;

    // Power up delay
    float I_softstart;

    //paramater set
    float freq;

    // Flags
    FLAG flag;
    KEY key;
}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
