#ifndef PLL_1PH_H 
#define PLL_1PH_H

//
// Typedefs
//

//! \brief  Defines the SOGI_1PH_OSG_COEFF structure
//!
typedef struct{
    float32_t osg_k;
    float32_t osg_x;
    float32_t osg_y;
    float32_t osg_b0;
    float32_t osg_b2;
    float32_t osg_a1;
    float32_t osg_a2;
    float32_t osg_qb0;
    float32_t osg_qb1;
    float32_t osg_qb2;
} SOGI_1PH_OSG_COEFF;

//! \brief  Defines the SOGI_1PH_LPF_COEFF structure
//!
typedef struct{
    float32_t b1;
    float32_t b0;
} SOGI_1PH_LPF_COEFF;

//! \brief Defines the Orthogonal Signal Generator SOGI_1PH
//!        structure
//!
//! \details The SOGI_1PH can be used to generate the
//!          orthogonal signal from the sensed single phase grid voltage
//!          and use that information to provide phase of the grid voltage
//!
typedef struct{
    float32_t   u[3];       //!< AC input data buffer
    float32_t   osg_u[3];   //!< Orthogonal signal generator data buffer
    float32_t   osg_qu[3];  //!< Orthogonal signal generator quadrature data buffer
    float32_t   u_Q[2];     //!< Q-axis component
    float32_t   u_D[2];     //!< D-axis component
    float32_t   ylf[2];     //!< Loop filter data storage
    float32_t   fo;         //!< Output frequency of PLL(Hz)
    float32_t   fn;         //!< Nominal frequency (Hz)
    float32_t   theta;      //!< Angle output (0-2*pi)
    float32_t   cosine;     //!< Cosine value of the PLL angle
    float32_t   sine;       //!< Sine value of the PLL angle
    float32_t   delta_t;    //!< Inverse of the ISR rate at which module is called
    SOGI_1PH_OSG_COEFF osg_coeff; //!< Orthogonal signal generator coefficient
    SOGI_1PH_LPF_COEFF lpf_coeff; //!< Loop filter coeffcient structure
} SOGI_1PH;


//
// Functions
//
void SOGI_1PH_Init(SOGI_1PH *spll_obj);
void SOGI_1PH_coeff_calc(SOGI_1PH *spll_obj);
void SOGI_1PH_Set(SOGI_1PH *spll_obj,float32_t acFreq,float32_t isrFrequency,float32_t lpf_b0,float32_t lpf_b1);
void SOGI_1PH_Run(SOGI_1PH *spll_obj,float32_t acValue);

#endif
