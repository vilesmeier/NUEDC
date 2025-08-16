#ifndef PLL_3PH_H
#define PLL_3PH_H

//
// Typedefs
//

//! \brief          Defines the coefficients for a loop filter
//!
//! \details        Loop filter coefficients
//!
typedef struct{
    float b1;
    float b0;
} SRF_3PH_LPF_COEFF;

//! \brief          Defines the SRF_3PH structure
//!
//! \details        This software module implements a software phase lock loop
//!                 based on synchronous reference frame for
//!                 grid connection to three phase grid
//!
typedef struct{
    float v_q[2];     //!< Rotating reference frame Q-axis value
    float ylf[2];     //!< Data buffer for loop filter output
    float fo;         //!< Output frequency of PLL
    float fn;         //!< Nominal frequency
    float theta[2];   //!< Grid phase angle
    float delta_t;    //!< Inverse of the ISR rate at which module is called
    SRF_3PH_LPF_COEFF lpf_coeff;  //!< Loop filter coefficients
} SRF_3PH;


//! \brief          Defines the coefficients for a loop filter
//!
//! \details        Loop filter coefficients
//!
typedef struct{
    float b1;
    float b0;
} DDSRF_3PH_LPF_COEFF;

//! \brief          Defines the DDSRF_3PH structure
//!
//! \details        This software module implements a software phase lock loop
//!                 based on decoupled double synchronous reference frame for
//!                 grid connection to three phase grid
//! \return None
//!
typedef struct{
    float d_p_decoupl;  //!< Positive Rotating reference Frame D-axis value
    float d_n_decoupl;  //!< Negative Rotating reference Frame D-axis value
    float q_p_decoupl;  //!< Positive Rotating reference Frame Q-axis value
    float q_n_decoupl;  //!< Negative Rotating reference Frame Q-axis value

    float cos_2theta;   //!< Cos of twice the grid frequency angle
    float sin_2theta;   //!< Sin of twice the grid frequency angle

    float y[2];         //!< Used to store history for filtering the decoupled D and Q axis components    
    float x[2];         //!< Used to store history for filtering the decoupled D and Q axis components
    float w[2];         //!< Used to store history for filtering the decoupled D and Q axis components
    float z[2];         //!< Used to store history for filtering the decoupled D and Q axis components
    float k1;           //!< Lpf coefficient
    float k2;           //!< Lpf coefficient    
    float d_p_decoupl_lpf;  //!< Decoupled positive sequence D-axis component filtered
    float d_n_decoupl_lpf;  //!< Decoupled negative sequence D-axis component filtered
    float q_p_decoupl_lpf;  //!< Decoupled positive sequence Q-axis component filtered
    float q_n_decoupl_lpf;  //!< Decoupled negative sequence Q-axis component filtered

    float v_q[2];       
    float theta[2];     //!< Grid phase angle
    float ylf[2];       //!< Internal Data Buffer for Loop Filter output
    float fo;           //!< Instantaneous Grid Frequency in Hz
    float fn;           //!< Nominal Grid Frequency in Hz
    float delta_t;      //!< 1/Frequency of calling the PLL routine
    DDSRF_3PH_LPF_COEFF lpf_coeff;
} DDSRF_3PH;

//
//  Functions
//

//SRF_3PH
void SRF_3PH_Init(SRF_3PH *spll_obj);
void SRF_3PH_Set(float grid_freq, float delta_t,SRF_3PH *spll_obj);
void SRF_3PH_Run(float v_q, SRF_3PH *spll_obj);

//DDSRF_3PH
void DDSRF_3PH_Init(DDSRF_3PH *spll_obj);
void DDSRF_3PH_Set(float grid_freq, float delta_t,float k1, float k2,DDSRF_3PH *spll_obj);
void DDSRF_3PH_Run(DDSRF_3PH *spll_obj,float d_p, float d_n,float q_p, float q_n);

#endif
