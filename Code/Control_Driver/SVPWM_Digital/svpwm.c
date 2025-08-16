#include "global.h"

/**
 * @brief init struct
 * 
 * @param svpwm struct pointer
 */
void SVPWM_Init(SVPWM* svpwm)
{
    svpwm->Ua = 0;
    svpwm->Ub = 0;
    svpwm->Uc = 0;
    svpwm->max = 0;
    svpwm->min = 0;
    svpwm->offset = 1.0f;
}

/**
 * @brief calculate SVPWM modulate
 * 
 * @param svpwm     struct pointer
 * @param Ua        Ua in abc, -1 - 1
 * @param Uc        Uc in abc, -1 - 1
 */
void SVPWM_Run(SVPWM* svpwm, float Ua, float Uc)
{
    // transform from 2 axis to 3 axis 
    svpwm->Ua = Ua;
    svpwm->Ub = -Ua-Uc;
    svpwm->Uc = Uc;

    // digital SVPWM modulate
    svpwm->Ua = (svpwm->Ua + svpwm->offset) * 0.5f;
    svpwm->Ub = (svpwm->Ub + svpwm->offset) * 0.5f;
    svpwm->Uc = (svpwm->Uc + svpwm->offset) * 0.5f;

    svpwm->max = (svpwm->Ua > svpwm->Ub)?svpwm->Ua:svpwm->Ub;
    svpwm->max = (svpwm->max > svpwm->Uc)?svpwm->max:svpwm->Uc;
    svpwm->min = (svpwm->Ua < svpwm->Ub)?svpwm->Ua:svpwm->Ub;
    svpwm->min = (svpwm->min < svpwm->Uc)?svpwm->min:svpwm->Uc;

    svpwm->Ua = svpwm->Ua + (svpwm->offset-svpwm->max-svpwm->min)*0.5f;
    svpwm->Ub = svpwm->Ub + (svpwm->offset-svpwm->max-svpwm->min)*0.5f;
    svpwm->Uc = svpwm->Uc + (svpwm->offset-svpwm->max-svpwm->min)*0.5f;
}
