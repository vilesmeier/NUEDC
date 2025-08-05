#include "global.h"

/**
 * @brief phase look loop for three phase grid
 * SRF_3PH   for balanced three phase grid
 * DDSRF_3PH for disbalanced three phase grid
 */


//! \brief              Reset SRF_3PH module
//! \param *spll_obj     The SRF_3PH structure
//! \return None
//!
void SRF_3PH_Init(SRF_3PH *spll_obj)
{
    spll_obj->v_q[0] = (float)(0.0);
    spll_obj->v_q[1] = (float)(0.0);

    spll_obj->ylf[0] = (float)(0.0);
    spll_obj->ylf[1] = (float)(0.0);

    spll_obj->fo = (float)(0.0);

    spll_obj->theta[0] = (float)(0.0);
    spll_obj->theta[1] = (float)(0.0);

}

//! \brief              Initialize SRF_3PH module
//! \param grid_freq    The grid frequency
//! \param delta_t      1/Frequency of calling the PLL routine
//! \param *spll_obj     The SRF_3PH structure
//! \return None
//!
void SRF_3PH_Set(SRF_3PH *spll_obj, float grid_freq, float delta_t)
{
    spll_obj->v_q[0] = (float)(0.0);
    spll_obj->v_q[1] = (float)(0.0);

    spll_obj->ylf[0] = (float)(0.0);
    spll_obj->ylf[1] = (float)(0.0);

    spll_obj->fo = (float)(0.0);
    spll_obj->fn = (float)(grid_freq);

    spll_obj->theta[0] = (float)(0.0);
    spll_obj->theta[1] = (float)(0.0);

    spll_obj->delta_t = (float)delta_t;
}

//! \brief              Run SRF_3PH module
//! \param v_q          Q component of the grid voltage
//! \param *spll_obj     The SRF_3PH structure
//!
void SRF_3PH_Run(SRF_3PH *spll_obj, float v_q)
{
    //
    // Update the spll_obj->v_q[0] with the grid value
    //
    spll_obj->v_q[0] = v_q;

    //
    // Loop Filter
    //
    spll_obj->ylf[0] =  spll_obj->ylf[1]
                     + (spll_obj->lpf_coeff.b0 * spll_obj->v_q[0])
                     + (spll_obj->lpf_coeff.b1 * spll_obj->v_q[1]);
    spll_obj->ylf[1] = spll_obj->ylf[0];
    spll_obj->v_q[1] = spll_obj->v_q[0];

    spll_obj->ylf[0] = (spll_obj->ylf[0] > (float)(200.0)) ?
                                (float)(200.0) : spll_obj->ylf[0];

    //
    // VCO
    //
    spll_obj->fo = spll_obj->fn + spll_obj->ylf[0];

    spll_obj->theta[0] = spll_obj->theta[1] +
                         ((spll_obj->fo * spll_obj->delta_t) *
                          (float)(2.0 * 3.1415926));
    if(spll_obj->theta[0] > (float)(2.0 * 3.1415926))
    {
        spll_obj->theta[0] = spll_obj->theta[0] - (float)(2.0 * 3.1415926);
    }

    spll_obj->theta[1] = spll_obj->theta[0];
}

//! \brief              Reset DDSRF_3PH module
//! \param *spll_obj    The DDSRF_3PH structure
//! \return None
//!
void DDSRF_3PH_Init(DDSRF_3PH *spll_obj)
{
    spll_obj->d_p_decoupl = (float)(0.0);
    spll_obj->d_n_decoupl = (float)(0.0);

    spll_obj->q_p_decoupl = (float)(0.0);
    spll_obj->q_n_decoupl = (float)(0.0);

    spll_obj->d_p_decoupl_lpf = (float)(0.0);
    spll_obj->d_n_decoupl_lpf = (float)(0.0);

    spll_obj->q_p_decoupl_lpf = (float)(0.0);
    spll_obj->q_n_decoupl_lpf = (float)(0.0);

    spll_obj->y[0] = (float)(0.0);
    spll_obj->y[1] = (float)(0.0);

    spll_obj->x[0] = (float)(0.0);
    spll_obj->x[1] = (float)(0.0);

    spll_obj->w[0] = (float)(0.0);
    spll_obj->w[1] = (float)(0.0);

    spll_obj->z[0] = (float)(0.0);
    spll_obj->z[1] = (float)(0.0);

    spll_obj->v_q[0] = (float)(0.0);
    spll_obj->v_q[1] = (float)(0.0);

    spll_obj->ylf[0] = (float)(0.0);
    spll_obj->ylf[1] = (float)(0.0);

    spll_obj->fo = (float)(0.0);

    spll_obj->theta[0] = (float)(0.0);
    spll_obj->theta[1] = (float)(0.0);
}

