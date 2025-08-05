#include "global.h"
#include "power.h"


/**
 * @brief struct init
 * 
 * @param v struct pointer
 */
void POWER_Init(POWER *v)
{
    v->vRms=0;
    v->vAvg=0;
    v->vEma=0;
    v->acFreq=0;
    v->iRms=0;
    v->pRms=0;
    v->vaRms=0;
    v->varRms=0;
    v->powerFactor=0;
    v->zcd=0;
    v->vSum=0;
    v->vSqrSum=0;
    v->iSqrSum=0;
    v->pSum=0;
    v->vNorm=0;
    v->iNorm=0;
    v->prevSign=0;
    v->currSign=0;
    v->nSamples=0;
    v->nSamplesMin = 0;
    v->nSamplesMax = 0;
    v->inverse_nSamples=0;
    v->sqrt_inverse_nSamples=0;
    v->jitterCount=0;
    v->emaFilterMultiplier=0;

}

/**
 * @brief set power analyzer parameter
 * 
 * @param v             struct pointer
 * @param isrFrequency  call frequency
 * @param threshold     zero pass limit
 * @param gridMaxFreq   maximum grid frequency
 * @param gridMinFreq   minimum grid frequency
 */
void POWER_Set(POWER *v, float isrFrequency, float threshold, float gridMaxFreq, float gridMinFreq)
{
    if(gridMaxFreq < gridMinFreq)
    {
        v->nSamplesMax=isrFrequency/gridMaxFreq;
        v->nSamplesMin=isrFrequency/gridMinFreq;
    }
    else
    {
        v->nSamplesMax=isrFrequency/gridMinFreq;
        v->nSamplesMin=isrFrequency/gridMaxFreq;
    }
    v->sampleFreq = (float)(isrFrequency);
    v->threshold = (float)(threshold);
    v->emaFilterMultiplier=2.0f/isrFrequency;
}

/**
 * @brief calculate power,when RMS and PF are unknown
 * 
 * @param v     struct pointer
 * @param vol   voltage in V
 * @param cur   current in A
 */
void POWER_Run(POWER *v, float vol, float cur)
{
    v->v = vol;
    v->i = cur;
    v->vNorm = fabsf(v->v);
    v->iNorm = fabsf(v->i);
    v->currSign = ( v->v > v->threshold) ? 1 : 0;
    v->nSamples++;
    v->vSum = v->vSum+v->vNorm;
    v->vSqrSum = v->vSqrSum+(v->vNorm*v->vNorm);
    v->vEma = v->vEma+(v->emaFilterMultiplier*(v->vNorm - v->vEma));
    v->iSqrSum = v->iSqrSum+(v->iNorm*v->iNorm);
    v->pSum = v->pSum+(v->i*v->v);
    v->zcd=0;

    // when positive zero pass is detected, indecating a full sine wave period
    if((v->prevSign != v->currSign) && (v->currSign == 1))
    {
        //
        // check if the nSamples are in the ball park of a real frequency
        // that can be on the grid, this is done by comparing the nSamples
        // with the max value and min value it can be for the 
        // AC Grid Connection these Max and Min are initialized by the 
        // user in the code
        //
        if(v->nSamplesMin < v->nSamples )
        {
            v->zcd=1;
            v->inverse_nSamples = (1.0f)/(v->nSamples);
            v->sqrt_inverse_nSamples = sqrtf(v->inverse_nSamples);
            v->vAvg  = (v->vSum*v->inverse_nSamples);
            v->vRms  = sqrtf(v->vSqrSum)*v->sqrt_inverse_nSamples;
            v->iRms  = sqrtf(v->iSqrSum)*v->sqrt_inverse_nSamples;
            v->pRms  = v->pSum*v->inverse_nSamples;
            v->vaRms = v->vRms*v->iRms;
            v->varRms = v->vaRms * v->vaRms - v->pRms * v->pRms;
            if(v->varRms <= 0 )
            {
                v->varRms = 0;
            }
            else
            {
                v->varRms = sqrtf(v->varRms);
            }
            v->acFreq = (v->sampleFreq*v->inverse_nSamples);
            v->powerFactor=v->pRms/v->vaRms;

            v->jitterCount=0;
            v->nSamples=0;
            v->vSum=0;
            v->vSqrSum=0;
            v->iSqrSum=0;
            v->pSum =0;
        }
        else
        {
            //
            // otherwise it may be jitter ignore this reading
            // but count the number of jitters you are getting
            // but do not count to infinity as then when the grid comes back
            // it will take too much time to wind down the jitter count
            //
            if(v->jitterCount<30)
            {
                v->jitterCount++;
            }
            v->nSamples=0;
        }
    }

    if(v->nSamples>v->nSamplesMax || v->jitterCount>20)
    {
        //
        // most certainly the AC voltage is not present
        //
        v->vRms = 0;
        v->vAvg = 0;
        v->vEma = 0;
        v->acFreq=0;
        v->iRms = 0;
        v->pRms = 0;
        v->vaRms =0;
        v->powerFactor=0;

        v->zcd=0;
        v->vSum=0;
        v->vSqrSum=0;
        v->iSqrSum=0;
        v->pSum=0;
        v->nSamples=0;
        v->jitterCount=0;
    }
    v->prevSign = v->currSign;
}

/**
 * @brief calculate power when parameters are known
 * 
 * @param v         struct pointer
 * @param vrms      RMS value of voltage in V
 * @param irms      RMS value of current in A
 * @param pf        power factor
 */
void POWER_RMS_Run(POWER *v,float vrms, float irms, float pf )
{
    v->vRms = vrms;
    v->iRms = irms;
    v->powerFactor = pf;
    v->vaRms = vrms * irms;
    v->pRms = vrms * irms * pf;
    v->varRms = sqrtf(v->vaRms * v->vaRms - v->pRms * v->pRms);
}



