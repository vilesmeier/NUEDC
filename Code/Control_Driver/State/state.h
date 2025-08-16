#ifndef STATE_H
#define STATE_H

// flages , 16 Flag bits max
typedef struct FLAG_BIT
{

}FLAG_BIT;

typedef union FLAG
{
    Uint16 all;
    FLAG_BIT bits;
} FLAG;

// state data
typedef struct STATE
{

    //Flags for current state
    FLAG state_Flag;

    //Set parameters

    //current parameters

}STATE;

void System_Clock_Close(void);
void State_Init(void);

#endif
