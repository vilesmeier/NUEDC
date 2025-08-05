#include "vsg.h"
#include "global.h"

/**
 * @brief init struct 
 * 
 * @param vsg struct pointer
 */
void VSG_Init(VSG *vsg)
{
    //vsg Init
    vsg->clamp_delta_w_lower = 0;
    vsg->clamp_delta_w_upper = 0;
    vsg->delta_t = 0;
    vsg->delta_w = 0;
    vsg->D = 0;
    vsg->fsw = 0;
    vsg->I_ref = 0;
    vsg->J = 0;
    vsg->P = 0;
    vsg->P_ref = 0;
    vsg->pw_A0 = 0;
    vsg->pw_B0 = 0;
    vsg->pw_B1 = 0;
    vsg->pw_err_last = 0;
    vsg->pw_result_last = 0;
    vsg->Q = 0;
    vsg->Q_ref = 0;
    vsg->T_input = 0;
    vsg->U_grid = 0;
    vsg->U_ref = 0;
    vsg->U_rms_nom = 0;
    vsg->Uamp = 0;
    vsg->w = 0;
    vsg->w0 = 0;
    vsg->w_last = 0;
    vsg->wt = 0;

    // Vir init
    VIR_Z_Init(&(vsg->vir));
}

/**
 * @brief set VSG parameter
 *  
 * @param vsg           struct pointer
 * @param fsw           called frequency
 * @param w0            nominal w
 * @param u_rms_nom     nominal output voltage
 * @param Pset          target P in W
 * @param Qset          target Q in Var
 * @param D             VSG parameter
 * @param J             VSG parameter
 * @param L             filter inductor value
 * @param r             filter inductor serial resistor
 * @param upper         clamp upper limit for delta_w
 * @param lower         clamp lower limit for delta_w
 */
void VSG_Set(VSG *vsg, float fsw, float w0, float u_rms_nom, float Pset, float Qset, float D, float J, float L, float r, float upper, float lower)
{
    vsg->fsw = fsw;
    vsg->delta_t = (1.0f) / fsw;
    if (upper > lower)
    {
        vsg->clamp_delta_w_lower = lower;
        vsg->clamp_delta_w_upper = upper;
    }
    else 
    {
        vsg->clamp_delta_w_lower = upper;
        vsg->clamp_delta_w_upper = lower;
    }
    vsg->D = D;
    vsg->J = J;
    vsg->P_ref = Pset;
    vsg->Q_ref = Qset;
    vsg->w0 = w0;
    vsg->U_rms_nom = u_rms_nom;
    vsg->pw_A0 = -(vsg->D * vsg->delta_t - 2.0f * vsg->J) / (2 * vsg->J + vsg->D * vsg->delta_t);
    vsg->pw_B0 = vsg->delta_t / (2 * vsg->J + vsg->D * vsg->delta_t);
    vsg->pw_B1 = vsg->delta_t / (2 * vsg->J + vsg->D * vsg->delta_t);

    VIR_Z_Set(&(vsg->vir), fsw, L, r);
}

/**
 * @brief perform VSG control
 * 
 * @param vsg struct pointer
 * @param P     P in W
 * @param Q     Q in Var
 * @param Ugrid grid voltage value
 */
void VSG_Run(VSG *vsg, float P, float Q, float Ugrid)
{
    // data input
    vsg->U_grid = Ugrid;
    vsg->P = P;
    vsg->Q = Q;

    // 计算转矩差
    vsg->T_input = (vsg->P_ref - vsg->P) / vsg->w;

    // 转矩方程，Tustin离散化
    vsg->delta_w = vsg->pw_A0 * vsg->pw_result_last + vsg->pw_B0 * vsg->T_input + vsg->pw_B1 * vsg->pw_err_last;

    // clamp for delta_w
    if (vsg->delta_w > vsg->clamp_delta_w_upper)
    {
        vsg->delta_w = vsg->clamp_delta_w_upper;
    }
    else if (vsg->delta_w < vsg->clamp_delta_w_lower) 
    {
        vsg->delta_w = vsg->clamp_delta_w_lower;
    }
    else
    {
        vsg->delta_w = vsg->delta_w;
    }

    // update stroge value
    vsg->pw_err_last = vsg->T_input;
    vsg->pw_result_last = vsg->delta_w;

    // get actual output w
    vsg->w = vsg->w0 + vsg->delta_w;

    // integral w to get output phase
    vsg->wt = vsg->wt + vsg->delta_t * vsg->w;
    if(vsg->wt > 2*PI)
    {
        vsg->wt = vsg->wt - 2*PI;
    }
    if(vsg->wt < 0)
    {
        vsg->wt = vsg->wt + 2*PI;
    }

    vsg->w_last = vsg->w;

    // get voltage amplitude
    vsg->Uamp = (vsg->Q_ref - vsg->Q) * 0.025f + vsg->U_rms_nom * 1.414f;

    // get voltage reference signal
    vsg->U_ref = sinf(vsg->wt) * vsg->Uamp;

    // Virtual Impedance, get current ref sigfnal
    VIR_Z_Run(&(vsg->vir), vsg->U_ref - vsg->U_grid);

    //get ref current value
    vsg->I_ref = vsg->vir.output;
}

/**
 * @brief Sync the phase output for VSG
 * 
 * @param vsg struct pointer
 */
void VSG_Pre_Sync(VSG *vsg)
{
    vsg->wt = 0.0f;
}

/**
 * @brief init struct
 * 
 * @param vir struct pointer
 */
void VIR_Z_Init(VIR_Z *vir)
{
    vir->A0 = 0;
    vir->B0 = 0;
    vir->B1 = 0;
    vir->delta_t = 0;
    vir->fsw = 0;
    vir->input = 0;
    vir->input_last = 0;
    vir->L = 0;
    vir->r = 0;
    vir->output = 0;
    vir->output_last = 0;
}

/**
 * @brief Set parameter for virtual impendance
 * 
 * @param vir struct pointer    
 * @param fsw switch frequency
 * @param L   inductor value
 * @param r   serial impendance
 */
void VIR_Z_Set(VIR_Z *vir, float fsw, float L, float r)
{
    vir->fsw = fsw;
    vir->delta_t = (1.0f) / fsw;
    vir->L = L;
    vir->r = r;
    vir->A0 = -(vir->r * vir->delta_t - vir->L * 2.0f) / (vir->r * vir->delta_t + vir->L * 2.0f);
    vir->B0 = vir->delta_t / (vir->r * vir->delta_t + vir->L * 2.0f);
    vir->B1 = vir->B0;
}

/**
 * @brief perform virtual impedace 
 * 
 * @param vir       struct pointer
 * @param input     input voltage on inductor
 */
void VIR_Z_Run(VIR_Z *vir, float input)
{
    vir->input = input;

    // get output 
    vir->output = vir->A0 * vir->output_last + vir->B0 * vir->input + vir->B1 * vir->input_last;

    //uodate storge
    vir->input_last = vir->input;
    vir->output_last = vir->output_last;
}
