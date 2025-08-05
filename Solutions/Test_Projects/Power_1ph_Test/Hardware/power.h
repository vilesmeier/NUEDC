#ifndef POWER_H
#define POWER_H

//struct pointer 
typedef struct 
{
    float v;           //!< Input: Voltage Sine Signal
    float i;           //!< Input Current Signal
    float sampleFreq;  //!< Input: Signal Sampling Freq
    float threshold;   //!< Input: Voltage level corresponding to zero i/p
    float vRms;        //!< Output: RMS Value
    float vAvg;        //!< Output: Average Value
    float vEma;        //!< Output: Exponential Moving Average Value
    float acFreq;      //!< Output: Signal Freq
    float iRms;        //!< Output: RMS Value of current
    float pRms;        //!< Output: RMS Value of input power(P)
    float varRms;      //!< Output: RMS Var(Q)
    float vaRms;       //!< Output: RMS VA(S)
    float powerFactor; //!< Output: powerFactor
    int  zcd;          //!< Output: Zero Cross detected
    
    float vSum;        //!< Internal : running sum for vac calculation over one sine cycles
    float vSqrSum;     //!< Internal : running sum for vacc square calculation over one sine cycle
    float iSqrSum;     //!< Internal : running sum for Iacc_rms calculation over one sine cycle
    float pSum;        //!< Internal : running sum for Pacc_rms calculation over one sine cycle
    float vNorm;       //!< Internal: Normalized value of the input voltage
    float iNorm;       //!< Internal: Normalized value of the input current
    int  prevSign;     //!< Internal: Flag to detect ZCD
    int  currSign;     //!< Internal: Flag to detect ZCD
    int  nSamples;     //!< Internal: No of samples in one cycle of the sine wave
    int  nSamplesMin;  //!< Internal: Lowerbound for no of samples in one sine wave cycle
    int  nSamplesMax;  //!< Internal: Upperbound for no of samples in one sine wave cycle
    float inverse_nSamples; //!< Internal: 1/( No of samples in one cycle of the sine wave)
    float sqrt_inverse_nSamples; //!< Internal: sqrt(1/( No of samples in one cycle of the sine wave))
    int jitterCount; //!< Internal: used to store jitter information due to noise on input
    float emaFilterMultiplier;  //!< Internal: multiplier value used for the exponential moving average filter
} POWER;


void POWER_Init(POWER *v);
void POWER_Set(POWER *v, float isrFrequency, float threshold, float gridMaxFreq, float gridMinFreq);
void POWER_Run(POWER *v, float vol, float cur);




#endif // !POWER_H
