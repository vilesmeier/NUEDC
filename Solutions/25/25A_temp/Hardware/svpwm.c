#include "global.h"

void SVPWM_Init(SVPWM* svpwm)
{
    svpwm->Ua = 0;
    svpwm->Ub = 0;
    svpwm->Uc = 0;
    svpwm->max = 0;
    svpwm->min = 0;
    svpwm->offset = 1.0f;
}

void SVPWM_Run(SVPWM* svpwm, float Ua, float Uc)
{
    svpwm->Ua = Ua;
    svpwm->Ub = -Ua-Uc;
    svpwm->Uc = Uc;

    svpwm->Ua = (svpwm->Ua + svpwm->offset) * 0.5f;
    svpwm->Ub = (svpwm->Ub + svpwm->offset) * 0.5f;
    svpwm->Uc = (svpwm->Uc + svpwm->offset) * 0.5f;

    svpwm->max = (svpwm->Ua > svpwm->Ub)?svpwm->Ua:svpwm->Ub;
    svpwm->max = (svpwm->max > svpwm->Uc)?svpwm->max:svpwm->Uc;
    svpwm->min = (svpwm->Ua < svpwm->Ub)?svpwm->Ua:svpwm->Ub;
    svpwm->min = (svpwm->min < svpwm->Uc)?svpwm->min:svpwm->Uc;

    svpwm->Ua = svpwm->Ua + (svpwm->offset-svpwm->max-svpwm->min)*0.5f;
    svpwm->Ub = svpwm->Ub + (svpwm->offset-svpwm->max-svpwm->min)*0.5f;
    svpwm->Uc = svpwm->Uc + (svpwm->offset-svpwm->max-svpwm->min)*0.5f;\

    EPwm1Regs.CMPA.half.CMPA = 3750 * (svpwm->Ua);
    EPwm2Regs.CMPA.half.CMPA = 3750 * (svpwm->Ub);
    EPwm3Regs.CMPA.half.CMPA = 3750 * (svpwm->Uc);
}







