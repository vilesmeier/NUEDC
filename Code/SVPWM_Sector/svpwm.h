#ifndef SVPWM_H
#define SVPWM_H

typedef struct SVPWM
{
    float u1,u2,u3;             //Determain Uref Sector
    float t1,t2,t3,t4,t5,t6;    //active time for 6 space vector
    float duty1,duty2,duty3;    //duty cycle for 3 phase
    float Mod_Rate;             //modulate rate for over modulate
    float k;
    float Ts;
    Uint8 Sector;               //Uref Sector
    Uint16 comp1,comp2,comp3;   //Compare value for EPWM
    Uint16 TBPRD;               //Period Value for EPWM
}SVPWM;

void SVPWM_Init(SVPWM* svpwm);
void SVPWM_Set(SVPWM* svpwm, Uint16 tbprd, float Udc ,float f_isr);
void SVPWM_Parameter_Cal(SVPWM* svpwm, float Ualpha, float Ubeta);
void SVPWM_Sector_Cal(SVPWM* svpwm, float Ualpha, float Ubeta);
void SVPWM_Duty_Update(SVPWM* svpwm);
void SVPWM_Run(SVPWM* svpwm, float Ualpha, float Ubeta);

#endif // !SVPWM_H
