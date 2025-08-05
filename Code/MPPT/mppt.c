#include "global.h"
#include "mppt.h"

/**
 * @brief sturct init
 * 
 * @param v struct pointer
 */
void MPPT_INCC_Init(MPPT_INCC *v)
{
	v->Ipv=0;
	v->Vpv=0;
	v->MaxVolt=0;
	v->MinVolt=0;
	v->Stepsize=0;
	v->VmppOut=0;
	v->Cond=0;
	v->IncCond=0;
	v->DeltaV=0;
	v->DeltaI=0;
	v->VpvOld=0;
	v->IpvOld=0;
	v->StepFirst=(0.02);
	v->mppt_enable=1;
	v->mppt_first=1;
}

/**
 * @brief set struct parameters
 * 
 * @param v     struct pointer
 * @param step  step length
 * @param max   upper limit for output
 * @param min   lower limit for input
 */
void MPPT_INCC_Set(MPPT_INCC *v, float step, float max, float min)
{
    if(max > min)
    {
        v->MaxVolt=max;
        v->MinVolt=min;
    }
    else
    {
        v->MaxVolt=min;
        v->MinVolt=max;
    }
	v->Stepsize=step;
}

/**
 * @brief MPPT realized by Voltage-based Incremental Conductance
 * 
 * @param v     struct pointer
 * @param Vpv   pv output voltage
 * @param Ipv   pv output current
 */
void MPPT_INCC_Run(MPPT_INCC *v, float Vpv, float Ipv)
{
    v->Vpv = Vpv;
    v->Ipv = Ipv;
	if (v->mppt_enable==1)
	{	/*MPPT mode enable*/
		if (v->mppt_first == 1)
		{
			v->VmppOut= v->Vpv - v->StepFirst;
	    	v->VpvOld=v->Vpv;
	    	v->IpvOld=v->Ipv;
			v->mppt_first=0;
		}
		else
		{
			v->DeltaV = v->Vpv-v->VpvOld ; 				/* PV voltage change */
	    	v->DeltaI = v->Ipv-v->IpvOld ; 				/* PV current change */
	    	if(v->DeltaV == 0)    						/* DeltaV=0 */
			{
				if(v->DeltaI !=0 )    					/* dI=0 operating at MPP */
				{
					if ( v->DeltaI > 0 )  				/* climbing towards MPP */
					{
						v->VmppOut=v->Vpv+v->Stepsize;	/* Increment Vref */
					}		
					else
					{
						v->VmppOut=v->Vpv-v->Stepsize;	/* Decrement Vref */
					}
				}
			}
	    	else
			{
                v->Cond = v->Ipv*__einvf32(v->Vpv);						/*Cond =-(v->Ipv<<10)/v->Vpv;*/
                v->IncCond = v->DeltaI*__einvf32(v->DeltaV);			/*IncCond =(DeltaI<<10)/DeltaV;*/
	       		if (v->IncCond != v->Cond)								/* Operating at MPP */
				{
					if (v->IncCond > (-v->Cond))						/* left of MPP */
					{
						v->VmppOut=v->Vpv+v->Stepsize;					/* change Vref */
					}
					else												/* Right of MPP */
					{
						v->VmppOut=v->Vpv-v->Stepsize;					/* change Vref */
					}
	        	}
			}
            if(v->VmppOut < v->MinVolt)
            {
                v->VmppOut = v->MinVolt;
            }
            else if(v->VmppOut > v->MaxVolt) 
            {
                v->VmppOut = v->MaxVolt;
            }
            else
            {
                v->VmppOut = v->VmppOut;
            }
			v->VpvOld = v->Vpv;
			v->IpvOld = v->Ipv;
		}
	}
}

/**
 * @brief struct init 
 * 
 * @param v struct pointer
 */
void MPPT_INCC_I_Init(MPPT_INCC_I *v)
{
    v->Ipv=0;
    v->Vpv=0;
    v->MaxI=0;
    v->MinI=0;
    v->Stepsize=0;
    v->ImppOut=0;
    v->Cond=0;
    v->IncCond=0;
    v->DeltaV=0;
    v->DeltaI=0;
    v->VpvOld=0;
    v->IpvOld=0;
    v->StepFirst=(0.02);
    v->mppt_enable=1;
    v->mppt_first=1;
}

/**
 * @brief config struct
 * 
 * @param v         struct pointer
 * @param step      step length
 * @param max       max output 
 * @param min       min output
 */
void MPPT_INCC_I_Set(MPPT_INCC_I *v, float step, float max, float min)
{
    if(max > min)
    {
        v->MaxI =max;
        v->MinI=min;
    }
    else
    {
        v->MaxI=min;
        v->MinI=max;
    }
	v->Stepsize=step;
}

/**
 * @brief realized by Current-based Incremental Conductance
 * 
 * @param v     struct pointer
 * @param Vpv   pv output voltage
 * @param Ipv   pv output current
 */
