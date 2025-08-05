
#ifndef DQ_H_
#define DQ_H_

#include <global.h>

typedef struct DQ
{
    float a;
    float b;
    float c;
    float alpha;
    float beta;
    float d;
    float q;
    float sin;
    float cos;
} DQ;

void DQ_Init(DQ *dq);
void Park_Run(DQ *dq);
void Inverse_Park_Run(DQ *dq);
void Clark_Run(DQ *dq);
void Inverse_Clark_Run(DQ *dq);
void DQ_Set_Angle(DQ *dq, float angle);
void DQ_Set_Sincos(DQ *dq, float sin, float cos);

#endif /* DQ_H_ */