//! \brief              Initialize DDSRF_3PH module
//! \param grid_freq    The grid frequency
//! \param delta_t      1/Frequency of calling the PLL routine
//! \param k1           parameter
//! \param k2           parameter
//! \param *spll_obj    The DDSRF_3PH structure
//! \return None
//!
void DDSRF_3PH_Set(float grid_freq, float delta_t,
                                    float k1, float k2,
                                    DDSRF_3PH *spll_obj)
{
    spll_obj->d_p_decoupl = (float)(0.0);
    spll_obj->d_n_decoupl = (float)(0.0);

    spll_obj->q_p_decoupl = (float)(0.0);
    spll_obj->q_n_decoupl = (float)(0.0);

    spll_obj->d_p_decoupl_lpf = (float)(0.0);
    spll_obj->d_n_decoupl_lpf = (float)(0.0);

    spll_obj->q_p_decoupl_lpf = (float)(0.0);
    spll_obj->q_n_decoupl_lpf = (float)(0.0);

    spll_obj->y[0] = (float)(0.0);
    spll_obj->y[1] = (float)(0.0);

    spll_obj->x[0] = (float)(0.0);
    spll_obj->x[1] = (float)(0.0);

    spll_obj->w[0] = (float)(0.0);
    spll_obj->w[1] = (float)(0.0);

    spll_obj->z[0] = (float)(0.0);
    spll_obj->z[1] = (float)(0.0);

    spll_obj->k1 = k1;
    spll_obj->k2 = k2;

    spll_obj->v_q[0] = (float)(0.0);
    spll_obj->v_q[1] = (float)(0.0);

    spll_obj->ylf[0] = (float)(0.0);
    spll_obj->ylf[1] = (float)(0.0);

    spll_obj->fo = (float)(0.0);
    spll_obj->fn = (float)(grid_freq);

    spll_obj->theta[0] = (float)(0.0);
    spll_obj->theta[1] = (float)(0.0);

    spll_obj->delta_t = delta_t;
}

//! \brief              Run SRF_3PH module
//! \param *spll_obj    The DDSRF_3PH structure
//! \param d_p          D Positive seq component of the grid voltage
//! \param d_n          D Negative seq component of the grid voltage
//! \param q_p          Q Positive seq component of the grid voltage
//! \param q_n          Q Negative seq component of the grid voltage
//! \return None
//!
void DDSRF_3PH_Run(DDSRF_3PH *spll_obj,
                                      float d_p, float d_n,
                                      float q_p, float q_n)
{
    //
    // before calling this routine run the ABC_DQ0_Pos & Neg run routines
    // pass updated values for d_p,d_n,q_p,q_n
    // and update the cos_2theta and sin_2theta values with the prev angle
    //

    //
    // Decoupling Network
    //
    spll_obj->d_p_decoupl = d_p
                           - (spll_obj->d_n_decoupl_lpf * spll_obj->cos_2theta)
                           - (spll_obj->q_n_decoupl * spll_obj->sin_2theta);
    spll_obj->q_p_decoupl = q_p
                           + (spll_obj->d_n_decoupl_lpf * spll_obj->sin_2theta)
                           - (spll_obj->q_n_decoupl * spll_obj->cos_2theta);

    spll_obj->d_n_decoupl = d_n
                           - (spll_obj->d_p_decoupl_lpf * spll_obj->cos_2theta)
                           + (spll_obj->q_p_decoupl * spll_obj->sin_2theta);
    spll_obj->q_n_decoupl = q_n
                           - (spll_obj->d_p_decoupl_lpf * spll_obj->sin_2theta)
                           - (spll_obj->q_p_decoupl * spll_obj->cos_2theta);

    //
    // Low pass filter
    //

    spll_obj->y[1] = (spll_obj->d_p_decoupl * spll_obj->k1)
                   - (spll_obj->y[0] * spll_obj->k2);
    spll_obj->d_p_decoupl_lpf = spll_obj->y[1] + spll_obj->y[0];
    spll_obj->y[0] = spll_obj->y[1];

    spll_obj->x[1] = (spll_obj->q_p_decoupl * spll_obj->k1)
                  - (spll_obj->x[0] * spll_obj->k2);
    spll_obj->q_p_decoupl_lpf = spll_obj->x[1] + spll_obj->x[0];
    spll_obj->x[0] = spll_obj->x[1];

    spll_obj->w[1] = (spll_obj->d_n_decoupl * spll_obj->k1)
                  - (spll_obj->w[0] * spll_obj->k2);
    spll_obj->d_n_decoupl_lpf = spll_obj->w[1] + spll_obj->w[0];
    spll_obj->w[0] = spll_obj->w[1];

    spll_obj->z[1] = (spll_obj->q_n_decoupl * spll_obj->k1)
                  - (spll_obj->z[0] * spll_obj->k2);
    spll_obj->q_n_decoupl_lpf = spll_obj->z[1] + spll_obj->z[0];
    spll_obj->z[0] = spll_obj->z[1];

    spll_obj->v_q[0] = spll_obj->q_p_decoupl;

    //
    // Loop Filter
    //
    spll_obj->ylf[0] = spll_obj->ylf[1]
                    + (spll_obj->lpf_coeff.b0 * spll_obj->v_q[0])
                    + (spll_obj->lpf_coeff.b1 * spll_obj->v_q[1]);
    spll_obj->ylf[1] = spll_obj->ylf[0];
    spll_obj->v_q[1] = spll_obj->v_q[0];

    //
    // VCO
    //
    spll_obj->fo = spll_obj->fn + spll_obj->ylf[0];

    spll_obj->theta[0] = spll_obj->theta[1] +
             ((spll_obj->fo * spll_obj->delta_t)
             * (float)(2.0f * 3.1415926f));

    if(spll_obj->theta[0] > (float)(2.0f * 3.1415926f))
    {
        spll_obj->theta[0] = spll_obj->theta[0] -
                  (float)(2.0f * 3.1415926f);
    }


    spll_obj->theta[1] = spll_obj->theta[0];

    spll_obj->cos_2theta = cosf(spll_obj->theta[1] * 2.0f);
    spll_obj->sin_2theta = sinf(spll_obj->theta[1] * 2.0f);
}

