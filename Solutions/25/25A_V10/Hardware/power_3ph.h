#ifndef POWER_3PH_H
#define POWER_3PH_H

// struct definition
typedef struct POWER_3PH
{
    // input data
    float Va,Vb,Vc;
    float Ia,Ib,Ic;

    //output data
    float Varms,Vbrms,Vcrms;
    float Iarms,Ibrms,Icrms;
    float P;
    float Q;
    float S;
    float PF;

    //parameters
    float isr_freq,grid_freq;
    float nsample;
    float nsample_inv;

    // temp data
    float va_sum,vb_sum,vc_sum;
    float ia_sum,ib_sum,ic_sum;
    float sample_num;
    float p_sum;
} POWER_3PH;

void POWER_3ph_Init(POWER_3PH *power);
void POWER_3ph_Set(POWER_3PH *power, float isr_freq, float grid_freq);
void POWER_3ph_Set_Voltage(POWER_3PH *power, float Va, float Vc);
void POWER_3ph_Set_Current(POWER_3PH *power, float Ia, float Ic);
void POWER_3ph_Run(POWER_3PH *power);
void POWER_3ph_RMS_Run(POWER_3PH *power, float Varms, float Vbrms, float Vcrms, float Iarms, float Ibrms, float Icrms, float PF);





#endif // !POWER_3PH_H
