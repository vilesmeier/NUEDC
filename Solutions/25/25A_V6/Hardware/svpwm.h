
#ifndef SVPWM_H_
#define SVPWM_H_

typedef struct SVPWM
{
    float Ua;
    float Ub;
    float Uc;
    float max;
    float min;
    float offset;
}SVPWM;

void SVPWM_Init(SVPWM* svpwm);
void SVPWM_Run(SVPWM* svpwm, float Ua, float Uc);


#endif /* SVPWM_H_ */
