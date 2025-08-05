#ifndef MPPT_H
#define MPPT_H


/**
 * @brief struct definition
 * MPPT_INCC    Voltage-based Incremental Conductance
 * MPPT_INCC_I  Current-based Incremental Conductance
 * MPPT_PNO     Perturb and Observe
 */
typedef struct MPPT_INCC
{
	//input values
	float  Ipv;
	float  Vpv;
	float  IpvH;
	float  IpvL;
	float  VpvH;
	float  VpvL;
	float  Stepsize;

	//clamp for input voltage
	float  MaxVolt;
	float  MinVolt;

	//output value
	float  VmppOut;

	// internal variables
	float  Cond;
	float  IncCond;
	float  DeltaV;
	float  DeltaI;
	float  VpvOld;
	float  IpvOld;
	float  StepFirst;

	//state flags
	Uint8 mppt_enable;
	Uint8 mppt_first;

}MPPT_INCC;

typedef struct MPPT_INCC_I
{
    float  Ipv;
    float  Vpv;
    float  IpvH;
    float  IpvL;
    float  VpvH;
    float  VpvL;
    float  MaxI;
    float  MinI;
    float  Stepsize;
    float  ImppOut;
    // internal variables
    float  Cond;
    float  IncCond;
    float  DeltaV;
    float  DeltaI;
    float  VpvOld;
    float  IpvOld;
    float  StepFirst;
    Uint8 mppt_enable;
    Uint8 mppt_first;
} MPPT_INCC_I;

typedef struct MPPT_PNO
{
    float  Ipv;
    float  Vpv;
    float  DeltaPmin;
    float  MaxVolt;
    float  MinVolt;
    float  Stepsize;
    float  VmppOut;
    float  DeltaP;
    float  PanelPower;
    float  PanelPower_Prev;
    Uint8    mppt_enable;
    Uint8    mppt_first;
} MPPT_PNO;


/**
 * @brief functions for MPPT br different method
 * 
 * @param v struct pointer
 */
//function for Voltage-based Incremental Conductance
void MPPT_INCC_Init(MPPT_INCC *v);
void MPPT_INCC_Run(MPPT_INCC *v);

//function for Current-based Incremental Conductance
void MPPT_INCC_I_Init(MPPT_INCC_I *v);
void MPPT_INCC_I_Run(MPPT_INCC_I *v);

//function for Perturb and Observe
void MPPT_PNO_Init(MPPT_PNO *v);
void MPPT_PNO_Run(MPPT_PNO *v);

#endif
