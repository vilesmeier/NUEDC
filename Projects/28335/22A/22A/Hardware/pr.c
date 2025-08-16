#include "global.h"

/**
 * @brief Init Struct
 * 
 * @param pr 
 */
void PR_Init(PR* pr)
{
    pr->ActuralValue = 0;
    pr->err = 0;
    pr->e_last_1 = 0;
    pr->e_last_2 = 0;
    pr->Res_last_1 = 0;
    pr->Res_last_2 = 0;
    pr->Kp = 1;
    pr->Kr = 1;
    pr->Omega_c = PI;
    pr->Omega_0 = 100 * PI;
    pr->max = 1000;
    pr->max_inv = 1 / pr->max;
}

/**
 * @brief Set QPR Transfer Fun
 * 
 * @param pr 
 * @param Kp 
 * @param Kr 
 * @param Omega_c 
 * @param Omega_0 
 * @param max 
 */
void PR_Set(PR* pr, float Kp, float Kr, float Omega_c, float Omega_0,float max)
{
    pr->Kp = Kp;
    pr->Kr = Kr;
    pr->Omega_c = Omega_c;
    pr->Omega_0 = Omega_0;
    pr->B0 = 4 * pr->Kp + 4 * T_ISR * (pr->Kp + pr->Kr) * pr->Omega_c + pr->Kp * pr->Omega_0 * pr->Omega_0 * T_ISR * T_ISR;
    pr->B1 = pr->Kp * (2 * pr->Omega_0 * pr->Omega_0 * T_ISR * T_ISR - 8);
    pr->B2 = 4 * pr->Kp - 4 * T_ISR * (pr->Kp + pr->Kr) * pr->Omega_c + pr->Kp * pr->Omega_0 * pr->Omega_0 * T_ISR * T_ISR;
    pr->A0 = 4 + 4 * T_ISR * pr->Omega_c + pr->Omega_0 * pr->Omega_0 * T_ISR * T_ISR;
    pr->A1 = 2 * pr->Omega_0 * pr->Omega_0 * T_ISR * T_ISR - 8;
    pr->A2 = 4 - 4 * T_ISR * pr->Omega_c + pr->Omega_0 * pr->Omega_0 * T_ISR * T_ISR;
    pr->max = max;
    pr->max_inv = 1 / pr->max;
    pr->inv_A0 = 1 / pr->A0;
}

/**
 * @brief Perform QPR Control
 * 
 * @param pr 
 */
void PR_Run(PR* pr)
{
    pr->err = pr->GiveValue - pr->ActuralValue;
    pr->Result = (pr->B0 * pr->err + pr->B1 * pr->e_last_1 + pr->B2 * pr->e_last_2 - pr->A1 * pr->Res_last_1 - pr->A2 * pr->Res_last_2) * pr->inv_A0;

    pr->e_last_2 = pr->e_last_1;
    pr->e_last_1 = pr->err;

    pr->Res_last_2 = pr->Res_last_1;
    pr->Res_last_1 = pr->Result;

    if(pr->Result <= -pr->max)
    {
        pr->Result = -pr->max;
    }

    if(pr->Result >= pr->max)
    {
        pr->Result = pr->max;
    }

    pr->Result = pr->Result * pr->max_inv;
}
