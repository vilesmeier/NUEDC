#ifndef NOTCH_H
#define NOTCH_H

typedef struct NOTCH
{
    //datas
    float input;
    float result;

    //result stroge
    float in_last_1;
    float in_last_2;
    float res_last_1;
    float res_last_2;

    //calculate parameters
    float A2, A1, A0;
    float B2, B1, B0;
    float delta_t;
    float omega_0;

    //Mode select
    Uint8 mode;
}NOTCH;

//Module Definition
#define MODE_BANDPASS 0 
#define MODE_BANDSTOP 1

void NOTCH_Init(NOTCH* notch);
void NOTCH_Set(NOTCH* notch, float fstop, float fsample,float mode);
void NOTCH_Run(NOTCH* notch, float input);

#endif /* NOTCH_H_ */
