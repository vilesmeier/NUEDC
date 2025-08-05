#ifndef DC_AC_VSG_acc_h_
#define DC_AC_VSG_acc_h_
#ifndef DC_AC_VSG_acc_COMMON_INCLUDES_
#define DC_AC_VSG_acc_COMMON_INCLUDES_
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
#include "DC_AC_VSG_acc_types.h"
#include <stddef.h>
#include <string.h>
#include "rt_defines.h"
typedef struct { real_T B_3_216_1672 ; real_T B_3_217_1680 ; }
B_PR_DC_AC_VSG_T ; typedef struct { int32_T PR_sysIdxToRun ; char_T
pad_PR_sysIdxToRun [ 4 ] ; } DW_PR_DC_AC_VSG_T ; typedef struct { real_T
TransferFcn_CSTATE_e [ 2 ] ; } X_PR_DC_AC_VSG_T ; typedef struct { real_T
TransferFcn_CSTATE_e [ 2 ] ; } XDot_PR_DC_AC_VSG_T ; typedef struct {
boolean_T TransferFcn_CSTATE_e [ 2 ] ; } XDis_PR_DC_AC_VSG_T ; typedef struct
{ real_T B_7_205_1584 ; real_T B_7_206_1592 ; real_T B_7_207_1600 ; real_T
B_7_208_1608 ; real_T B_7_209_1616 ; real_T B_7_210_1624 ; real_T
B_7_211_1632 ; real_T B_7_212_1640 ; real_T B_7_213_1648 ; real_T
B_7_214_1656 ; real_T B_7_215_1664 ; } B_RMS_DC_AC_VSG_T ; typedef struct {
real_T Memory_PreviousInput ; real_T Memory_PreviousInput_c ; struct { real_T
modelTStart ; } TransportDelay_RWORK ; struct { real_T modelTStart ; }
TransportDelay_RWORK_g ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_j ; int32_T RMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_j ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_g ; int8_T
RMS_SubsysRanBC ; boolean_T RMS_MODE ; char_T pad_RMS_MODE [ 2 ] ; }
DW_RMS_DC_AC_VSG_T ; typedef struct { real_T integrator_CSTATE_ld ; real_T
integrator_CSTATE_f ; } X_RMS_DC_AC_VSG_T ; typedef struct { real_T
integrator_CSTATE_ld ; real_T integrator_CSTATE_f ; } XDot_RMS_DC_AC_VSG_T ;
typedef struct { boolean_T integrator_CSTATE_ld ; boolean_T
integrator_CSTATE_f ; } XDis_RMS_DC_AC_VSG_T ; typedef struct { real_T
B_9_199_1536 ; real_T B_9_200_1544 ; real_T B_9_201_1552 ; real_T
B_9_202_1560 ; real_T B_9_203_1568 ; real_T B_9_204_1576 ; }
B_TrueRMS_DC_AC_VSG_T ; typedef struct { real_T Memory_PreviousInput ; struct
{ real_T modelTStart ; } TransportDelay_RWORK ; struct { void * TUbufferPtrs
[ 2 ] ; } TransportDelay_PWORK ; int32_T TrueRMS_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; struct { int_T Tail ; int_T Head
; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; int8_T TrueRMS_SubsysRanBC ; boolean_T TrueRMS_MODE ;
char_T pad_TrueRMS_MODE [ 2 ] ; } DW_TrueRMS_DC_AC_VSG_T ; typedef struct {
real_T integrator_CSTATE_n ; } X_TrueRMS_DC_AC_VSG_T ; typedef struct {
real_T integrator_CSTATE_n ; } XDot_TrueRMS_DC_AC_VSG_T ; typedef struct {
boolean_T integrator_CSTATE_n ; } XDis_TrueRMS_DC_AC_VSG_T ; typedef struct {
real_T B_30_0_0 ; real_T B_30_1_8 ; real_T B_30_2_16 ; real_T B_30_3_24 ;
real_T B_30_4_32 ; real_T B_30_5_40 ; real_T B_30_6_48 ; real_T B_30_7_56 ;
real_T B_30_8_64 [ 2 ] ; real_T B_30_10_80 [ 9 ] ; real_T B_30_19_152 [ 4 ] ;
real_T B_30_23_184 ; real_T B_30_24_192 ; real_T B_30_25_200 ; real_T
B_30_26_208 ; real_T B_30_27_216 ; real_T B_30_28_224 ; real_T B_30_29_232 ;
real_T B_30_30_240 ; real_T B_30_31_248 ; real_T B_30_32_256 ; real_T
B_30_33_264 ; real_T B_30_34_272 ; real_T B_30_35_280 ; real_T B_30_36_288 ;
real_T B_30_37_296 ; real_T B_30_38_304 ; real_T B_30_39_312 ; real_T
B_30_40_320 [ 4 ] ; real_T B_30_44_352 ; real_T B_30_45_360 ; real_T
B_30_46_368 ; real_T B_30_47_376 ; real_T B_30_48_384 ; real_T B_30_49_392 ;
real_T B_30_50_400 ; real_T B_30_51_408 ; real_T B_30_52_416 ; real_T
B_30_53_424 ; real_T B_30_54_432 ; real_T B_30_55_440 ; real_T B_30_56_448 ;
real_T B_30_57_456 ; real_T B_30_58_464 ; real_T B_30_59_472 ; real_T
B_30_60_480 ; real_T B_30_61_488 ; real_T B_30_62_496 [ 2 ] ; real_T
B_30_64_512 [ 9 ] ; real_T B_30_73_584 [ 4 ] ; real_T B_30_77_616 ; real_T
B_30_78_624 ; real_T B_30_79_632 ; real_T B_30_80_640 ; real_T B_30_81_648 ;
real_T B_30_82_656 ; real_T B_30_83_664 ; real_T B_30_84_672 ; real_T
B_30_85_680 ; real_T B_30_86_688 ; real_T B_30_87_696 ; real_T B_30_88_704 ;
real_T B_30_89_712 ; real_T B_30_90_720 ; real_T B_30_91_728 ; real_T
B_30_92_736 ; real_T B_30_93_744 [ 4 ] ; real_T B_30_97_776 ; real_T
B_30_98_784 ; real_T B_30_99_792 ; real_T B_30_100_800 ; real_T B_30_101_808
; real_T B_30_102_816 ; real_T B_30_103_824 ; real_T B_30_104_832 ; real_T
B_30_105_840 ; real_T B_30_106_848 ; real_T B_30_107_856 ; real_T
B_30_108_864 ; real_T B_30_109_872 ; real_T B_30_110_880 ; real_T
B_30_111_888 [ 4 ] ; real_T B_30_115_920 ; real_T B_30_116_928 ; real_T
B_30_117_936 ; real_T B_30_118_944 ; real_T B_30_119_952 ; real_T
B_30_120_960 ; real_T B_30_121_968 ; real_T B_30_122_976 [ 4 ] ; real_T
B_30_126_1008 ; real_T B_30_127_1016 ; real_T B_30_128_1024 ; real_T
B_30_129_1032 ; real_T B_30_130_1040 ; real_T B_30_131_1048 ; real_T
B_30_132_1056 ; real_T B_30_133_1064 ; real_T B_30_134_1072 ; real_T
B_30_135_1080 ; real_T B_17_136_1088 ; real_T B_2_137_1096 ; boolean_T
B_30_138_1104 ; boolean_T B_30_139_1105 ; boolean_T B_30_140_1106 ; boolean_T
B_30_141_1107 ; boolean_T B_30_142_1108 ; boolean_T B_30_143_1109 ; boolean_T
B_30_144_1110 ; boolean_T B_30_145_1111 ; B_TrueRMS_DC_AC_VSG_T TrueRMS_b ;
B_RMS_DC_AC_VSG_T RMS_m ; B_TrueRMS_DC_AC_VSG_T TrueRMS_c ; B_RMS_DC_AC_VSG_T
RMS_h ; B_PR_DC_AC_VSG_T PR_l ; B_TrueRMS_DC_AC_VSG_T TrueRMS_o ;
B_RMS_DC_AC_VSG_T RMS_p ; B_TrueRMS_DC_AC_VSG_T TrueRMS ; B_RMS_DC_AC_VSG_T
RMS ; B_PR_DC_AC_VSG_T PR ; } B_DC_AC_VSG_T ; typedef struct { real_T
StateSpace_DSTATE [ 3 ] ; real_T StateSpace_DSTATE_j [ 3 ] ; real_T
Memory_PreviousInput ; real_T Memory_PreviousInput_a ; real_T
Memory_PreviousInput_i ; real_T Memory_PreviousInput_f ; real_T
Memory_PreviousInput_k ; real_T Memory_PreviousInput_p ; struct { real_T
modelTStart ; } TransportDelay_RWORK ; struct { real_T modelTStart ; }
T_RWORK ; struct { real_T modelTStart ; } T1_RWORK ; struct { real_T
modelTStart ; } T_RWORK_n ; struct { real_T modelTStart ; } T1_RWORK_n ;
struct { real_T modelTStart ; } TransportDelay_RWORK_n ; struct { real_T
modelTStart ; } TransportDelay_RWORK_o ; struct { real_T modelTStart ; }
TransportDelay_RWORK_n2 ; struct { real_T modelTStart ; } T_RWORK_i ; struct
{ real_T modelTStart ; } T1_RWORK_m ; struct { real_T modelTStart ; }
T_RWORK_h ; struct { real_T modelTStart ; } T1_RWORK_a ; struct { real_T
modelTStart ; } TransportDelay_RWORK_d ; struct { real_T modelTStart ; }
TransportDelay_RWORK_g ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK ; struct { void * TUbufferPtrs [ 2 ] ; } T_PWORK ;
struct { void * TUbufferPtrs [ 2 ] ; } T1_PWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } T_PWORK_k ; struct { void * TUbufferPtrs [ 2 ] ; }
T1_PWORK_k ; struct { void * AS ; void * BS ; void * CS ; void * DS ; void *
DX_COL ; void * BD_COL ; void * TMP1 ; void * TMP2 ; void * XTMP ; void *
SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void * SW_CHG ; void * G_STATE ;
void * USWLAST ; void * XKM12 ; void * XKP12 ; void * XLAST ; void * ULAST ;
void * IDX_SW_CHG ; void * Y_SWITCH ; void * SWITCH_TYPES ; void * IDX_OUT_SW
; void * SWITCH_TOPO_SAVED_IDX ; void * SWITCH_MAP ; } StateSpace_PWORK ;
struct { void * TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_a ; struct { void
* TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_h ; void * Scope2_PWORK [ 2 ] ;
void * Scope5_PWORK [ 2 ] ; void * Scope1_PWORK [ 2 ] ; void * Scope40_PWORK
[ 2 ] ; void * Scope66_PWORK ; void * Scope6_PWORK ; void * Scope7_PWORK ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK ; struct { void *
TUbufferPtrs [ 2 ] ; } TransportDelay_PWORK_e ; struct { void * TUbufferPtrs
[ 2 ] ; } T_PWORK_f ; struct { void * TUbufferPtrs [ 2 ] ; } T1_PWORK_o ;
struct { void * TUbufferPtrs [ 2 ] ; } T_PWORK_n ; struct { void *
TUbufferPtrs [ 2 ] ; } T1_PWORK_b ; struct { void * AS ; void * BS ; void *
CS ; void * DS ; void * DX_COL ; void * BD_COL ; void * TMP1 ; void * TMP2 ;
void * XTMP ; void * SWITCH_STATUS ; void * SWITCH_STATUS_INIT ; void *
SW_CHG ; void * G_STATE ; void * USWLAST ; void * XKM12 ; void * XKP12 ; void
* XLAST ; void * ULAST ; void * IDX_SW_CHG ; void * Y_SWITCH ; void *
SWITCH_TYPES ; void * IDX_OUT_SW ; void * SWITCH_TOPO_SAVED_IDX ; void *
SWITCH_MAP ; } StateSpace_PWORK_g ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_f ; struct { void * TUbufferPtrs [ 2 ] ; }
TransportDelay_PWORK_l ; void * Scope1_PWORK_a [ 2 ] ; void * Scope3_PWORK [
2 ] ; void * Scope4_PWORK [ 2 ] ; void * Scope6_PWORK_b ; struct { void *
AQHandles ; } TAQSigLogging_InsertedFor_Cm0_at_outport_0_PWORK ; struct {
void * AQHandles ; } TAQSigLogging_InsertedFor_Cm3_at_outport_0_PWORK ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Vm0_at_outport_0_PWORK ; void * Scope1_PWORK_i ;
void * Scope2_PWORK_j ; void * Scope6_PWORK_d ; void * Scope7_PWORK_p ;
struct { void * AQHandles ; }
TAQSigLogging_InsertedFor_Add4_at_outport_0_PWORK ; struct { void * AQHandles
; } TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK_b ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun ; int32_T PF_sysIdxToRun ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_c ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_p ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_j ; int32_T PF_sysIdxToRun_m ;
int32_T TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_n ; int32_T
TmpAtomicSubsysAtSwitchInport1_sysIdxToRun_h ; struct { int_T Tail ; int_T
Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
T1_IWORK ; struct { int_T Tail ; int_T Head ; int_T Last ; int_T
CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK_o ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
T1_IWORK_p ; int_T StateSpace_IWORK [ 11 ] ; struct { int_T Tail ; int_T Head
; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_f ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_k ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } TransportDelay_IWORK_n ; struct { int_T Tail ; int_T Head ;
int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK_d ;
struct { int_T Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T
MaxNewBufSize ; } T1_IWORK_c ; struct { int_T Tail ; int_T Head ; int_T Last
; int_T CircularBufSize ; int_T MaxNewBufSize ; } T_IWORK_k ; struct { int_T
Tail ; int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize
; } T1_IWORK_e ; int_T StateSpace_IWORK_n [ 11 ] ; struct { int_T Tail ;
int_T Head ; int_T Last ; int_T CircularBufSize ; int_T MaxNewBufSize ; }
TransportDelay_IWORK_e ; struct { int_T Tail ; int_T Head ; int_T Last ;
int_T CircularBufSize ; int_T MaxNewBufSize ; } TransportDelay_IWORK_i ;
DW_TrueRMS_DC_AC_VSG_T TrueRMS_b ; DW_RMS_DC_AC_VSG_T RMS_m ;
DW_TrueRMS_DC_AC_VSG_T TrueRMS_c ; DW_RMS_DC_AC_VSG_T RMS_h ;
DW_PR_DC_AC_VSG_T PR_l ; DW_TrueRMS_DC_AC_VSG_T TrueRMS_o ;
DW_RMS_DC_AC_VSG_T RMS_p ; DW_TrueRMS_DC_AC_VSG_T TrueRMS ;
DW_RMS_DC_AC_VSG_T RMS ; DW_PR_DC_AC_VSG_T PR ; } DW_DC_AC_VSG_T ; typedef
struct { real_T integrator_CSTATE ; real_T integ1_CSTATE ; real_T
Integ2_CSTATE ; real_T integ1_CSTATE_f ; real_T Integ2_CSTATE_k ; real_T
integrator_CSTATE_j ; real_T integrator_CSTATE_m ; real_T TransferFcn_CSTATE
; real_T Integrator1_CSTATE ; real_T TransferFcn_CSTATE_c ; real_T
integrator_CSTATE_j0 ; real_T integ1_CSTATE_l ; real_T Integ2_CSTATE_b ;
real_T integ1_CSTATE_j ; real_T Integ2_CSTATE_e ; real_T integrator_CSTATE_l
; real_T integrator_CSTATE_b ; real_T TransferFcn_CSTATE_a ; real_T
Integrator1_CSTATE_l ; real_T Integrator2_CSTATE ; real_T uJsD_CSTATE ;
real_T uJw0s_CSTATE ; X_TrueRMS_DC_AC_VSG_T TrueRMS_b ; X_RMS_DC_AC_VSG_T
RMS_m ; X_TrueRMS_DC_AC_VSG_T TrueRMS_c ; X_RMS_DC_AC_VSG_T RMS_h ;
X_PR_DC_AC_VSG_T PR_l ; X_TrueRMS_DC_AC_VSG_T TrueRMS_o ; X_RMS_DC_AC_VSG_T
RMS_p ; X_TrueRMS_DC_AC_VSG_T TrueRMS ; X_RMS_DC_AC_VSG_T RMS ;
X_PR_DC_AC_VSG_T PR ; } X_DC_AC_VSG_T ; typedef int_T
PeriodicIndX_DC_AC_VSG_T [ 3 ] ; typedef real_T PeriodicRngX_DC_AC_VSG_T [ 6
] ; typedef struct { real_T integrator_CSTATE ; real_T integ1_CSTATE ; real_T
Integ2_CSTATE ; real_T integ1_CSTATE_f ; real_T Integ2_CSTATE_k ; real_T
integrator_CSTATE_j ; real_T integrator_CSTATE_m ; real_T TransferFcn_CSTATE
; real_T Integrator1_CSTATE ; real_T TransferFcn_CSTATE_c ; real_T
integrator_CSTATE_j0 ; real_T integ1_CSTATE_l ; real_T Integ2_CSTATE_b ;
real_T integ1_CSTATE_j ; real_T Integ2_CSTATE_e ; real_T integrator_CSTATE_l
; real_T integrator_CSTATE_b ; real_T TransferFcn_CSTATE_a ; real_T
Integrator1_CSTATE_l ; real_T Integrator2_CSTATE ; real_T uJsD_CSTATE ;
real_T uJw0s_CSTATE ; XDot_TrueRMS_DC_AC_VSG_T TrueRMS_b ;
XDot_RMS_DC_AC_VSG_T RMS_m ; XDot_TrueRMS_DC_AC_VSG_T TrueRMS_c ;
XDot_RMS_DC_AC_VSG_T RMS_h ; XDot_PR_DC_AC_VSG_T PR_l ;
XDot_TrueRMS_DC_AC_VSG_T TrueRMS_o ; XDot_RMS_DC_AC_VSG_T RMS_p ;
XDot_TrueRMS_DC_AC_VSG_T TrueRMS ; XDot_RMS_DC_AC_VSG_T RMS ;
XDot_PR_DC_AC_VSG_T PR ; } XDot_DC_AC_VSG_T ; typedef struct { boolean_T
integrator_CSTATE ; boolean_T integ1_CSTATE ; boolean_T Integ2_CSTATE ;
boolean_T integ1_CSTATE_f ; boolean_T Integ2_CSTATE_k ; boolean_T
integrator_CSTATE_j ; boolean_T integrator_CSTATE_m ; boolean_T
TransferFcn_CSTATE ; boolean_T Integrator1_CSTATE ; boolean_T
TransferFcn_CSTATE_c ; boolean_T integrator_CSTATE_j0 ; boolean_T
integ1_CSTATE_l ; boolean_T Integ2_CSTATE_b ; boolean_T integ1_CSTATE_j ;
boolean_T Integ2_CSTATE_e ; boolean_T integrator_CSTATE_l ; boolean_T
integrator_CSTATE_b ; boolean_T TransferFcn_CSTATE_a ; boolean_T
Integrator1_CSTATE_l ; boolean_T Integrator2_CSTATE ; boolean_T uJsD_CSTATE ;
boolean_T uJw0s_CSTATE ; XDis_TrueRMS_DC_AC_VSG_T TrueRMS_b ;
XDis_RMS_DC_AC_VSG_T RMS_m ; XDis_TrueRMS_DC_AC_VSG_T TrueRMS_c ;
XDis_RMS_DC_AC_VSG_T RMS_h ; XDis_PR_DC_AC_VSG_T PR_l ;
XDis_TrueRMS_DC_AC_VSG_T TrueRMS_o ; XDis_RMS_DC_AC_VSG_T RMS_p ;
XDis_TrueRMS_DC_AC_VSG_T TrueRMS ; XDis_RMS_DC_AC_VSG_T RMS ;
XDis_PR_DC_AC_VSG_T PR ; } XDis_DC_AC_VSG_T ; struct P_PR_DC_AC_VSG_T_ {
real_T P_0 ; real_T P_1 ; real_T P_2 [ 2 ] ; real_T P_3 [ 2 ] ; real_T P_4 ;
real_T P_5 ; } ; struct P_RMS_DC_AC_VSG_T_ { real_T P_0 ; real_T P_1 ; real_T
P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T
P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ;
real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T
P_19 ; real_T P_20 ; real_T P_21 ; } ; struct P_TrueRMS_DC_AC_VSG_T_ { real_T
P_0 ; real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T
P_6 ; real_T P_7 ; real_T P_8 ; } ; struct P_DC_AC_VSG_T_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T
P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ;
real_T P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T
P_23 ; real_T P_24 ; real_T P_25 ; real_T P_26 ; real_T P_27 [ 2 ] ; real_T
P_28 [ 9 ] ; real_T P_29 [ 2 ] ; real_T P_30 [ 15 ] ; real_T P_31 [ 2 ] ;
real_T P_32 [ 27 ] ; real_T P_33 [ 2 ] ; real_T P_34 [ 45 ] ; real_T P_35 [ 2
] ; real_T P_36 [ 3 ] ; real_T P_37 ; real_T P_38 ; real_T P_39 ; real_T P_40
; real_T P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 ; real_T P_45 ;
real_T P_46 ; real_T P_47 ; real_T P_48 ; real_T P_49 ; real_T P_50 ; real_T
P_51 ; real_T P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ;
real_T P_57 ; real_T P_58 ; real_T P_59 ; real_T P_60 ; real_T P_61 [ 3 ] ;
real_T P_62 [ 3 ] ; real_T P_63 ; real_T P_64 ; real_T P_65 ; real_T P_66 ;
real_T P_67 ; real_T P_68 ; real_T P_69 ; real_T P_70 ; real_T P_71 ; real_T
P_72 ; real_T P_73 ; real_T P_74 ; real_T P_75 ; real_T P_76 ; real_T P_77 ;
real_T P_78 ; real_T P_79 ; real_T P_80 ; real_T P_81 ; real_T P_82 ; real_T
P_83 ; real_T P_84 ; real_T P_85 ; real_T P_86 ; real_T P_87 ; real_T P_88 ;
real_T P_89 ; real_T P_90 ; real_T P_91 ; real_T P_92 ; real_T P_93 ; real_T
P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ; real_T P_98 ; real_T P_99 ;
real_T P_100 ; real_T P_101 ; real_T P_102 ; real_T P_103 ; real_T P_104 ;
real_T P_105 ; real_T P_106 ; real_T P_107 ; real_T P_108 ; real_T P_109 ;
real_T P_110 [ 2 ] ; real_T P_111 [ 9 ] ; real_T P_112 [ 2 ] ; real_T P_113 [
15 ] ; real_T P_114 [ 2 ] ; real_T P_115 [ 27 ] ; real_T P_116 [ 2 ] ; real_T
P_117 [ 45 ] ; real_T P_118 [ 2 ] ; real_T P_119 [ 3 ] ; real_T P_120 ;
real_T P_121 ; real_T P_122 ; real_T P_123 ; real_T P_124 ; real_T P_125 ;
real_T P_126 ; real_T P_127 ; real_T P_128 ; real_T P_129 ; real_T P_130 ;
real_T P_131 ; real_T P_132 ; real_T P_133 ; real_T P_134 ; real_T P_135 ;
real_T P_136 ; real_T P_137 ; real_T P_138 ; real_T P_139 ; real_T P_140 ;
real_T P_141 ; real_T P_142 ; real_T P_143 ; real_T P_144 [ 3 ] ; real_T
P_145 [ 3 ] ; real_T P_146 ; real_T P_147 ; real_T P_148 ; real_T P_149 ;
real_T P_150 ; real_T P_151 ; real_T P_152 ; real_T P_153 ; real_T P_154 ;
real_T P_155 ; real_T P_156 ; real_T P_157 ; real_T P_158 ; real_T P_159 ;
real_T P_160 ; real_T P_161 ; real_T P_162 ; real_T P_163 ; real_T P_164 ;
real_T P_165 ; real_T P_166 ; real_T P_167 ; real_T P_168 ; real_T P_169 ;
real_T P_170 ; real_T P_171 ; real_T P_172 ; real_T P_173 ; real_T P_174 ;
real_T P_175 ; real_T P_176 ; real_T P_177 ; real_T P_178 ; real_T P_179 ;
real_T P_180 [ 4 ] ; real_T P_181 ; real_T P_182 ; real_T P_183 ; real_T
P_184 ; real_T P_185 ; real_T P_186 ; real_T P_187 ; real_T P_188 ; real_T
P_189 ; real_T P_190 ; real_T P_191 [ 4 ] ; real_T P_192 ; real_T P_193 ;
real_T P_194 ; real_T P_195 ; real_T P_196 ; real_T P_197 ; real_T P_198 ;
real_T P_199 ; real_T P_200 ; real_T P_201 ; real_T P_202 ; real_T P_203 ;
real_T P_204 ; real_T P_205 ; uint8_T P_206 ; char_T pad_P_206 [ 7 ] ;
P_TrueRMS_DC_AC_VSG_T TrueRMS_b ; P_RMS_DC_AC_VSG_T RMS_m ;
P_TrueRMS_DC_AC_VSG_T TrueRMS_c ; P_RMS_DC_AC_VSG_T RMS_h ; P_PR_DC_AC_VSG_T
PR_l ; P_TrueRMS_DC_AC_VSG_T TrueRMS_o ; P_RMS_DC_AC_VSG_T RMS_p ;
P_TrueRMS_DC_AC_VSG_T TrueRMS ; P_RMS_DC_AC_VSG_T RMS ; P_PR_DC_AC_VSG_T PR ;
} ; extern P_DC_AC_VSG_T DC_AC_VSG_rtDefaultP ;
#endif