void MPPT_INCC_I_Run(MPPT_INCC_I *v, float Vpv, float Ipv)
{
    v->Vpv = Vpv;
    v->Ipv = Ipv;
    if (v->mppt_enable==1)
    {   /*MPPT mode enable*/
        if (v->mppt_first == 1)
        {
            v->ImppOut= v->Ipv + v->StepFirst;
            v->VpvOld=v->Vpv;
            v->IpvOld=v->Ipv;
            v->mppt_first=0;
        }
        else
        {
            v->DeltaV = v->Vpv-v->VpvOld ;              /* PV voltage change */
            v->DeltaI = v->Ipv-v->IpvOld ;              /* PV current change */
            if(v->DeltaV >= 0)                          /* DeltaV=0 */
            {
                if(v->DeltaI !=0 )                      /* dI=0 operating at MPP */
                {
                    if ( v->DeltaI > 0 )                /* climbing towards MPP */
                    {
                        v->ImppOut=v->Ipv-v->Stepsize;  /* Increment Vref */
                    }
                    else
                    {
                        v->ImppOut=v->Ipv+v->Stepsize;  /* Decrement Vref */
                    }
                }
            }
            else
            {
                v->Cond = v->Ipv*__einvf32(v->Vpv);             /*Cond =-(v->Ipv<<10)/v->Vpv;*/
                v->IncCond = v->DeltaI*__einvf32(v->DeltaV);    /*IncCond =(DeltaI<<10)/DeltaV;*/
                if (v->IncCond != v->Cond)                      /* Operating at MPP */
                {
                    if (v->IncCond > (-v->Cond))                /* left of MPP */
                    {
                        v->ImppOut=v->Ipv-v->Stepsize;          /* change Vref */
                    }
                    else                                        /* Right of MPP */
                    {
                        v->ImppOut=v->Ipv+v->Stepsize;          /* change Vref */
                    }
                }
            }
            v->ImppOut=(v->ImppOut<v->MinI)?v->MinI: v->ImppOut;     /*Min. MPP Current */
            v->ImppOut=(v->ImppOut>v->MaxI)?v->MaxI: v->ImppOut;    /*Max. MPP Current    */

            v->VpvOld = v->Vpv;
            v->IpvOld = v->Ipv;
        }
    }
}

/**
 * @brief struct init
 * 
 * @param v struct pointer
 */
void MPPT_PNO_Init(MPPT_PNO *v)
{
    v->Ipv=0;
    v->Vpv=0;
    v->DeltaPmin=(0.00001);
    v->MaxVolt=(0.9);
    v->MinVolt=0;
    v->Stepsize=(0.002);
    v->VmppOut=0;
    v->DeltaP=0;
    v->PanelPower=0;
    v->PanelPower_Prev=0;
    v->mppt_enable=1;
    v->mppt_first=1;
}

/**
 * @brief config struct 
 * 
 * @param v             struct pointer
 * @param step          step length
 * @param max           max output
 * @param min           min output
 * @param delta_p_min   min power differ, less than this value regard as no change
 */
void MPPT_PNO_Set(MPPT_PNO *v, float step, float max, float min, float delta_p_min)
{
    v->DeltaPmin = delta_p_min;
    v->Stepsize = step;
    if(max > min)
    {
        v->MaxVolt =max;
        v->MinVolt=min;
    }
    else
    {
        v->MaxVolt=min;
        v->MinVolt=max;
    }
}

/**
 * @brief MPPT realized by Perturb and Observe
 * 
 * @param v     struct pointer
 * @param Vpv   PV output Voltage 
 * @param Ipv   PV output Current
 */
void MPPT_PNO_Run(MPPT_PNO *v, float Vpv, float Ipv)
{
    v->Vpv = Vpv;
    v->Ipv = Ipv;
    if (v->mppt_enable==1)
    {
        if (v->mppt_first == 1)
        {
            v->VmppOut= v->Vpv - (0.02);
            v->mppt_first=0;
            v->PanelPower_Prev=v->PanelPower;
        }
        else
        {
            v->PanelPower=(v->Vpv*v->Ipv);
            v->DeltaP=v->PanelPower-v->PanelPower_Prev;
            if (v->DeltaP > v->DeltaPmin)
            {
                v->VmppOut=v->Vpv+v->Stepsize;
            }
            else
            {
                if (v->DeltaP < -v->DeltaPmin)
                {
                    v->Stepsize=-v->Stepsize;
                    v->VmppOut=v->Vpv+v->Stepsize;
                }
            }
            v->PanelPower_Prev = v->PanelPower;
        }
        if(v->VmppOut < v->MinVolt)
        {
            v->VmppOut = v->MinVolt;
        }
        else if(v->VmppOut > v->MaxVolt) 
        {
            v->VmppOut = v->MaxVolt;
        }
        else
        {
            v->VmppOut = v->VmppOut;
        }
    }
}
