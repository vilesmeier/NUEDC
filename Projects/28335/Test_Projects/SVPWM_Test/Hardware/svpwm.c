#include "global.h"
#include "svpwm.h"

/*
    reference article: 
    https://blog.csdn.net/qlexcel/article/details/74787619
    https://blog.csdn.net/mftang/article/details/145790065
*/

/**
 * @brief Struct Init
 * 
 * @param svpwm struct pointer
 */
void SVPWM_Init(SVPWM* svpwm)
{
    svpwm->u1 = 0;
    svpwm->u2 = 0;
    svpwm->u3 = 0;
    svpwm->Sector = 0;
    svpwm->TBPRD = 0;
    svpwm->comp1 = 0;
    svpwm->comp2 = 0;
    svpwm->comp3 = 0;
    svpwm->duty1 = 0;
    svpwm->duty2 = 0;
    svpwm->duty3 = 0;
    svpwm->Mod_Rate = 1;
    svpwm->t1 = 0;
    svpwm->t2 = 0;
    svpwm->t3 = 0;
    svpwm->t4 = 0;
    svpwm->t5 = 0;
    svpwm->t6 = 0;
}

/**
 * @brief caculate modulate rate for SVPWM ,7 state mode
 * 
 * @param svpwm     struct pointer
 * @param tbprd     PWM TBPRD register value
 */
void SVPWM_Set(SVPWM* svpwm, Uint16 tbprd)
{
    svpwm->TBPRD = tbprd;
}

/**
 * @brief Calculate Parameters for SVPWM Modulate
 * 
 * @param svpwm     Struct pointer
 * @param Ualpha    Alpha axis value in alpha-beta axis
 * @param Ubeta     Beta axis value in alpha-beta axis
 */
void SVPWM_Parameter_Cal(SVPWM* svpwm, float Ualpha, float Ubeta)
{
    svpwm->u1 = Ubeta;
    svpwm->u2 = (( 1.732050808f) * Ualpha - Ubeta) * 0.5f;
    svpwm->u3 = ((-1.732050808f) * Ualpha - Ubeta) * 0.5f;
}

/**
 * @brief Calculate the Sector 
 * 
 * @param svpwm     Struct pointer
 * @param Ualpha    Alpha axis value in alpha-beta axis
 * @param Ubeta     Beta axis value in alpha-beta axis
 */
void SVPWM_Sector_Cal(SVPWM* svpwm, float Ualpha, float Ubeta)
{
    //Calculate Parameters for SVPWM Modulate
    SVPWM_Parameter_Cal(svpwm, Ualpha, Ubeta);

    svpwm->Sector = 0;

    //Determain the Sector
    if(svpwm->u1 > 0.0f)
    {
        svpwm->Sector = svpwm->Sector + 1;
    }
    if(svpwm->u2 > 0.0f)
    {
        svpwm->Sector = svpwm->Sector + 2;
    }
    if(svpwm->u3 > 0.0f)
    {
        svpwm->Sector = svpwm->Sector + 4;
    }
}

/**
 * @brief update COMPA value of epwm1/epwm2/epwm3
 * 
 * @param svpwm struct pointer
 */
void SVPWM_Duty_Update(SVPWM* svpwm)
{
    //get comp value
    svpwm->comp1 = (svpwm->duty1 + 0.5f) * svpwm->TBPRD;
    svpwm->comp2 = (svpwm->duty2 + 0.5f) * svpwm->TBPRD;
    svpwm->comp3 = (svpwm->duty3 + 0.5f) * svpwm->TBPRD;

    //load ePWM registers
    EPwm1Regs.CMPA.half.CMPA = svpwm->comp1;
    EPwm2Regs.CMPA.half.CMPA = svpwm->comp2;
    EPwm3Regs.CMPA.half.CMPA = svpwm->comp3;
}

/**
 * @brief SVPWM Modulate
 * 
 * @param svpwm  Struct pointer
 * @param Ualpha Alpha axis value in alpha-beta axis
 * @param Ubeta  Beta axis value in alpha-beta axis
 */
