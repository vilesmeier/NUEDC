#include "global.h"
#include "rms.h"

/**
 * @brief init the struct
 * 
 * @param rms_addr struct pointer
 */
void RMS_Init(RMS* rms_addr)
{
    int i = 0;
    for(i = 0 ; i < 400 ; i++)
    {
        rms_addr->data[i] = 0;
    }
    rms_addr -> sample_avg = 0;
    rms_addr -> sample_rms = 0;
    rms_addr -> index      = 0;
    rms_addr -> temp_avg   = 0;
    rms_addr -> temp_rms   = 0;
    rms_addr -> rms_nooffset   = 0;
}

/**
 * @brief calcutate rms value
 * 
 * @param rms_addr struct pointer
 * @param data sample value
 */
void RMS_Run(RMS* rms_addr,float data)
{
    //calculate template data
    rms_addr->temp_avg = rms_addr -> temp_avg - rms_addr->data[rms_addr->index] + data;
    rms_addr->temp_rms = rms_addr ->temp_rms - rms_addr->data[rms_addr->index] * rms_addr->data[rms_addr->index] + data * data;
    
    //push data
    rms_addr->data[rms_addr->index] = data;
    rms_addr->index ++;
    if(rms_addr->index >= 400)
    {
        rms_addr->index = 0;
    }

    //calculate rms/avg value
    rms_addr->sample_rms = sqrtf(rms_addr->temp_rms * 0.0025);
    rms_addr->sample_avg = rms_addr->temp_avg * 0.0025;

    //delete offset
    rms_addr->rms_nooffset = sqrtf(rms_addr->temp_rms * 0.0025 - (rms_addr->sample_avg * rms_addr->sample_avg) );
}

/**
 * @brief Push data into RMS data array
 * 
 * @param rms_addr 
 * @param data 
 */
void RMS_Push(RMS* rms_addr,float data)
{
    rms_addr->data[rms_addr->index] = data;
    rms_addr->index ++;
    if(rms_addr->index >= 400)
    {
        rms_addr->index = 0;
    }
}

/**
 * @brief discrete time delay realized by look-up-table
 * 
 * @param rms_addr struct pointer
 * @param phase    delay phase in rad
 * @return float   value after delay
 */
float RMS_Delay(RMS *rms_addr, float phase)
{
    int index_differ = phase * 200.0f * 0.3183098861837f;
    if(rms_addr->index < index_differ)
    {
        return rms_addr->data[400 + rms_addr->index - index_differ];
    }
    else
    {
        return rms_addr->data[rms_addr->index - index_differ];
    }
}

