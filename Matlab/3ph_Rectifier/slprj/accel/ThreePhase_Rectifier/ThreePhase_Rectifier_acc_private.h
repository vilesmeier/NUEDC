#ifndef ThreePhase_Rectifier_acc_private_h_
#define ThreePhase_Rectifier_acc_private_h_
#include "rtwtypes.h"
#include "multiword_types.h"
#include <stddef.h>
#include <float.h>
#include "mwmathutil.h"
#include "ThreePhase_Rectifier_acc.h"
#include "ThreePhase_Rectifier_acc_types.h"
#if !defined(ss_VALIDATE_MEMORY)
#define ss_VALIDATE_MEMORY(S, ptr)     if(!(ptr)) {\
    ssSetErrorStatus(S, RT_MEMORY_ALLOCATION_ERROR);\
    }
#endif
#if !defined(rt_FREE)
#if !defined(_WIN32)
#define rt_FREE(ptr)     if((ptr) != (NULL)) {\
    free((ptr));\
    (ptr) = (NULL);\
    }
#else
#define rt_FREE(ptr)     if((ptr) != (NULL)) {\
    free((void *)(ptr));\
    (ptr) = (NULL);\
    }
#endif
#endif
#ifndef __RTW_UTFREE__
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) ; real_T
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T
tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) ; extern real_T look1_binlxpw ( real_T u0 ,
const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) ; void
ThreePhase_Rectifier_RMS_Init ( SimStruct * S , DW_RMS_ThreePhase_Rectifier_T
* localDW , P_RMS_ThreePhase_Rectifier_T * localP ,
X_RMS_ThreePhase_Rectifier_T * localX ) ; void ThreePhase_Rectifier_RMS_Deriv
( SimStruct * S , B_RMS_ThreePhase_Rectifier_T * localB ,
DW_RMS_ThreePhase_Rectifier_T * localDW , XDot_RMS_ThreePhase_Rectifier_T *
localXdot ) ; void ThreePhase_Rectifier_RMS_Disable ( SimStruct * S ,
DW_RMS_ThreePhase_Rectifier_T * localDW ) ; void
ThreePhase_Rectifier_RMS_Update ( SimStruct * S ,
B_RMS_ThreePhase_Rectifier_T * localB , DW_RMS_ThreePhase_Rectifier_T *
localDW , P_RMS_ThreePhase_Rectifier_T * localP ) ; void
ThreePhase_Rectifier_RMS ( SimStruct * S , boolean_T rtu_Enable , real_T
rtu_In , real_T rtu_In_h , real_T rtu_In_k , B_RMS_ThreePhase_Rectifier_T *
localB , DW_RMS_ThreePhase_Rectifier_T * localDW ,
P_RMS_ThreePhase_Rectifier_T * localP , X_RMS_ThreePhase_Rectifier_T * localX
, XDis_RMS_ThreePhase_Rectifier_T * localXdis ) ; void
ThreePhase_Rectifier_TrueRMS_Init ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_T * localP , X_TrueRMS_ThreePhase_Rectifier_T
* localX ) ; void ThreePhase_Rectifier_TrueRMS_Deriv ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , XDot_TrueRMS_ThreePhase_Rectifier_T * localXdot ) ; void
ThreePhase_Rectifier_TrueRMS_ZC ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , P_TrueRMS_ThreePhase_Rectifier_T * localP ,
ZCV_TrueRMS_ThreePhase_Rectifier_T * localZCSV ) ; void
ThreePhase_Rectifier_TrueRMS_Disable ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_T * localDW ) ; void
ThreePhase_Rectifier_TrueRMS_Update ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , P_TrueRMS_ThreePhase_Rectifier_T * localP ) ; void
ThreePhase_Rectifier_TrueRMS ( SimStruct * S , boolean_T rtu_Enable , real_T
rtu_In , real_T rtu_In_h , real_T rtu_In_c , B_TrueRMS_ThreePhase_Rectifier_T
* localB , DW_TrueRMS_ThreePhase_Rectifier_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_T * localP , X_TrueRMS_ThreePhase_Rectifier_T
* localX , XDis_TrueRMS_ThreePhase_Rectifier_T * localXdis ) ; void
ThreePhase_Rectifier_RMS_p_Init ( SimStruct * S ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW , P_RMS_ThreePhase_Rectifier_f_T *
localP , X_RMS_ThreePhase_Rectifier_j_T * localX ) ; void
ThreePhase_Rectifier_RMS_h_Deriv ( SimStruct * S ,
B_RMS_ThreePhase_Rectifier_n_T * localB , DW_RMS_ThreePhase_Rectifier_i_T *
localDW , XDot_RMS_ThreePhase_Rectifier_n_T * localXdot ) ; void
ThreePhase_Rectifier_RMS_l_Disable ( SimStruct * S ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW ) ; void
ThreePhase_Rectifier_RMS_j_Update ( SimStruct * S ,
B_RMS_ThreePhase_Rectifier_n_T * localB , DW_RMS_ThreePhase_Rectifier_i_T *
localDW , P_RMS_ThreePhase_Rectifier_f_T * localP ) ; void
ThreePhase_Rectifier_RMS_k ( SimStruct * S , boolean_T rtu_Enable , const
real_T rtu_In [ 3 ] , B_RMS_ThreePhase_Rectifier_n_T * localB ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW , P_RMS_ThreePhase_Rectifier_f_T *
localP , X_RMS_ThreePhase_Rectifier_j_T * localX ,
XDis_RMS_ThreePhase_Rectifier_g_T * localXdis ) ; void
ThreePhase_Rectifier_TrueRMS_p_Init ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ,
X_TrueRMS_ThreePhase_Rectifier_f_T * localX ) ; void
ThreePhase_Rectifier_TrueRMS_e_Deriv ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
XDot_TrueRMS_ThreePhase_Rectifier_n_T * localXdot ) ; void
ThreePhase_Rectifier_TrueRMS_a_ZC ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ,
ZCV_TrueRMS_ThreePhase_Rectifier_k_T * localZCSV ) ; void
ThreePhase_Rectifier_TrueRMS_e_Disable ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ) ; void
ThreePhase_Rectifier_TrueRMS_c_Update ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ) ; void
ThreePhase_Rectifier_TrueRMS_e ( SimStruct * S , boolean_T rtu_Enable , const
real_T rtu_In [ 3 ] , B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ,
X_TrueRMS_ThreePhase_Rectifier_f_T * localX ,
XDis_TrueRMS_ThreePhase_Rectifier_p_T * localXdis ) ; void
ThreePhase_Rectifier_RMS_Term ( SimStruct * const S ) ; void
ThreePhase_Rectifier_TrueRMS_Term ( SimStruct * const S ) ; void
ThreePhase_Rectifier_RMS_i_Term ( SimStruct * const S ) ; void
ThreePhase_Rectifier_TrueRMS_o_Term ( SimStruct * const S ) ;
#endif
