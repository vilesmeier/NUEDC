#ifndef ThreePhase_Rectifier_acc_h_
#define ThreePhase_Rectifier_acc_h_
#ifndef ThreePhase_Rectifier_acc_COMMON_INCLUDES_
#define ThreePhase_Rectifier_acc_COMMON_INCLUDES_
#include <stdlib.h>
#define S_FUNCTION_NAME simulink_only_sfcn
#define S_FUNCTION_LEVEL 2
#ifndef RTW_GENERATED_S_FUNCTION
#define RTW_GENERATED_S_FUNCTION
#endif
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif
#include "ThreePhase_Rectifier_acc_types.h"
#include <string.h>
#include "rt_defines.h"
#include <stddef.h>
typedef struct { real_T B_6_276_2152 [ 3 ] ; real_T B_6_279_2176 ; real_T
B_6_280_2184 [ 3 ] ; real_T B_6_283_2208 [ 3 ] ; real_T B_6_286_2232 [ 3 ] ;
real_T B_6_289_2256 ; real_T B_6_290_2264 [ 3 ] ; real_T B_6_293_2288 [ 3 ] ;
real_T B_6_296_2312 [ 3 ] ; real_T B_6_299_2336 [ 3 ] ; real_T B_6_302_2360 [
3 ] ; } B_RMS_ThreePhase_Rectifier_T ; typedef struct { real_T
Memory_PreviousInput [ 3 ] ; real_T Memory_PreviousInput_f [ 3 ] ; struct {
real_T modelTStart ; } TransportDelay_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_n ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_j ; int32_T RMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_k ; struct { int_T Tail [ 3 ] ;
int_T Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; struct { int_T Tail [ 3 ] ; int_T
Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_n ; int8_T RMS_SubsysRanBC ; boolean_T
RMS_MODE ; char_T pad_RMS_MODE [ 2 ] ; } DW_RMS_ThreePhase_Rectifier_T ;
typedef struct { real_T integrator_CSTATE_o [ 3 ] ; real_T
integrator_CSTATE_fb [ 3 ] ; } X_RMS_ThreePhase_Rectifier_T ; typedef struct
{ real_T integrator_CSTATE_o [ 3 ] ; real_T integrator_CSTATE_fb [ 3 ] ; }
XDot_RMS_ThreePhase_Rectifier_T ; typedef struct { boolean_T
integrator_CSTATE_o [ 3 ] ; boolean_T integrator_CSTATE_fb [ 3 ] ; }
XDis_RMS_ThreePhase_Rectifier_T ; typedef struct { real_T integrator_CSTATE_o
[ 3 ] ; real_T integrator_CSTATE_fb [ 3 ] ; }
XAbsTol_RMS_ThreePhase_Rectifier_T ; typedef struct { real_T
integrator_CSTATE_o [ 3 ] ; real_T integrator_CSTATE_fb [ 3 ] ; }
XPtMin_RMS_ThreePhase_Rectifier_T ; typedef struct { real_T
integrator_CSTATE_o [ 3 ] ; real_T integrator_CSTATE_fb [ 3 ] ; }
XPtMax_RMS_ThreePhase_Rectifier_T ; typedef struct { real_T B_8_260_2024 [ 3
] ; real_T B_8_263_2048 ; real_T B_8_264_2056 [ 3 ] ; real_T B_8_267_2080 [ 3
] ; real_T B_8_270_2104 [ 3 ] ; real_T B_8_273_2128 [ 3 ] ; }
B_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { real_T
Memory_PreviousInput [ 3 ] ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK ; int32_T TrueRMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; struct { int_T Tail [ 3 ] ;
int_T Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; int_T
Saturationtoavoidnegativesqrt_MODE [ 3 ] ; int8_T TrueRMS_SubsysRanBC ;
int8_T Sqrt_DWORK1 ; boolean_T TrueRMS_MODE ; char_T pad_TrueRMS_MODE [ 5 ] ;
} DW_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { real_T
integrator_CSTATE_f [ 3 ] ; } X_TrueRMS_ThreePhase_Rectifier_T ; typedef
struct { real_T integrator_CSTATE_f [ 3 ] ; }
XDot_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { boolean_T
integrator_CSTATE_f [ 3 ] ; } XDis_TrueRMS_ThreePhase_Rectifier_T ; typedef
struct { real_T integrator_CSTATE_f [ 3 ] ; }
XAbsTol_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { real_T
integrator_CSTATE_f [ 3 ] ; } XPtMin_TrueRMS_ThreePhase_Rectifier_T ; typedef
struct { real_T integrator_CSTATE_f [ 3 ] ; }
XPtMax_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { real_T
Saturationtoavoidnegativesqrt_UprLim_ZC_i [ 3 ] ; real_T
Saturationtoavoidnegativesqrt_LwrLim_ZC_m [ 3 ] ; }
ZCV_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { ZCSigState
Saturationtoavoidnegativesqrt_UprLim_ZCE_c [ 3 ] ; ZCSigState
Saturationtoavoidnegativesqrt_LwrLim_ZCE_j [ 3 ] ; }
ZCE_TrueRMS_ThreePhase_Rectifier_T ; typedef struct { real_T B_17_186_1432 [
3 ] ; real_T B_17_189_1456 ; real_T B_17_190_1464 [ 3 ] ; real_T
B_17_193_1488 [ 3 ] ; real_T B_17_196_1512 [ 3 ] ; real_T B_17_199_1536 ;
real_T B_17_200_1544 [ 3 ] ; real_T B_17_203_1568 [ 3 ] ; real_T
B_17_206_1592 [ 3 ] ; real_T B_17_209_1616 [ 3 ] ; real_T B_17_212_1640 [ 3 ]
; } B_RMS_ThreePhase_Rectifier_n_T ; typedef struct { real_T
Memory_PreviousInput [ 3 ] ; real_T Memory_PreviousInput_p [ 3 ] ; struct {
real_T modelTStart ; } TransportDelay_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_p ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_g ; int32_T RMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_b ; struct { int_T Tail [ 3 ] ;
int_T Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; struct { int_T Tail [ 3 ] ; int_T
Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_a ; int8_T RMS_SubsysRanBC ; boolean_T
RMS_MODE ; char_T pad_RMS_MODE [ 2 ] ; } DW_RMS_ThreePhase_Rectifier_i_T ;
typedef struct { real_T integrator_CSTATE_h [ 3 ] ; real_T
integrator_CSTATE_i [ 3 ] ; } X_RMS_ThreePhase_Rectifier_j_T ; typedef struct
{ real_T integrator_CSTATE_h [ 3 ] ; real_T integrator_CSTATE_i [ 3 ] ; }
XDot_RMS_ThreePhase_Rectifier_n_T ; typedef struct { boolean_T
integrator_CSTATE_h [ 3 ] ; boolean_T integrator_CSTATE_i [ 3 ] ; }
XDis_RMS_ThreePhase_Rectifier_g_T ; typedef struct { real_T
integrator_CSTATE_h [ 3 ] ; real_T integrator_CSTATE_i [ 3 ] ; }
XAbsTol_RMS_ThreePhase_Rectifier_j_T ; typedef struct { real_T
integrator_CSTATE_h [ 3 ] ; real_T integrator_CSTATE_i [ 3 ] ; }
XPtMin_RMS_ThreePhase_Rectifier_h_T ; typedef struct { real_T
integrator_CSTATE_h [ 3 ] ; real_T integrator_CSTATE_i [ 3 ] ; }
XPtMax_RMS_ThreePhase_Rectifier_b_T ; typedef struct { real_T B_19_170_1304 [
3 ] ; real_T B_19_173_1328 ; real_T B_19_174_1336 [ 3 ] ; real_T
B_19_177_1360 [ 3 ] ; real_T B_19_180_1384 [ 3 ] ; real_T B_19_183_1408 [ 3 ]
; } B_TrueRMS_ThreePhase_Rectifier_e_T ; typedef struct { real_T
Memory_PreviousInput [ 3 ] ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK ; int32_T TrueRMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; struct { int_T Tail [ 3 ] ;
int_T Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK ; int_T
Saturationtoavoidnegativesqrt_MODE [ 3 ] ; int8_T TrueRMS_SubsysRanBC ;
int8_T Sqrt_DWORK1 ; boolean_T TrueRMS_MODE ; char_T pad_TrueRMS_MODE [ 5 ] ;
} DW_TrueRMS_ThreePhase_Rectifier_j_T ; typedef struct { real_T
integrator_CSTATE_k [ 3 ] ; } X_TrueRMS_ThreePhase_Rectifier_f_T ; typedef
struct { real_T integrator_CSTATE_k [ 3 ] ; }
XDot_TrueRMS_ThreePhase_Rectifier_n_T ; typedef struct { boolean_T
integrator_CSTATE_k [ 3 ] ; } XDis_TrueRMS_ThreePhase_Rectifier_p_T ; typedef
struct { real_T integrator_CSTATE_k [ 3 ] ; }
XAbsTol_TrueRMS_ThreePhase_Rectifier_c_T ; typedef struct { real_T
integrator_CSTATE_k [ 3 ] ; } XPtMin_TrueRMS_ThreePhase_Rectifier_e_T ;
typedef struct { real_T integrator_CSTATE_k [ 3 ] ; }
XPtMax_TrueRMS_ThreePhase_Rectifier_j_T ; typedef struct { real_T
Saturationtoavoidnegativesqrt_UprLim_ZC [ 3 ] ; real_T
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 3 ] ; }
ZCV_TrueRMS_ThreePhase_Rectifier_k_T ; typedef struct { ZCSigState
Saturationtoavoidnegativesqrt_UprLim_ZCE [ 3 ] ; ZCSigState
Saturationtoavoidnegativesqrt_LwrLim_ZCE [ 3 ] ; }
ZCE_TrueRMS_ThreePhase_Rectifier_k_T ; typedef struct { real_T B_25_0_0 ;
real_T B_25_1_8 ; real_T B_25_2_16 ; real_T B_25_3_24 [ 11 ] ; real_T
B_25_14_112 [ 6 ] ; real_T B_25_20_160 ; real_T B_25_21_168 [ 6 ] ; real_T
B_25_27_216 ; real_T B_25_28_224 [ 6 ] ; real_T B_25_34_272 [ 6 ] ; real_T
B_25_40_320 ; real_T B_25_41_328 ; real_T B_25_42_336 ; real_T B_25_43_344 [
2 ] ; real_T B_25_45_360 [ 2 ] ; real_T B_25_47_376 ; real_T B_25_48_384 ;
real_T B_25_49_392 ; real_T B_25_50_400 ; real_T B_25_51_408 ; real_T
B_25_52_416 ; real_T B_25_53_424 ; real_T B_25_54_432 ; real_T B_25_55_440 [
3 ] ; real_T B_25_58_464 ; real_T B_25_59_472 [ 3 ] ; real_T B_25_62_496 [ 3
] ; real_T B_25_65_520 [ 3 ] ; real_T B_25_68_544 ; real_T B_25_69_552 [ 3 ]
; real_T B_25_72_576 [ 3 ] ; real_T B_25_75_600 ; real_T B_25_76_608 ; real_T
B_25_77_616 [ 2 ] ; real_T B_25_79_632 ; real_T B_25_80_640 ; real_T
B_25_81_648 [ 3 ] ; real_T B_25_84_672 ; real_T B_25_85_680 [ 3 ] ; real_T
B_25_88_704 ; real_T B_25_89_712 ; real_T B_25_90_720 [ 3 ] ; real_T
B_25_93_744 ; real_T B_25_94_752 ; real_T B_25_95_760 [ 6 ] ; real_T
B_25_101_808 [ 6 ] ; real_T B_25_107_856 ; real_T B_25_108_864 ; real_T
B_25_109_872 ; real_T B_25_110_880 ; real_T B_1_111_888 ; real_T B_1_112_896
; real_T B_1_113_904 ; real_T B_1_114_912 ; real_T B_1_115_920 [ 2 ] ;
boolean_T B_25_117_936 ; boolean_T B_25_118_937 ; boolean_T B_25_119_938 ;
boolean_T B_25_120_939 ; boolean_T B_25_121_940 ; boolean_T B_25_122_941 ;
boolean_T B_25_123_942 ; boolean_T B_25_124_943 ;
B_TrueRMS_ThreePhase_Rectifier_e_T TrueRMS_eh ;
B_RMS_ThreePhase_Rectifier_n_T RMS_kr ; B_TrueRMS_ThreePhase_Rectifier_e_T
TrueRMS_e ; B_RMS_ThreePhase_Rectifier_n_T RMS_k ;
B_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ; B_RMS_ThreePhase_Rectifier_T
RMS_g ; B_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
B_RMS_ThreePhase_Rectifier_T RMS ; } B_ThreePhase_Rectifier_T ; typedef
struct { real_T StateSpace_DSTATE [ 6 ] ; real_T Memory_PreviousInput [ 6 ] ;
real_T Memory_PreviousInput_g ; real_T Memory_PreviousInput_o [ 3 ] ; real_T
Memory_PreviousInput_j [ 3 ] ; struct { real_T modelTStart ; }
TransportDelay_RWORK ; struct { real_T modelTStart ; }
VariableTimeDelay1_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_c ; struct { real_T modelTStart ; }
TransportDelay_RWORK_f ; struct { real_T modelTStart ; }
TransportDelay_RWORK_ck ; struct { void * AS ; void * BS ; void * CS ; void *
DS ; void * DX_COL ; void * BD_COL ; void * TMP1 ; void * TMP2 ; void * XTMP
; void * SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void * SW_CHG ; void *
G_STATE ; void * USWLAST ; void * XKM12 ; void * XKP12 ; void * XLAST ; void
* ULAST ; void * IDX_SW_CHG ; void * Y_SWITCH ; void * SWITCH_TYPES ; void *
IDX_OUT_SW ; void * SWITCH_TOPO_SAVED_IDX ; void * SWITCH_MAP ; }
StateSpace_PWORK ; void * Scope1_PWORK ; struct { void * TUbufferPtrs [ 12 ]
; } TransportDelay_PWORK ; void * Scope10_PWORK [ 3 ] ; struct { void *
TUbufferPtrs [ 4 ] ; } VariableTimeDelay1_PWORK ; void * Scope2_PWORK [ 2 ] ;
void * Scope5_PWORK [ 2 ] ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_n ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_i ; struct { void * TUbufferPtrs [ 6 ] ; }
TransportDelay_PWORK_d ; void * Scope_PWORK ; struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Gain1_at_outport_0_PWORK ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_MinMax1_at_outport_0_PWORK ; struct {
void * AQHandles ; } TAQSigLogging_InsertedFor_MinMax_at_outport_0_PWORK ;
int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_e ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_m ; int32_T PR_sysIdxToRun ;
int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_n ; int_T StateSpace_IWORK
[ 11 ] ; struct { int_T Tail [ 6 ] ; int_T Head [ 6 ] ; int_T Last [ 6 ] ;
int_T CircularBufSize [ 6 ] ; int_T MaxNewBufSize ; } TransportDelay_IWORK ;
struct { int_T Tail [ 2 ] ; int_T Head [ 2 ] ; int_T Last [ 2 ] ; int_T
CircularBufSize [ 2 ] ; int_T MaxNewBufSize ; } VariableTimeDelay1_IWORK ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_f ; struct { int_T Tail [ 3 ] ; int_T
Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_n ; struct { int_T Tail [ 3 ] ; int_T
Head [ 3 ] ; int_T Last [ 3 ] ; int_T CircularBufSize [ 3 ] ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_h ; int_T MinMax1_MODE ; int_T
MinMax_MODE ; int_T Saturation_MODE [ 2 ] ; int8_T Sqrt_DWORK1 ; boolean_T
RelationalOperator7_Mode ; boolean_T RelationalOperator6_Mode ; boolean_T
RelationalOperator2_Mode ; DW_TrueRMS_ThreePhase_Rectifier_j_T TrueRMS_eh ;
DW_RMS_ThreePhase_Rectifier_i_T RMS_kr ; DW_TrueRMS_ThreePhase_Rectifier_j_T
TrueRMS_e ; DW_RMS_ThreePhase_Rectifier_i_T RMS_k ;
DW_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ; DW_RMS_ThreePhase_Rectifier_T
RMS_g ; DW_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
DW_RMS_ThreePhase_Rectifier_T RMS ; } DW_ThreePhase_Rectifier_T ; typedef
struct { real_T integrator_CSTATE [ 6 ] ; real_T Integrator_CSTATE ; real_T
integrator_CSTATE_j ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; X_TrueRMS_ThreePhase_Rectifier_f_T TrueRMS_eh ;
X_RMS_ThreePhase_Rectifier_j_T RMS_kr ; X_TrueRMS_ThreePhase_Rectifier_f_T
TrueRMS_e ; X_RMS_ThreePhase_Rectifier_j_T RMS_k ;
X_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ; X_RMS_ThreePhase_Rectifier_T
RMS_g ; X_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
X_RMS_ThreePhase_Rectifier_T RMS ; real_T TransferFcn_CSTATE [ 2 ] ; real_T
TransferFcn1_CSTATE [ 2 ] ; } X_ThreePhase_Rectifier_T ; typedef struct {
real_T integrator_CSTATE [ 6 ] ; real_T Integrator_CSTATE ; real_T
integrator_CSTATE_j ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; XDot_TrueRMS_ThreePhase_Rectifier_n_T TrueRMS_eh
; XDot_RMS_ThreePhase_Rectifier_n_T RMS_kr ;
XDot_TrueRMS_ThreePhase_Rectifier_n_T TrueRMS_e ;
XDot_RMS_ThreePhase_Rectifier_n_T RMS_k ; XDot_TrueRMS_ThreePhase_Rectifier_T
TrueRMS_f ; XDot_RMS_ThreePhase_Rectifier_T RMS_g ;
XDot_TrueRMS_ThreePhase_Rectifier_T TrueRMS ; XDot_RMS_ThreePhase_Rectifier_T
RMS ; real_T TransferFcn_CSTATE [ 2 ] ; real_T TransferFcn1_CSTATE [ 2 ] ; }
XDot_ThreePhase_Rectifier_T ; typedef struct { boolean_T integrator_CSTATE [
6 ] ; boolean_T Integrator_CSTATE ; boolean_T integrator_CSTATE_j ; boolean_T
integrator_CSTATE_p [ 3 ] ; boolean_T integrator_CSTATE_l [ 3 ] ;
XDis_TrueRMS_ThreePhase_Rectifier_p_T TrueRMS_eh ;
XDis_RMS_ThreePhase_Rectifier_g_T RMS_kr ;
XDis_TrueRMS_ThreePhase_Rectifier_p_T TrueRMS_e ;
XDis_RMS_ThreePhase_Rectifier_g_T RMS_k ; XDis_TrueRMS_ThreePhase_Rectifier_T
TrueRMS_f ; XDis_RMS_ThreePhase_Rectifier_T RMS_g ;
XDis_TrueRMS_ThreePhase_Rectifier_T TrueRMS ; XDis_RMS_ThreePhase_Rectifier_T
RMS ; boolean_T TransferFcn_CSTATE [ 2 ] ; boolean_T TransferFcn1_CSTATE [ 2
] ; } XDis_ThreePhase_Rectifier_T ; typedef struct { real_T integrator_CSTATE
[ 6 ] ; real_T Integrator_CSTATE ; real_T integrator_CSTATE_j ; real_T
integrator_CSTATE_p [ 3 ] ; real_T integrator_CSTATE_l [ 3 ] ;
XAbsTol_TrueRMS_ThreePhase_Rectifier_c_T TrueRMS_eh ;
XAbsTol_RMS_ThreePhase_Rectifier_j_T RMS_kr ;
XAbsTol_TrueRMS_ThreePhase_Rectifier_c_T TrueRMS_e ;
XAbsTol_RMS_ThreePhase_Rectifier_j_T RMS_k ;
XAbsTol_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ;
XAbsTol_RMS_ThreePhase_Rectifier_T RMS_g ;
XAbsTol_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
XAbsTol_RMS_ThreePhase_Rectifier_T RMS ; real_T TransferFcn_CSTATE [ 2 ] ;
real_T TransferFcn1_CSTATE [ 2 ] ; } CStateAbsTol_ThreePhase_Rectifier_T ;
typedef struct { real_T integrator_CSTATE [ 6 ] ; real_T Integrator_CSTATE ;
real_T integrator_CSTATE_j ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; XPtMin_TrueRMS_ThreePhase_Rectifier_e_T
TrueRMS_eh ; XPtMin_RMS_ThreePhase_Rectifier_h_T RMS_kr ;
XPtMin_TrueRMS_ThreePhase_Rectifier_e_T TrueRMS_e ;
XPtMin_RMS_ThreePhase_Rectifier_h_T RMS_k ;
XPtMin_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ;
XPtMin_RMS_ThreePhase_Rectifier_T RMS_g ;
XPtMin_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
XPtMin_RMS_ThreePhase_Rectifier_T RMS ; real_T TransferFcn_CSTATE [ 2 ] ;
real_T TransferFcn1_CSTATE [ 2 ] ; } CXPtMin_ThreePhase_Rectifier_T ; typedef
struct { real_T integrator_CSTATE [ 6 ] ; real_T Integrator_CSTATE ; real_T
integrator_CSTATE_j ; real_T integrator_CSTATE_p [ 3 ] ; real_T
integrator_CSTATE_l [ 3 ] ; XPtMax_TrueRMS_ThreePhase_Rectifier_j_T
TrueRMS_eh ; XPtMax_RMS_ThreePhase_Rectifier_b_T RMS_kr ;
XPtMax_TrueRMS_ThreePhase_Rectifier_j_T TrueRMS_e ;
XPtMax_RMS_ThreePhase_Rectifier_b_T RMS_k ;
XPtMax_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ;
XPtMax_RMS_ThreePhase_Rectifier_T RMS_g ;
XPtMax_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
XPtMax_RMS_ThreePhase_Rectifier_T RMS ; real_T TransferFcn_CSTATE [ 2 ] ;
real_T TransferFcn1_CSTATE [ 2 ] ; } CXPtMax_ThreePhase_Rectifier_T ; typedef
struct { real_T MinMax1_MinmaxInput_ZC ; real_T MinMax_MinmaxInput_ZC ;
real_T RelationalOperator7_RelopInput_ZC ; real_T
RelationalOperator6_RelopInput_ZC ; real_T RelationalOperator2_RelopInput_ZC
; ZCV_TrueRMS_ThreePhase_Rectifier_k_T TrueRMS_eh ;
ZCV_TrueRMS_ThreePhase_Rectifier_k_T TrueRMS_e ;
ZCV_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ;
ZCV_TrueRMS_ThreePhase_Rectifier_T TrueRMS ; real_T Saturation_UprLim_ZC [ 2
] ; real_T Saturation_LwrLim_ZC [ 2 ] ; } ZCV_ThreePhase_Rectifier_T ;
typedef struct { ZCSigState MinMax1_MinmaxInput_ZCE ; ZCSigState
MinMax_MinmaxInput_ZCE ; ZCSigState RelationalOperator7_RelopInput_ZCE ;
ZCSigState RelationalOperator6_RelopInput_ZCE ; ZCSigState
RelationalOperator2_RelopInput_ZCE ; ZCE_TrueRMS_ThreePhase_Rectifier_k_T
TrueRMS_eh ; ZCE_TrueRMS_ThreePhase_Rectifier_k_T TrueRMS_e ;
ZCE_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ;
ZCE_TrueRMS_ThreePhase_Rectifier_T TrueRMS ; ZCSigState Saturation_UprLim_ZCE
[ 2 ] ; ZCSigState Saturation_LwrLim_ZCE [ 2 ] ; }
PrevZCX_ThreePhase_Rectifier_T ; struct P_RMS_ThreePhase_Rectifier_T_ {
real_T P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ;
real_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11
; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ;
real_T P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 ; } ; struct
P_TrueRMS_ThreePhase_Rectifier_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ;
real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; } ; struct
P_RMS_ThreePhase_Rectifier_f_T_ { real_T P_0 ; real_T P_1 ; real_T P_2 ;
real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ;
real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T
P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T P_19 ;
real_T P_20 ; } ; struct P_TrueRMS_ThreePhase_Rectifier_d_T_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; } ; struct P_ThreePhase_Rectifier_T_ { real_T P_0 ; real_T P_1 ;
real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 [ 2 ] ; real_T P_6 [ 2 ] ;
real_T P_7 [ 2 ] ; real_T P_8 [ 2 ] ; real_T P_9 ; real_T P_10 ; real_T P_11
; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ;
real_T P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T
P_22 ; real_T P_23 ; real_T P_24 ; real_T P_25 ; real_T P_26 [ 2 ] ; real_T
P_27 [ 36 ] ; real_T P_28 [ 2 ] ; real_T P_29 [ 54 ] ; real_T P_30 [ 2 ] ;
real_T P_31 [ 66 ] ; real_T P_32 [ 2 ] ; real_T P_33 [ 99 ] ; real_T P_34 [ 2
] ; real_T P_35 [ 6 ] ; real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39
; real_T P_40 ; real_T P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 ;
real_T P_45 ; real_T P_46 ; real_T P_47 ; real_T P_48 ; real_T P_49 ; real_T
P_50 ; real_T P_51 ; real_T P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 ;
real_T P_56 ; real_T P_57 ; real_T P_58 ; real_T P_59 ; real_T P_60 ; real_T
P_61 ; real_T P_62 ; real_T P_63 ; real_T P_64 ; real_T P_65 ; real_T P_66 ;
real_T P_67 ; real_T P_68 ; real_T P_69 ; real_T P_70 ; real_T P_71 ; real_T
P_72 ; real_T P_73 ; real_T P_74 ; real_T P_75 ; real_T P_76 ; real_T P_77 ;
real_T P_78 [ 3 ] ; real_T P_79 [ 3 ] ; real_T P_80 [ 6 ] ; real_T P_81 ;
real_T P_82 ; real_T P_83 ; real_T P_84 ; real_T P_85 ; real_T P_86 ; real_T
P_87 ; real_T P_88 ; real_T P_89 ; real_T P_90 ; real_T P_91 ; real_T P_92 ;
real_T P_93 ; real_T P_94 ; P_TrueRMS_ThreePhase_Rectifier_d_T TrueRMS_eh ;
P_RMS_ThreePhase_Rectifier_f_T RMS_kr ; P_TrueRMS_ThreePhase_Rectifier_d_T
TrueRMS_e ; P_RMS_ThreePhase_Rectifier_f_T RMS_k ;
P_TrueRMS_ThreePhase_Rectifier_T TrueRMS_f ; P_RMS_ThreePhase_Rectifier_T
RMS_g ; P_TrueRMS_ThreePhase_Rectifier_T TrueRMS ;
P_RMS_ThreePhase_Rectifier_T RMS ; } ; extern P_ThreePhase_Rectifier_T
ThreePhase_Rectifier_rtDefaultP ;
#endif
