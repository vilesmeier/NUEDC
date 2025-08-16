#include "global.h"

/**
 * @brief sturct init
 * 
 * @param v struct pointer
 */
void MPPT_INCC_Init(MPPT_INCC *v)
{
	v->Ipv=0;
	v->Vpv=0;
	v->IpvH=(1.65);
	v->IpvL=0;
	v->VpvH=0;
	v->VpvL=0;
	v->MaxVolt=16.0f;
	v->MinVolt=14.5f;
	v->Stepsize=0.1f;
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
 * @brief MPPT realized by Voltage-based Incremental Conductance
 * 
 * @param v struct pointer
 */
void MPPT_INCC_Run(MPPT_INCC *v)
{
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
			v->VpvOld = v->Vpv;
			v->IpvOld = v->Ipv;
		}
        if(v->VmppOut < v->MinVolt) v->VmppOut = v->MinVolt;
        if(v->VmppOut > v->MaxVolt) v->VmppOut = v->MaxVolt;
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
    v->IpvH=(1.65f);
    v->IpvL=0;
    v->VpvH=0;
    v->VpvL=0;
    v->MaxI=2.0f;
    v->MinI=0.1f;
    v->Stepsize=0.1f;
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
 * @brief MPPT realized by Current-based Incremental Conductance
 * 
 * @param v struct pointer
 */
void MPPT_INCC_I_Run(MPPT_INCC_I *v)
{
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
    v->DeltaPmin=(1.0f);
    v->MaxVolt=(16.0f);
    v->MinVolt=14.5f;
    v->Stepsize=(0.01f);
    v->VmppOut=0;
    v->DeltaP=0;
    v->PanelPower=0;
    v->PanelPower_Prev=0;
    v->mppt_enable=1;
    v->mppt_first=1;
}

/**
 * @brief MPPT realized by Perturb and Observe
 * 
 * @param v 
 */
void MPPT_PNO_Run(MPPT_PNO *v)
{
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
        if(v->VmppOut < v->MinVolt) v->VmppOut = v->MinVolt;
        if(v->VmppOut > v->MaxVolt) v->VmppOut = v->MaxVolt;
    }
}
