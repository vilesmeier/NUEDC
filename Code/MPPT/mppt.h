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

    //set parameters
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
    // input data
    float  Ipv;
    float  Vpv;

    // set parameter
    float  Stepsize;

    //output clamp
    float  MaxI;
    float  MinI;
    
    //output data
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
    // input data
    float  Ipv;
    float  Vpv;

    // set parameter
    float  DeltaPmin;
    float  Stepsize;
    
    //output clamp
    float  MaxVolt;
    float  MinVolt;

    // output data
    float  VmppOut;

    // internal variable
    float  DeltaP;
    float  PanelPower;
    float  PanelPower_Prev;
    Uint8    mppt_enable;
    Uint8    mppt_first;
} MPPT_PNO;


/**
 * @brief function declarations
 * 
 */

//function for Voltage-based Incremental Conductance
void MPPT_INCC_Init(MPPT_INCC *v);
void MPPT_INCC_Set(MPPT_INCC *v, float step, float max, float min);
void MPPT_INCC_Run(MPPT_INCC *v, float Vpv, float Ipv);

//function for Current-based Incremental Conductance
void MPPT_INCC_I_Init(MPPT_INCC_I *v);
void MPPT_INCC_I_Set(MPPT_INCC_I *v, float step, float max, float min);
void MPPT_INCC_I_Run(MPPT_INCC_I *v, float Vpv, float Ipv);

//function for Perturb and Observe
void MPPT_PNO_Init(MPPT_PNO *v);
void MPPT_PNO_Set(MPPT_PNO *v, float step, float max, float min, float delta_p_min);
void MPPT_PNO_Run(MPPT_PNO *v, float Vpv, float Ipv);

#endif
