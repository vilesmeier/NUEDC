#ifndef RMS_H
#define RMS_H

typedef struct RMS
{
    float   data[400];
    float   sample_rms;
    float   sample_avg;
    int     index;
    float   temp_rms;
    float   temp_avg;
    float   rms_nooffset;
    float   angle;
}RMS;

void RMS_Init(RMS* rms_addr);
void RMS_Run(RMS* rms_addr,float data);
void RMS_Push(RMS* rms_addr,float data);
float RMS_Delay(RMS *rms_addr, float phase);
float RMS_Current_Value(RMS *rms_addr);

//#pragma CODE_SECTION(RMS_Init, ".TI.ramfunc");
#pragma CODE_SECTION(RMS_Run, ".TI.ramfunc");
#pragma CODE_SECTION(RMS_Push, ".TI.ramfunc");
//#pragma CODE_SECTION(RMS_Delay, ".TI.ramfunc");
//#pragma CODE_SECTION(RMS_Current_Value, ".TI.ramfunc");

#endif // !RMS_H