void SVPWM_Run(SVPWM* svpwm, float Ualpha, float Ubeta)
{
    //get parameter and sector
    SVPWM_Sector_Cal(svpwm, Ualpha, Ubeta);

    //SVPWM modulate
    switch(svpwm->Sector)
    {
        //Sector Tx Ty
        case 1: //Sector 2  110 010
            svpwm->t6 = -svpwm->u2;
            svpwm->t2 = -svpwm->u3;
            if((svpwm->t6 + svpwm->t2) > 1)
            {
                svpwm->Mod_Rate = 1.0f/(svpwm->t2 + svpwm->t6);
                svpwm->t2 = svpwm->Mod_Rate * svpwm->t2;
                svpwm->t6 = svpwm->Mod_Rate * svpwm->t6;
            }
            svpwm->duty2 = 0.25f * (1 - svpwm->t2 - svpwm->t6);
            svpwm->duty1 = svpwm->duty2 + 0.5f * svpwm->t6;
            svpwm->duty3 = svpwm->duty1 + 0.5f * svpwm->t2;
            break;

        case 2: //Sector 6  101 100
            svpwm->t5 = -svpwm->u3;
            svpwm->t4 = -svpwm->u1;
            if((svpwm->t5 + svpwm-> t4) > 1)
            {
                svpwm->Mod_Rate = 1.0f/(svpwm->t5 + svpwm-> t4);
                svpwm->t5 = svpwm->Mod_Rate * svpwm->t5;
                svpwm->t4 = svpwm->Mod_Rate * svpwm->t4;
            }
            svpwm->duty1 = 0.25f * (1 - svpwm->t4 - svpwm->t5);
            svpwm->duty3 = svpwm->duty1 + 0.5f * svpwm->t5;
            svpwm->duty2 = svpwm->duty3 + 0.5f * svpwm->t4;
            break;

        case 3: //Sector 1  100 110
            svpwm->t4 = svpwm->u2;
            svpwm->t6 = svpwm->u1;
            if((svpwm->t4 + svpwm->t6) > 1)
            {
                svpwm->Mod_Rate = 1.0f/(svpwm->t4 + svpwm->t6);
                svpwm->t4 = svpwm->Mod_Rate * svpwm->t4;
                svpwm->t6 = svpwm->Mod_Rate * svpwm->t6;
            }
            svpwm->duty1 = 0.25f *(1 - svpwm->t4 - svpwm->t6);
            svpwm->duty2 = svpwm->duty1 + 0.5f * svpwm->t4;
            svpwm->duty3 = svpwm->duty2 + 0.5f * svpwm->t6;
            break;

        case 4: //sector 4  011 001
            svpwm->t3 = -svpwm->u1;
            svpwm->t1 = -svpwm->u2;
            if((svpwm->t3 + svpwm->t1) > 1)
            {
                svpwm->Mod_Rate = 1.0f/(svpwm->t1 + svpwm->t3);
                svpwm->t1 = svpwm->Mod_Rate * svpwm->t1;
                svpwm->t3 = svpwm->Mod_Rate * svpwm->t3;
            }
            svpwm->duty3 = 0.25f * (1 - svpwm->t1 - svpwm->t3);
            svpwm->duty2 = svpwm->duty3 + 0.5f * svpwm->t3;
            svpwm->duty1 = svpwm->duty2 + 0.5f * svpwm->t1;
            break;

        case 5: //Sector 3  010 011
            svpwm->t2 = svpwm->u1;
            svpwm->t3 = svpwm->u3;
            if((svpwm->t2 + svpwm->t3) > 1)
            {
                svpwm->Mod_Rate = 1.0f/(svpwm->t2 + svpwm->t3);
                svpwm->t2 = svpwm->Mod_Rate * svpwm->t2;
                svpwm->t3 = svpwm->Mod_Rate * svpwm->t3;
            }
            svpwm->duty2 = 0.25f * (1 - svpwm->t2 - svpwm->t3);
            svpwm->duty3 = svpwm->duty2 + 0.5f * svpwm->t2;
            svpwm->duty1 = svpwm->duty3 + 0.5f * svpwm->t3;
            break;

        case 6: //Sector 5  001 101
            svpwm->t1 = svpwm->u3;
            svpwm->t5 = svpwm->u2;
            if((svpwm->t5 + svpwm->t1) > 1)
            {
                svpwm->Mod_Rate = 1.0f/(svpwm->t1 + svpwm->t5);
                svpwm->t1 = svpwm->Mod_Rate * svpwm->t1;
                svpwm->t5 = svpwm->Mod_Rate * svpwm->t5;
            }
            svpwm->duty3 = 0.25f * (1 - svpwm->t1 - svpwm->t5);
            svpwm->duty1 = svpwm->duty3 + 0.5f * svpwm->t1;
            svpwm->duty2 = svpwm->duty1 + 0.5f * svpwm->t5;
            break;

        default:
            svpwm->duty1 = 0.5f;
            svpwm->duty2 = 0.5f;
            svpwm->duty3 = 0.5f;
            break;
    }

    //update comp value for epwm1/epwm2/epwm3
    SVPWM_Duty_Update(svpwm);
}
