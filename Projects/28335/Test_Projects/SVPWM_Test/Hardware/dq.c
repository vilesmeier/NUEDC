#include "global.h"
#include "dq.h"

/**
 * @brief Init Struct
 * 
 * @param dq struct pointer
 */
void DQ_Init(DQ* dq)
{
    dq->a = 0;
    dq->b = 0;
    dq->c = 0;
    dq->alpha = 0;
    dq->beta = 0;
    dq->sin = 0;
    dq->cos = 0;
    dq->d = 0;
    dq->q = 0;
}

/**
 * @brief Park Transform
 * 
 * @param dq struct pointer
 */
void Park_Run(DQ* dq)
{
    dq->d =  dq->alpha * dq->cos + dq->beta * dq->sin;

    dq->q = -dq->alpha * dq->sin + dq->beta * dq->cos;
}

/**
 * @brief Inverse Park Transform
 * 
 * @param dq struct pointer
 */
void Inverse_Park_Run(DQ* dq)
{
    dq->alpha = dq->d * dq->cos - dq->q * dq->sin;

    dq->beta  = dq->d * dq->sin + dq->q * dq->cos;
}

/**
 * @brief Clark Transform
 * 
 * @param dq struct pointer
 */
void Clark_Run(DQ* dq)
{
    dq->alpha = 0.666667f * dq->a - 0.333333f * dq->b - 0.333333f * dq->c;
    dq->beta  = 0.577350f * dq->b - 0.577350f * dq->c;
}

/**
 * @brief Inverse Clark Transform
 * 
 * @param dq struct pointer
 */
void Inverse_Clark_Run(DQ* dq)
{
    dq->a = dq->alpha;
    dq->b =  0.866025f * dq->beta - 0.5f * dq->alpha;
    dq->c = -0.866025f * dq->beta - 0.5f * dq->alpha;
}

/**
 * @brief set electrical angle in rad
 * 
 * @param dq    struct pointer
 * @param angle electrical angle in rad
 */
void DQ_Set_Angle(DQ *dq, float angle)
{
    dq->cos = cosf(angle);
    dq->sin = sinf(angle);
}

/**
 * @brief set electical angle by sin and cos
 * 
 * @param dq    struct pointer
 * @param sin   sin value
 * @param cos   cos value
 */
void DQ_Set_Sincos(DQ *dq, float sin, float cos)
{
    dq->cos = cos;
    dq->sin = sin;
}
