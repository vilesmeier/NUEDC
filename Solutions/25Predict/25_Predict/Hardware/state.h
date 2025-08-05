#ifndef STATE_H
#define STATE_H

// flages , 16 Flag bits max
typedef struct FLAG_BIT
{
    // definition of flags
    Uint16 Flag_VDC_OV : 1;     // DC voltage over
    Uint16 Flag_VDC_ON : 1;     // DC capacitor precharge
    Uint16 Flag_INV_EN : 1;     // Inverter enable output
    Uint16 Flag_INV_SS : 1;     // Inverter soft start state

}FLAG_BIT;

typedef union FLAG
{
    Uint16 all;
    FLAG_BIT bits;
} FLAG;

// state data
typedef struct STATE
{

    // Flags for current state
    FLAG state_Flag;

    // Set parameters

    // Current parameters

}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
