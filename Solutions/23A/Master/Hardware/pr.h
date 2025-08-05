#ifndef PR_H
#define PR_H

typedef struct PR
{
    float ActuralValue;
    float GiveValue;
    float Result;
    float err;
    float e_last_1;
    float e_last_2;
    float Res_last_1;
    float Res_last_2;
    float Kp;
    float Kr;
    float Omega_c;
    float Omega_0;
    float A0, A1, A2;
    float B0, B1, B2;
    float max;
    float max_inv;
    float inv_A0;
}PR;

void PR_Init(PR* pr);
void PR_Set(PR* pr, float Kp, float Kr, float Omega_c, float Omega_0,float max);
void PR_Run(PR* pr);

#endif /* PR_H */
