#include "DC_AC_VSG_acc.h"
#include <stddef.h>
#include <float.h>
#include "mwmathutil.h"
#include "rtwtypes.h"
#include "DC_AC_VSG_acc_types.h"
#include "DC_AC_VSG_acc_private.h"
#include <stdio.h>
#include "slexec_vm_simstruct_bridge.h"
#include "slexec_vm_zc_functions.h"
#include "slexec_vm_lookup_functions.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simtarget/slSimTgtMdlrefSfcnBridge.h"
#include "simstruc.h"
#include "fixedpoint.h"
#define CodeFormat S-Function
#define AccDefine1 Accelerator_S-Function
#include "simtarget/slAccSfcnBridge.h"
#ifndef __RTW_UTFREE__  
extern void * utMalloc ( size_t ) ; extern void utFree ( void * ) ;
#endif
boolean_T DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T * bufSzPtr ,
int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T tMinusDelay ,
real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T istransportdelay ,
int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T tail = * tailPtr ; int_T
bufSz = * bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ; real_T * xBuf = (
NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) { numBuffer = 3 ; xBuf
= * uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz - 1 ) ) ? ( tail + 1 )
: 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && ! isfixedbuf ) { int_T j ;
real_T * tempT ; real_T * tempU ; real_T * tempX = ( NULL ) ; real_T * uBuf =
* uBufPtr ; int_T newBufSz = bufSz + 1024 ; if ( newBufSz > * maxNewBufSzPtr
) { * maxNewBufSzPtr = newBufSz ; } tempU = ( real_T * ) utMalloc ( numBuffer
* newBufSz * sizeof ( real_T ) ) ; if ( tempU == ( NULL ) ) { return ( false
) ; } tempT = tempU + newBufSz ; if ( istransportdelay ) tempX = tempT +
newBufSz ; for ( j = tail ; j < bufSz ; j ++ ) { tempT [ j - tail ] = tBuf [
j ] ; tempU [ j - tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j -
tail ] = xBuf [ j ] ; } for ( j = 0 ; j < tail ; j ++ ) { tempT [ j + bufSz -
tail ] = tBuf [ j ] ; tempU [ j + bufSz - tail ] = uBuf [ j ] ; if (
istransportdelay ) tempX [ j + bufSz - tail ] = xBuf [ j ] ; } if ( * lastPtr
> tail ) { * lastPtr -= tail ; } else { * lastPtr += ( bufSz - tail ) ; } *
tailPtr = 0 ; * headPtr = bufSz ; utFree ( uBuf ) ; * bufSzPtr = newBufSz ; *
uBufPtr = tempU ; } else { * tailPtr = testIdx ; } return ( true ) ; } real_T
DC_AC_VSG_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T tStart ,
real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx , int_T
newIdx , real_T initOutput , boolean_T discrete , boolean_T
minorStepAndTAtLastMajorOutput ) { int_T i ; real_T yout , t1 , t2 , u1 , u2
; real_T * tBuf = uBuf + bufSz ; if ( ( newIdx == 0 ) && ( oldestIdx == 0 )
&& ( tMinusDelay > tStart ) ) return initOutput ; if ( tMinusDelay <= tStart
) return initOutput ; if ( ( tMinusDelay <= tBuf [ oldestIdx ] ) ) { if (
discrete ) { return ( uBuf [ oldestIdx ] ) ; } else { int_T tempIdx =
oldestIdx + 1 ; if ( oldestIdx == bufSz - 1 ) tempIdx = 0 ; t1 = tBuf [
oldestIdx ] ; t2 = tBuf [ tempIdx ] ; u1 = uBuf [ oldestIdx ] ; u2 = uBuf [
tempIdx ] ; if ( t2 == t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else {
yout = u1 ; } } else { real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ;
real_T f2 = 1.0 - f1 ; yout = f1 * u1 + f2 * u2 ; } return yout ; } } if (
minorStepAndTAtLastMajorOutput ) { if ( newIdx != 0 ) { if ( * lastIdx ==
newIdx ) { ( * lastIdx ) -- ; } newIdx -- ; } else { if ( * lastIdx == newIdx
) { * lastIdx = bufSz - 1 ; } newIdx = bufSz - 1 ; } } i = * lastIdx ; if (
tBuf [ i ] < tMinusDelay ) { while ( tBuf [ i ] < tMinusDelay ) { if ( i ==
newIdx ) break ; i = ( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } } else { while
( tBuf [ i ] >= tMinusDelay ) { i = ( i > 0 ) ? i - 1 : ( bufSz - 1 ) ; } i =
( i < ( bufSz - 1 ) ) ? ( i + 1 ) : 0 ; } * lastIdx = i ; if ( discrete ) {
double tempEps = ( DBL_EPSILON ) * 128.0 ; double localEps = tempEps *
muDoubleScalarAbs ( tBuf [ i ] ) ; if ( tempEps > localEps ) { localEps =
tempEps ; } localEps = localEps / 2.0 ; if ( tMinusDelay >= ( tBuf [ i ] -
localEps ) ) { yout = uBuf [ i ] ; } else { if ( i == 0 ) { yout = uBuf [
bufSz - 1 ] ; } else { yout = uBuf [ i - 1 ] ; } } } else { if ( i == 0 ) {
t1 = tBuf [ bufSz - 1 ] ; u1 = uBuf [ bufSz - 1 ] ; } else { t1 = tBuf [ i -
1 ] ; u1 = uBuf [ i - 1 ] ; } t2 = tBuf [ i ] ; u2 = uBuf [ i ] ; if ( t2 ==
t1 ) { if ( tMinusDelay >= t2 ) { yout = u2 ; } else { yout = u1 ; } } else {
real_T f1 = ( t2 - tMinusDelay ) / ( t2 - t1 ) ; real_T f2 = 1.0 - f1 ; yout
= f1 * u1 + f2 * u2 ; } } return ( yout ) ; } real_T look1_binlxpw ( real_T
u0 , const real_T bp0 [ ] , const real_T table [ ] , uint32_T maxIndex ) {
real_T frac ; real_T yL_0d0 ; uint32_T bpIdx ; uint32_T iLeft ; uint32_T
iRght ; if ( u0 <= bp0 [ 0U ] ) { iLeft = 0U ; frac = ( u0 - bp0 [ 0U ] ) / (
bp0 [ 1U ] - bp0 [ 0U ] ) ; } else if ( u0 < bp0 [ maxIndex ] ) { bpIdx =
maxIndex >> 1U ; iLeft = 0U ; iRght = maxIndex ; while ( iRght - iLeft > 1U )
{ if ( u0 < bp0 [ bpIdx ] ) { iRght = bpIdx ; } else { iLeft = bpIdx ; }
bpIdx = ( iRght + iLeft ) >> 1U ; } frac = ( u0 - bp0 [ iLeft ] ) / ( bp0 [
iLeft + 1U ] - bp0 [ iLeft ] ) ; } else { iLeft = maxIndex - 1U ; frac = ( u0
- bp0 [ maxIndex - 1U ] ) / ( bp0 [ maxIndex ] - bp0 [ maxIndex - 1U ] ) ; }
yL_0d0 = table [ iLeft ] ; return ( table [ iLeft + 1U ] - yL_0d0 ) * frac +
yL_0d0 ; } void rt_ssGetBlockPath ( void * S , int_T sysIdx , int_T blkIdx ,
char_T * * path ) { _ssGetBlockPath ( ( SimStruct * ) S , sysIdx , blkIdx ,
path ) ; } void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct *
castedS = ( SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( castedS ) )
{ _ssSet_slErrMsg ( castedS , diag ) ; } else { _ssDiscardDiagnostic (
castedS , diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void
* diag ) { _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) {
_ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } void
DC_AC_VSG_PR_Init ( SimStruct * S , X_PR_DC_AC_VSG_T * localX ) { localX ->
TransferFcn_CSTATE_e [ 0 ] = 0.0 ; localX -> TransferFcn_CSTATE_e [ 1 ] = 0.0
; } void DC_AC_VSG_PR ( SimStruct * S , real_T rtu_err , B_PR_DC_AC_VSG_T *
localB , P_PR_DC_AC_VSG_T * localP , X_PR_DC_AC_VSG_T * localX ) { real_T u0
; localB -> B_3_216_1672 = localP -> P_1 * rtu_err ; u0 = ( localP -> P_3 [ 0
] * localX -> TransferFcn_CSTATE_e [ 0 ] + localP -> P_3 [ 1 ] * localX ->
TransferFcn_CSTATE_e [ 1 ] ) + localP -> P_0 * rtu_err ; if ( u0 > localP ->
P_4 ) { localB -> B_3_217_1680 = localP -> P_4 ; } else if ( u0 < localP ->
P_5 ) { localB -> B_3_217_1680 = localP -> P_5 ; } else { localB ->
B_3_217_1680 = u0 ; } } void DC_AC_VSG_PR_Deriv ( SimStruct * S ,
B_PR_DC_AC_VSG_T * localB , P_PR_DC_AC_VSG_T * localP , X_PR_DC_AC_VSG_T *
localX , XDot_PR_DC_AC_VSG_T * localXdot ) { localXdot ->
TransferFcn_CSTATE_e [ 0 ] = 0.0 ; localXdot -> TransferFcn_CSTATE_e [ 0 ] +=
localP -> P_2 [ 0 ] * localX -> TransferFcn_CSTATE_e [ 0 ] ; localXdot ->
TransferFcn_CSTATE_e [ 1 ] = 0.0 ; localXdot -> TransferFcn_CSTATE_e [ 0 ] +=
localP -> P_2 [ 1 ] * localX -> TransferFcn_CSTATE_e [ 1 ] ; localXdot ->
TransferFcn_CSTATE_e [ 1 ] += localX -> TransferFcn_CSTATE_e [ 0 ] ;
localXdot -> TransferFcn_CSTATE_e [ 0 ] += localB -> B_3_216_1672 ; } void
DC_AC_VSG_PR_Term ( SimStruct * const S ) { UNUSED_PARAMETER ( S ) ; } void
DC_AC_VSG_RMS_Init ( SimStruct * S , B_RMS_DC_AC_VSG_T * localB ,
DW_RMS_DC_AC_VSG_T * localDW , P_RMS_DC_AC_VSG_T * localP , X_RMS_DC_AC_VSG_T
* localX ) { localX -> integrator_CSTATE_ld = localP -> P_3 ; localDW ->
Memory_PreviousInput = localP -> P_7 ; localX -> integrator_CSTATE_f = localP
-> P_8 ; localDW -> Memory_PreviousInput_c = localP -> P_12 ; localB ->
B_7_215_1664 = localP -> P_2 ; } void DC_AC_VSG_RMS_Disable ( SimStruct * S ,
DW_RMS_DC_AC_VSG_T * localDW ) { localDW -> RMS_MODE = false ; } void
DC_AC_VSG_RMS ( SimStruct * S , boolean_T rtu_Enable , real_T rtu_In ,
B_RMS_DC_AC_VSG_T * localB , DW_RMS_DC_AC_VSG_T * localDW , P_RMS_DC_AC_VSG_T
* localP , X_RMS_DC_AC_VSG_T * localX , XDis_RMS_DC_AC_VSG_T * localXdis ) {
real_T B_7_464_3568 ; real_T B_7_466_3584 ; int32_T isHit ; boolean_T
rtb_B_7_476_3664 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 )
&& ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if ( ! localDW ->
RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_ld ) , 0 , 2 * sizeof ( boolean_T ) ) ;
localDW -> RMS_MODE = true ; } } else { if ( ssGetTaskTime ( S , 1 ) ==
ssGetTStart ( S ) ) { ( void ) memset ( & ( localXdis -> integrator_CSTATE_ld
) , 1 , 2 * sizeof ( boolean_T ) ) ; } if ( localDW -> RMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_ld ) , 1 , 2 * sizeof ( boolean_T ) ) ;
DC_AC_VSG_RMS_Disable ( S , localDW ) ; } } } if ( localDW -> RMS_MODE ) {
localB -> B_7_205_1584 = localX -> integrator_CSTATE_ld ; { real_T * *
uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - localP ->
P_4 ; B_7_464_3568 = DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 ,
* uBuffer , localDW -> TransportDelay_IWORK . CircularBufSize , & localDW ->
TransportDelay_IWORK . Last , localDW -> TransportDelay_IWORK . Tail ,
localDW -> TransportDelay_IWORK . Head , localP -> P_5 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW -> TransportDelay_IWORK .
CircularBufSize ) [ localDW -> TransportDelay_IWORK . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB ->
B_7_206_1592 = localP -> P_6 ; } rtb_B_7_476_3664 = ( ssGetT ( S ) >= localB
-> B_7_206_1592 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
localB -> B_7_207_1600 = localDW -> Memory_PreviousInput ; } if (
rtb_B_7_476_3664 ) { localB -> B_7_208_1608 = ( localB -> B_7_205_1584 -
B_7_464_3568 ) * localP -> P_1 ; } else { localB -> B_7_208_1608 = localB ->
B_7_207_1600 ; } localB -> B_7_209_1616 = localX -> integrator_CSTATE_f ; {
real_T * * uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK_j .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - localP -> P_9 ; B_7_466_3584 = DC_AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , localDW -> TransportDelay_IWORK_g .
CircularBufSize , & localDW -> TransportDelay_IWORK_g . Last , localDW ->
TransportDelay_IWORK_g . Tail , localDW -> TransportDelay_IWORK_g . Head ,
localP -> P_10 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer
+ localDW -> TransportDelay_IWORK_g . CircularBufSize ) [ localDW ->
TransportDelay_IWORK_g . Head ] == ssGetT ( S ) ) ) ) ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_7_210_1624 =
localP -> P_11 ; } rtb_B_7_476_3664 = ( ssGetT ( S ) >= localB ->
B_7_210_1624 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
localB -> B_7_211_1632 = localDW -> Memory_PreviousInput_c ; } if (
rtb_B_7_476_3664 ) { localB -> B_7_212_1640 = ( localB -> B_7_209_1616 -
B_7_466_3584 ) * localP -> P_0 ; } else { localB -> B_7_212_1640 = localB ->
B_7_211_1632 ; } localB -> B_7_213_1648 = ( muDoubleScalarSin ( localP ->
P_15 * ssGetTaskTime ( S , 0 ) + localP -> P_16 ) * localP -> P_13 + localP
-> P_14 ) * rtu_In ; localB -> B_7_214_1656 = ( muDoubleScalarSin ( localP ->
P_19 * ssGetTaskTime ( S , 0 ) + localP -> P_20 ) * localP -> P_17 + localP
-> P_18 ) * rtu_In ; localB -> B_7_215_1664 = localP -> P_21 *
muDoubleScalarHypot ( localB -> B_7_208_1608 , localB -> B_7_212_1640 ) ; if
( ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW -> RMS_SubsysRanBC )
; } } } void DC_AC_VSG_RMS_Update ( SimStruct * S , B_RMS_DC_AC_VSG_T *
localB , DW_RMS_DC_AC_VSG_T * localDW , P_RMS_DC_AC_VSG_T * localP ) {
int32_T isHit ; if ( localDW -> RMS_MODE ) { { real_T * * uBuffer = ( real_T
* * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime
= ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head = ( ( localDW ->
TransportDelay_IWORK . Head < ( localDW -> TransportDelay_IWORK .
CircularBufSize - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head + 1 ) : 0
) ; if ( localDW -> TransportDelay_IWORK . Head == localDW ->
TransportDelay_IWORK . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize , & localDW -> TransportDelay_IWORK .
Tail , & localDW -> TransportDelay_IWORK . Head , & localDW ->
TransportDelay_IWORK . Last , simTime - localP -> P_4 , uBuffer , ( boolean_T
) 0 , false , & localDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize
) [ localDW -> TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [
localDW -> TransportDelay_IWORK . Head ] = localB -> B_7_205_1584 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput = localB -> B_7_208_1608 ; } { real_T * * uBuffer = (
real_T * * ) & localDW -> TransportDelay_PWORK_j . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; localDW -> TransportDelay_IWORK_g . Head = (
( localDW -> TransportDelay_IWORK_g . Head < ( localDW ->
TransportDelay_IWORK_g . CircularBufSize - 1 ) ) ? ( localDW ->
TransportDelay_IWORK_g . Head + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK_g . Head == localDW -> TransportDelay_IWORK_g . Tail ) {
if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_g . CircularBufSize , & localDW ->
TransportDelay_IWORK_g . Tail , & localDW -> TransportDelay_IWORK_g . Head ,
& localDW -> TransportDelay_IWORK_g . Last , simTime - localP -> P_9 ,
uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK_g .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK_g . CircularBufSize ) [ localDW ->
TransportDelay_IWORK_g . Head ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK_g . Head ] = localB -> B_7_209_1616 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput_c = localB -> B_7_212_1640 ; } } } void
DC_AC_VSG_RMS_Deriv ( SimStruct * S , B_RMS_DC_AC_VSG_T * localB ,
DW_RMS_DC_AC_VSG_T * localDW , XDot_RMS_DC_AC_VSG_T * localXdot ) { if (
localDW -> RMS_MODE ) { localXdot -> integrator_CSTATE_ld = localB ->
B_7_213_1648 ; localXdot -> integrator_CSTATE_f = localB -> B_7_214_1656 ; }
else { { real_T * dx ; int_T i1 ; dx = & ( localXdot -> integrator_CSTATE_ld
) ; for ( i1 = 0 ; i1 < 2 ; i1 ++ ) { dx [ i1 ] = 0.0 ; } } } } void
DC_AC_VSG_RMS_Term ( SimStruct * const S ) { UNUSED_PARAMETER ( S ) ; } void
DC_AC_VSG_TrueRMS_Init ( SimStruct * S , B_TrueRMS_DC_AC_VSG_T * localB ,
DW_TrueRMS_DC_AC_VSG_T * localDW , P_TrueRMS_DC_AC_VSG_T * localP ,
X_TrueRMS_DC_AC_VSG_T * localX ) { localX -> integrator_CSTATE_n = localP ->
P_2 ; localDW -> Memory_PreviousInput = localP -> P_6 ; localB ->
B_9_204_1576 = localP -> P_1 ; } void DC_AC_VSG_TrueRMS_Disable ( SimStruct *
S , DW_TrueRMS_DC_AC_VSG_T * localDW ) { localDW -> TrueRMS_MODE = false ; }
void DC_AC_VSG_TrueRMS ( SimStruct * S , boolean_T rtu_Enable , real_T rtu_In
, B_TrueRMS_DC_AC_VSG_T * localB , DW_TrueRMS_DC_AC_VSG_T * localDW ,
P_TrueRMS_DC_AC_VSG_T * localP , X_TrueRMS_DC_AC_VSG_T * localX ,
XDis_TrueRMS_DC_AC_VSG_T * localXdis ) { real_T B_9_458_3512 ; real_T tmp ;
int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( ( isHit != 0 ) &&
ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if ( ! localDW ->
TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_n ) , 0 , 1 * sizeof ( boolean_T ) ) ; localDW
-> TrueRMS_MODE = true ; } } else { if ( ssGetTaskTime ( S , 1 ) ==
ssGetTStart ( S ) ) { ( void ) memset ( & ( localXdis -> integrator_CSTATE_n
) , 1 , 1 * sizeof ( boolean_T ) ) ; } if ( localDW -> TrueRMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_n ) , 1 , 1 * sizeof ( boolean_T ) ) ;
DC_AC_VSG_TrueRMS_Disable ( S , localDW ) ; } } } if ( localDW ->
TrueRMS_MODE ) { localB -> B_9_199_1536 = localX -> integrator_CSTATE_n ; {
real_T * * uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - localP -> P_3 ; B_9_458_3512 = DC_AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , localDW -> TransportDelay_IWORK .
CircularBufSize , & localDW -> TransportDelay_IWORK . Last , localDW ->
TransportDelay_IWORK . Tail , localDW -> TransportDelay_IWORK . Head , localP
-> P_4 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer +
localDW -> TransportDelay_IWORK . CircularBufSize ) [ localDW ->
TransportDelay_IWORK . Head ] == ssGetT ( S ) ) ) ) ; } isHit = ssIsSampleHit
( S , 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_9_200_1544 = localP -> P_5 ;
localB -> B_9_201_1552 = localDW -> Memory_PreviousInput ; } if ( ssGetT ( S
) >= localB -> B_9_200_1544 ) { localB -> B_9_202_1560 = ( localB ->
B_9_199_1536 - B_9_458_3512 ) * localP -> P_0 ; } else { localB ->
B_9_202_1560 = localB -> B_9_201_1552 ; } localB -> B_9_203_1568 = rtu_In *
rtu_In ; if ( localB -> B_9_202_1560 > localP -> P_7 ) { tmp = localP -> P_7
; } else if ( localB -> B_9_202_1560 < localP -> P_8 ) { tmp = localP -> P_8
; } else { tmp = localB -> B_9_202_1560 ; } localB -> B_9_204_1576 =
muDoubleScalarSqrt ( tmp ) ; if ( ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC
( localDW -> TrueRMS_SubsysRanBC ) ; } } } void DC_AC_VSG_TrueRMS_Update (
SimStruct * S , B_TrueRMS_DC_AC_VSG_T * localB , DW_TrueRMS_DC_AC_VSG_T *
localDW , P_TrueRMS_DC_AC_VSG_T * localP ) { int32_T isHit ; if ( localDW ->
TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
localDW -> TransportDelay_IWORK . Head = ( ( localDW -> TransportDelay_IWORK
. Head < ( localDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? (
localDW -> TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK . Head == localDW -> TransportDelay_IWORK . Tail ) { if
( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize , & localDW -> TransportDelay_IWORK .
Tail , & localDW -> TransportDelay_IWORK . Head , & localDW ->
TransportDelay_IWORK . Last , simTime - localP -> P_3 , uBuffer , ( boolean_T
) 0 , false , & localDW -> TransportDelay_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize
) [ localDW -> TransportDelay_IWORK . Head ] = simTime ; ( * uBuffer ) [
localDW -> TransportDelay_IWORK . Head ] = localB -> B_9_199_1536 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput = localB -> B_9_202_1560 ; } } } void
DC_AC_VSG_TrueRMS_Deriv ( SimStruct * S , B_TrueRMS_DC_AC_VSG_T * localB ,
DW_TrueRMS_DC_AC_VSG_T * localDW , XDot_TrueRMS_DC_AC_VSG_T * localXdot ) {
if ( localDW -> TrueRMS_MODE ) { localXdot -> integrator_CSTATE_n = localB ->
B_9_203_1568 ; } else { localXdot -> integrator_CSTATE_n = 0.0 ; } } void
DC_AC_VSG_TrueRMS_Term ( SimStruct * const S ) { UNUSED_PARAMETER ( S ) ; }
static void mdlOutputs ( SimStruct * S , int_T tid ) { real_T B_30_218_1688 ;
real_T B_30_220_1704 ; real_T B_30_223_1728 ; real_T B_30_228_1768 ; real_T
B_30_231_1792 ; real_T B_30_249_1936 ; real_T B_30_258_2008 ; real_T
B_30_262_2040 ; real_T B_30_284_2216 ; real_T B_30_286_2232 ; real_T
B_30_289_2256 ; real_T B_30_294_2296 ; real_T B_30_297_2320 ; real_T
B_30_315_2464 ; real_T B_30_324_2536 ; real_T B_30_330_2584 ; B_DC_AC_VSG_T *
_rtB ; DW_DC_AC_VSG_T * _rtDW ; P_DC_AC_VSG_T * _rtP ; XDis_DC_AC_VSG_T *
_rtXdis ; X_DC_AC_VSG_T * _rtX ; real_T rtb_B_30_221_1712 ; real_T
rtb_B_30_224_1736 ; real_T rtb_B_30_229_1776 ; real_T rtb_B_30_232_1800 ;
real_T rtb_B_30_236_1832 ; int32_T isHit ; boolean_T rtb_B_30_377_2960 ;
boolean_T rtb_B_30_382_2965 ; _rtDW = ( ( DW_DC_AC_VSG_T * ) ssGetRootDWork (
S ) ) ; _rtXdis = ( ( XDis_DC_AC_VSG_T * ) ssGetContStateDisabled ( S ) ) ;
_rtX = ( ( X_DC_AC_VSG_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_DC_AC_VSG_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( ( B_DC_AC_VSG_T * )
_ssGetModelBlockIO ( S ) ) ; _rtB -> B_30_0_0 = _rtX -> integrator_CSTATE ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_7 ; B_30_218_1688 = DC_AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK .
CircularBufSize , & _rtDW -> TransportDelay_IWORK . Last , _rtDW ->
TransportDelay_IWORK . Tail , _rtDW -> TransportDelay_IWORK . Head , _rtP ->
P_8 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW ->
TransportDelay_IWORK . CircularBufSize ) [ _rtDW -> TransportDelay_IWORK .
Head ] == ssGetT ( S ) ) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { _rtB -> B_30_1_8 = _rtP -> P_9 ; } rtb_B_30_377_2960 = (
ssGetT ( S ) >= _rtB -> B_30_1_8 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( isHit != 0 ) { _rtB -> B_30_2_16 = _rtDW -> Memory_PreviousInput ; } if (
rtb_B_30_377_2960 ) { _rtB -> B_30_3_24 = ( _rtB -> B_30_0_0 - B_30_218_1688
) * _rtP -> P_0 ; } else { _rtB -> B_30_3_24 = _rtB -> B_30_2_16 ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
30 , 8 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_30_4_32 = _rtX -> integ1_CSTATE
; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP ->
P_12 ; B_30_220_1704 = DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0
, * uBuffer , _rtDW -> T_IWORK . CircularBufSize , & _rtDW -> T_IWORK . Last
, _rtDW -> T_IWORK . Tail , _rtDW -> T_IWORK . Head , _rtP -> P_13 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T_IWORK .
CircularBufSize ) [ _rtDW -> T_IWORK . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_221_1712 = _rtB -> B_30_4_32 - B_30_220_1704 ; _rtB -> B_30_5_40 =
_rtX -> Integ2_CSTATE ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T1_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_15 ; B_30_223_1728 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK . CircularBufSize , & _rtDW -> T1_IWORK . Last , _rtDW -> T1_IWORK .
Tail , _rtDW -> T1_IWORK . Head , _rtP -> P_16 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK .
CircularBufSize ) [ _rtDW -> T1_IWORK . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_224_1736 = _rtB -> B_30_5_40 - B_30_223_1728 ; _rtB -> B_30_6_48 =
_rtX -> integ1_CSTATE_f ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T_PWORK_k . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_18 ; B_30_228_1768 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T_IWORK_o . CircularBufSize , & _rtDW -> T_IWORK_o . Last , _rtDW ->
T_IWORK_o . Tail , _rtDW -> T_IWORK_o . Head , _rtP -> P_19 , 0 , ( boolean_T
) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T_IWORK_o .
CircularBufSize ) [ _rtDW -> T_IWORK_o . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_229_1776 = _rtB -> B_30_6_48 - B_30_228_1768 ; _rtB -> B_30_7_56 =
_rtX -> Integ2_CSTATE_k ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T1_PWORK_k . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_21 ; B_30_231_1792 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK_p . CircularBufSize , & _rtDW -> T1_IWORK_p . Last , _rtDW ->
T1_IWORK_p . Tail , _rtDW -> T1_IWORK_p . Head , _rtP -> P_22 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK_p
. CircularBufSize ) [ _rtDW -> T1_IWORK_p . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_232_1800 = _rtB -> B_30_7_56 - B_30_231_1792 ; rtb_B_30_236_1832 =
muDoubleScalarSqrt ( rtb_B_30_221_1712 * rtb_B_30_221_1712 +
rtb_B_30_224_1736 * rtb_B_30_224_1736 ) * muDoubleScalarSqrt (
rtb_B_30_229_1776 * rtb_B_30_229_1776 + rtb_B_30_232_1800 * rtb_B_30_232_1800
) ; rtb_B_30_221_1712 = ( _rtP -> P_23 * muDoubleScalarAtan2 (
rtb_B_30_224_1736 , rtb_B_30_221_1712 ) - _rtP -> P_24 * muDoubleScalarAtan2
( rtb_B_30_232_1800 , rtb_B_30_229_1776 ) ) * _rtP -> P_25 ; _rtB ->
B_30_8_64 [ 0 ] = rtb_B_30_236_1832 * muDoubleScalarCos ( rtb_B_30_221_1712 )
* _rtP -> P_26 ; _rtB -> B_30_8_64 [ 1 ] = rtb_B_30_236_1832 *
muDoubleScalarSin ( rtb_B_30_221_1712 ) * _rtP -> P_26 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
30 , 41 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_30_23_184 = _rtP -> P_37 * _rtB
-> B_30_10_80 [ 8 ] ; } DC_AC_VSG_TrueRMS ( S , _rtB -> B_30_138_1104 , _rtB
-> B_30_23_184 , & _rtB -> TrueRMS , & _rtDW -> TrueRMS , & _rtP -> TrueRMS ,
& _rtX -> TrueRMS , & _rtXdis -> TrueRMS ) ; DC_AC_VSG_RMS ( S , _rtB ->
B_30_139_1105 , _rtB -> B_30_23_184 , & _rtB -> RMS , & _rtDW -> RMS , & _rtP
-> RMS , & _rtX -> RMS , & _rtXdis -> RMS ) ; if ( _rtB -> B_30_138_1104 ) {
rtb_B_30_236_1832 = _rtB -> TrueRMS . B_9_204_1576 ; } else {
rtb_B_30_236_1832 = _rtB -> RMS . B_7_215_1664 ; } _rtB -> B_30_24_192 = _rtX
-> integrator_CSTATE_j ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_a . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay = simTime - _rtP -> P_39 ; B_30_249_1936 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
TransportDelay_IWORK_f . CircularBufSize , & _rtDW -> TransportDelay_IWORK_f
. Last , _rtDW -> TransportDelay_IWORK_f . Tail , _rtDW ->
TransportDelay_IWORK_f . Head , _rtP -> P_40 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_30_25_200 = _rtP -> P_41 ; } rtb_B_30_377_2960 = ( ssGetT ( S ) >= _rtB ->
B_30_25_200 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_30_26_208 = _rtDW -> Memory_PreviousInput_a ; } if (
rtb_B_30_377_2960 ) { _rtB -> B_30_27_216 = ( _rtB -> B_30_24_192 -
B_30_249_1936 ) * _rtP -> P_2 ; } else { _rtB -> B_30_27_216 = _rtB ->
B_30_26_208 ; } rtb_B_30_221_1712 = _rtB -> B_30_27_216 * _rtB -> B_30_27_216
; DC_AC_VSG_TrueRMS ( S , _rtB -> B_30_140_1106 , _rtB -> B_30_23_184 , &
_rtB -> TrueRMS_o , & _rtDW -> TrueRMS_o , & _rtP -> TrueRMS_o , & _rtX ->
TrueRMS_o , & _rtXdis -> TrueRMS_o ) ; DC_AC_VSG_RMS ( S , _rtB ->
B_30_141_1107 , _rtB -> B_30_23_184 , & _rtB -> RMS_p , & _rtDW -> RMS_p , &
_rtP -> RMS_p , & _rtX -> RMS_p , & _rtXdis -> RMS_p ) ; if ( _rtB ->
B_30_140_1106 ) { rtb_B_30_224_1736 = _rtB -> TrueRMS_o . B_9_204_1576 ; }
else { rtb_B_30_224_1736 = _rtB -> RMS_p . B_7_215_1664 ; } rtb_B_30_224_1736
*= rtb_B_30_224_1736 ; if ( rtb_B_30_224_1736 > _rtP -> P_43 ) {
rtb_B_30_229_1776 = _rtP -> P_43 ; } else if ( rtb_B_30_224_1736 < _rtP ->
P_44 ) { rtb_B_30_229_1776 = _rtP -> P_44 ; } else { rtb_B_30_229_1776 =
rtb_B_30_224_1736 ; } rtb_B_30_221_1712 = ( ( rtb_B_30_236_1832 *
rtb_B_30_236_1832 - rtb_B_30_221_1712 ) - rtb_B_30_224_1736 ) /
rtb_B_30_229_1776 ; if ( rtb_B_30_221_1712 > _rtP -> P_45 ) {
rtb_B_30_221_1712 = _rtP -> P_45 ; } else if ( rtb_B_30_221_1712 < _rtP ->
P_46 ) { rtb_B_30_221_1712 = _rtP -> P_46 ; } _rtB -> B_30_28_224 =
muDoubleScalarSqrt ( rtb_B_30_221_1712 ) ; ssCallAccelRunBlock ( S , 2 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { ssCallAccelRunBlock ( S , 30 , 66 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_30_29_232 = _rtX -> integrator_CSTATE_m ; { real_T * * uBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_48 ; B_30_258_2008 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
TransportDelay_IWORK_k . CircularBufSize , & _rtDW -> TransportDelay_IWORK_k
. Last , _rtDW -> TransportDelay_IWORK_k . Tail , _rtDW ->
TransportDelay_IWORK_k . Head , _rtP -> P_49 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_k .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_k . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_30_30_240 = _rtP -> P_50 ; } rtb_B_30_377_2960 = ( ssGetT ( S ) >= _rtB ->
B_30_30_240 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_30_31_248 = _rtDW -> Memory_PreviousInput_i ; } if (
rtb_B_30_377_2960 ) { _rtB -> B_30_32_256 = ( _rtB -> B_30_29_232 -
B_30_258_2008 ) * _rtP -> P_1 ; } else { _rtB -> B_30_32_256 = _rtB ->
B_30_31_248 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
ssCallAccelRunBlock ( S , 30 , 75 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 30 , 76 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 30 , 77 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_30_33_264 = _rtP -> P_52 * _rtB -> B_30_10_80 [ 5 ] ; _rtB -> B_30_34_272 =
_rtP -> P_53 * _rtB -> B_30_10_80 [ 7 ] ; ssCallAccelRunBlock ( S , 30 , 80 ,
SS_CALL_MDL_OUTPUTS ) ; } rtb_B_30_232_1800 = _rtP -> P_55 * _rtX ->
TransferFcn_CSTATE ; if ( rtb_B_30_232_1800 > _rtP -> P_56 ) {
rtb_B_30_232_1800 = _rtP -> P_56 ; } else if ( rtb_B_30_232_1800 < _rtP ->
P_57 ) { rtb_B_30_232_1800 = _rtP -> P_57 ; } isHit = ssIsSampleHit ( S , 1 ,
0 ) ; if ( isHit != 0 ) { _rtB -> B_30_35_280 = _rtP -> P_58 * _rtB ->
B_30_10_80 [ 6 ] ; } B_30_262_2040 = rtb_B_30_232_1800 - _rtB -> B_30_35_280
; DC_AC_VSG_PR ( S , B_30_262_2040 , & _rtB -> PR , & _rtP -> PR , & _rtX ->
PR ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_30_36_288 = _rtP -> P_59 * _rtB -> B_30_10_80 [ 4 ] ; } _rtB -> B_30_37_296
= ( _rtB -> PR . B_3_217_1680 + _rtB -> B_30_36_288 ) / _rtB -> B_30_33_264 ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock
( S , 30 , 89 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 30 , 90 ,
SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_30_38_304 = ( _rtB -> B_30_115_920 - _rtB
-> B_30_32_256 ) * _rtP -> P_60 + _rtB -> B_30_117_936 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
30 , 94 , SS_CALL_MDL_OUTPUTS ) ; } rtb_B_30_232_1800 = look1_binlxpw (
muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_30_39_312 , _rtB -> B_30_119_952
) , _rtP -> P_62 , _rtP -> P_61 , 2U ) ; rtb_B_30_377_2960 = ( _rtB ->
B_30_37_296 + _rtB -> B_30_118_944 >= rtb_B_30_232_1800 ) ; rtb_B_30_382_2965
= ( _rtP -> P_63 * _rtB -> B_30_37_296 + _rtB -> B_30_118_944 >=
rtb_B_30_232_1800 ) ; _rtB -> B_30_40_320 [ 0 ] = rtb_B_30_377_2960 ; _rtB ->
B_30_40_320 [ 1 ] = ! rtb_B_30_377_2960 ; _rtB -> B_30_40_320 [ 2 ] =
rtb_B_30_382_2965 ; _rtB -> B_30_40_320 [ 3 ] = ! rtb_B_30_382_2965 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_30_44_352 = _rtP
-> P_64 * _rtB -> B_30_23_184 ; } _rtB -> B_30_45_360 = ( muDoubleScalarSin (
_rtP -> P_67 * ssGetTaskTime ( S , 0 ) + _rtP -> P_68 ) * _rtP -> P_65 + _rtP
-> P_66 ) * _rtB -> B_30_44_352 ; _rtB -> B_30_46_368 = ( muDoubleScalarSin (
_rtP -> P_71 * ssGetTaskTime ( S , 0 ) + _rtP -> P_72 ) * _rtP -> P_69 + _rtP
-> P_70 ) * _rtB -> B_30_44_352 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { _rtB -> B_30_47_376 = _rtP -> P_73 * _rtB -> B_30_36_288 ; }
_rtB -> B_30_48_384 = ( muDoubleScalarSin ( _rtP -> P_76 * ssGetTaskTime ( S
, 0 ) + _rtP -> P_77 ) * _rtP -> P_74 + _rtP -> P_75 ) * _rtB -> B_30_47_376
; _rtB -> B_30_49_392 = ( muDoubleScalarSin ( _rtP -> P_80 * ssGetTaskTime (
S , 0 ) + _rtP -> P_81 ) * _rtP -> P_78 + _rtP -> P_79 ) * _rtB ->
B_30_47_376 ; _rtB -> B_30_50_400 = muDoubleScalarSin ( _rtX ->
Integrator1_CSTATE ) * _rtB -> B_30_38_304 - _rtB -> B_30_36_288 ; _rtB ->
B_30_51_408 = 0.0 ; _rtB -> B_30_51_408 += _rtP -> P_86 * _rtX ->
TransferFcn_CSTATE_c ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { ssCallAccelRunBlock ( S , 30 , 140 , SS_CALL_MDL_OUTPUTS ) ; } if ( _rtB
-> B_30_51_408 > _rtP -> P_87 ) { rtb_B_30_229_1776 = _rtP -> P_87 ; } else
if ( _rtB -> B_30_51_408 < _rtP -> P_88 ) { rtb_B_30_229_1776 = _rtP -> P_88
; } else { rtb_B_30_229_1776 = _rtB -> B_30_51_408 ; } _rtB -> B_30_52_416 =
( rtb_B_30_229_1776 + _rtB -> B_30_120_960 ) + _rtB -> B_30_116_928 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
30 , 143 , SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_30_52_416 + 0 ) ; } } } _rtB ->
B_30_53_424 = _rtB -> B_30_110_880 / _rtB -> B_30_52_416 - _rtB -> B_30_3_24
/ _rtB -> B_30_52_416 ; _rtB -> B_30_54_432 = _rtX -> integrator_CSTATE_j0 ;
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_e .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_90 ; B_30_284_2216 = DC_AC_VSG_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_n .
CircularBufSize , & _rtDW -> TransportDelay_IWORK_n . Last , _rtDW ->
TransportDelay_IWORK_n . Tail , _rtDW -> TransportDelay_IWORK_n . Head , _rtP
-> P_91 , 0 , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer +
_rtDW -> TransportDelay_IWORK_n . CircularBufSize ) [ _rtDW ->
TransportDelay_IWORK_n . Head ] == ssGetT ( S ) ) ) ) ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_30_55_440 = _rtP
-> P_92 ; } rtb_B_30_377_2960 = ( ssGetT ( S ) >= _rtB -> B_30_55_440 ) ;
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_30_56_448
= _rtDW -> Memory_PreviousInput_f ; } if ( rtb_B_30_377_2960 ) { _rtB ->
B_30_57_456 = ( _rtB -> B_30_54_432 - B_30_284_2216 ) * _rtP -> P_3 ; } else
{ _rtB -> B_30_57_456 = _rtB -> B_30_56_448 ; } isHit = ssIsSampleHit ( S , 1
, 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 30 , 168 ,
SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_30_58_464 = _rtX -> integ1_CSTATE_l ; {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> T_PWORK_f . TUbufferPtrs [ 0 ]
; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_95
; B_30_286_2232 = DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
uBuffer , _rtDW -> T_IWORK_d . CircularBufSize , & _rtDW -> T_IWORK_d . Last
, _rtDW -> T_IWORK_d . Tail , _rtDW -> T_IWORK_d . Head , _rtP -> P_96 , 0 ,
( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T_IWORK_d
. CircularBufSize ) [ _rtDW -> T_IWORK_d . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_236_1832 = _rtB -> B_30_58_464 - B_30_286_2232 ; _rtB -> B_30_59_472
= _rtX -> Integ2_CSTATE_b ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T1_PWORK_o . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_98 ; B_30_289_2256 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK_c . CircularBufSize , & _rtDW -> T1_IWORK_c . Last , _rtDW ->
T1_IWORK_c . Tail , _rtDW -> T1_IWORK_c . Head , _rtP -> P_99 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK_c
. CircularBufSize ) [ _rtDW -> T1_IWORK_c . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_221_1712 = _rtB -> B_30_59_472 - B_30_289_2256 ; _rtB -> B_30_60_480
= _rtX -> integ1_CSTATE_j ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T_PWORK_n . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_101 ; B_30_294_2296 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T_IWORK_k . CircularBufSize , & _rtDW -> T_IWORK_k . Last , _rtDW ->
T_IWORK_k . Tail , _rtDW -> T_IWORK_k . Head , _rtP -> P_102 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T_IWORK_k .
CircularBufSize ) [ _rtDW -> T_IWORK_k . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_224_1736 = _rtB -> B_30_60_480 - B_30_294_2296 ; _rtB -> B_30_61_488
= _rtX -> Integ2_CSTATE_e ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T1_PWORK_b . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T
tMinusDelay = simTime - _rtP -> P_104 ; B_30_297_2320 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
T1_IWORK_e . CircularBufSize , & _rtDW -> T1_IWORK_e . Last , _rtDW ->
T1_IWORK_e . Tail , _rtDW -> T1_IWORK_e . Head , _rtP -> P_105 , 0 , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> T1_IWORK_e
. CircularBufSize ) [ _rtDW -> T1_IWORK_e . Head ] == ssGetT ( S ) ) ) ) ; }
rtb_B_30_229_1776 = _rtB -> B_30_61_488 - B_30_297_2320 ; rtb_B_30_232_1800 =
muDoubleScalarSqrt ( rtb_B_30_236_1832 * rtb_B_30_236_1832 +
rtb_B_30_221_1712 * rtb_B_30_221_1712 ) * muDoubleScalarSqrt (
rtb_B_30_224_1736 * rtb_B_30_224_1736 + rtb_B_30_229_1776 * rtb_B_30_229_1776
) ; rtb_B_30_236_1832 = ( _rtP -> P_106 * muDoubleScalarAtan2 (
rtb_B_30_221_1712 , rtb_B_30_236_1832 ) - _rtP -> P_107 * muDoubleScalarAtan2
( rtb_B_30_229_1776 , rtb_B_30_224_1736 ) ) * _rtP -> P_108 ; _rtB ->
B_30_62_496 [ 0 ] = rtb_B_30_232_1800 * muDoubleScalarCos ( rtb_B_30_236_1832
) * _rtP -> P_109 ; _rtB -> B_30_62_496 [ 1 ] = rtb_B_30_232_1800 *
muDoubleScalarSin ( rtb_B_30_236_1832 ) * _rtP -> P_109 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
30 , 201 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_30_77_616 = _rtP -> P_120 * _rtB
-> B_30_64_512 [ 8 ] ; } DC_AC_VSG_TrueRMS ( S , _rtB -> B_30_142_1108 , _rtB
-> B_30_77_616 , & _rtB -> TrueRMS_c , & _rtDW -> TrueRMS_c , & _rtP ->
TrueRMS_c , & _rtX -> TrueRMS_c , & _rtXdis -> TrueRMS_c ) ; DC_AC_VSG_RMS (
S , _rtB -> B_30_143_1109 , _rtB -> B_30_77_616 , & _rtB -> RMS_h , & _rtDW
-> RMS_h , & _rtP -> RMS_h , & _rtX -> RMS_h , & _rtXdis -> RMS_h ) ; if (
_rtB -> B_30_142_1108 ) { rtb_B_30_232_1800 = _rtB -> TrueRMS_c .
B_9_204_1576 ; } else { rtb_B_30_232_1800 = _rtB -> RMS_h . B_7_215_1664 ; }
_rtB -> B_30_78_624 = _rtX -> integrator_CSTATE_l ; { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_122 ;
B_30_315_2464 = DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , *
uBuffer , _rtDW -> TransportDelay_IWORK_e . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_e . Last , _rtDW -> TransportDelay_IWORK_e . Tail ,
_rtDW -> TransportDelay_IWORK_e . Head , _rtP -> P_123 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_e .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_e . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_30_79_632 = _rtP -> P_124 ; } rtb_B_30_377_2960 = ( ssGetT ( S ) >= _rtB ->
B_30_79_632 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_30_80_640 = _rtDW -> Memory_PreviousInput_k ; } if (
rtb_B_30_377_2960 ) { _rtB -> B_30_81_648 = ( _rtB -> B_30_78_624 -
B_30_315_2464 ) * _rtP -> P_5 ; } else { _rtB -> B_30_81_648 = _rtB ->
B_30_80_640 ; } rtb_B_30_236_1832 = _rtB -> B_30_81_648 * _rtB -> B_30_81_648
; DC_AC_VSG_TrueRMS ( S , _rtB -> B_30_144_1110 , _rtB -> B_30_77_616 , &
_rtB -> TrueRMS_b , & _rtDW -> TrueRMS_b , & _rtP -> TrueRMS_b , & _rtX ->
TrueRMS_b , & _rtXdis -> TrueRMS_b ) ; DC_AC_VSG_RMS ( S , _rtB ->
B_30_145_1111 , _rtB -> B_30_77_616 , & _rtB -> RMS_m , & _rtDW -> RMS_m , &
_rtP -> RMS_m , & _rtX -> RMS_m , & _rtXdis -> RMS_m ) ; if ( _rtB ->
B_30_144_1110 ) { rtb_B_30_221_1712 = _rtB -> TrueRMS_b . B_9_204_1576 ; }
else { rtb_B_30_221_1712 = _rtB -> RMS_m . B_7_215_1664 ; } rtb_B_30_221_1712
*= rtb_B_30_221_1712 ; if ( rtb_B_30_221_1712 > _rtP -> P_126 ) {
rtb_B_30_229_1776 = _rtP -> P_126 ; } else if ( rtb_B_30_221_1712 < _rtP ->
P_127 ) { rtb_B_30_229_1776 = _rtP -> P_127 ; } else { rtb_B_30_229_1776 =
rtb_B_30_221_1712 ; } rtb_B_30_221_1712 = ( ( rtb_B_30_232_1800 *
rtb_B_30_232_1800 - rtb_B_30_236_1832 ) - rtb_B_30_221_1712 ) /
rtb_B_30_229_1776 ; if ( rtb_B_30_221_1712 > _rtP -> P_128 ) {
rtb_B_30_221_1712 = _rtP -> P_128 ; } else if ( rtb_B_30_221_1712 < _rtP ->
P_129 ) { rtb_B_30_221_1712 = _rtP -> P_129 ; } _rtB -> B_30_82_656 =
muDoubleScalarSqrt ( rtb_B_30_221_1712 ) ; ssCallAccelRunBlock ( S , 17 , 0 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { ssCallAccelRunBlock ( S , 30 , 226 , SS_CALL_MDL_OUTPUTS ) ; } _rtB ->
B_30_83_664 = _rtX -> integrator_CSTATE_b ; { real_T * * uBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_l . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; real_T tMinusDelay = simTime - _rtP -> P_131 ; B_30_324_2536 =
DC_AC_VSG_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer , _rtDW ->
TransportDelay_IWORK_i . CircularBufSize , & _rtDW -> TransportDelay_IWORK_i
. Last , _rtDW -> TransportDelay_IWORK_i . Tail , _rtDW ->
TransportDelay_IWORK_i . Head , _rtP -> P_132 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_i .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_i . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_30_84_672 = _rtP -> P_133 ; } rtb_B_30_377_2960 = ( ssGetT ( S ) >= _rtB ->
B_30_84_672 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_30_85_680 = _rtDW -> Memory_PreviousInput_p ; } if (
rtb_B_30_377_2960 ) { _rtB -> B_30_86_688 = ( _rtB -> B_30_83_664 -
B_30_324_2536 ) * _rtP -> P_4 ; } else { _rtB -> B_30_86_688 = _rtB ->
B_30_85_680 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
ssCallAccelRunBlock ( S , 30 , 235 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 30 , 236 , SS_CALL_MDL_OUTPUTS ) ;
ssCallAccelRunBlock ( S , 30 , 237 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_30_87_696 = _rtP -> P_135 * _rtB -> B_30_64_512 [ 5 ] ; _rtB -> B_30_88_704
= _rtP -> P_136 * _rtB -> B_30_64_512 [ 7 ] ; ssCallAccelRunBlock ( S , 30 ,
240 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_30_89_712 = _rtP -> P_137 * _rtB ->
B_30_64_512 [ 4 ] ; ssCallAccelRunBlock ( S , 30 , 242 , SS_CALL_MDL_OUTPUTS
) ; } _rtB -> B_30_90_720 = ( _rtB -> B_30_127_1016 - _rtB -> B_30_86_688 ) *
_rtP -> P_138 + _rtB -> B_30_128_1024 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ;
if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 30 , 246 , SS_CALL_MDL_OUTPUTS
) ; _rtB -> B_30_91_728 = _rtP -> P_139 * _rtB -> B_30_64_512 [ 6 ] ; { if (
_rtDW -> TAQSigLogging_InsertedFor_Cm0_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Cm0_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_30_91_728 + 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_Cm3_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Cm3_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_30_77_616 + 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_Vm0_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Vm0_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_30_89_712 + 0 ) ; } } } rtb_B_30_221_1712
= _rtP -> P_141 * _rtX -> TransferFcn_CSTATE_a ; if ( rtb_B_30_221_1712 >
_rtP -> P_142 ) { rtb_B_30_221_1712 = _rtP -> P_142 ; } else if (
rtb_B_30_221_1712 < _rtP -> P_143 ) { rtb_B_30_221_1712 = _rtP -> P_143 ; }
B_30_330_2584 = rtb_B_30_221_1712 - _rtB -> B_30_91_728 ; DC_AC_VSG_PR ( S ,
B_30_330_2584 , & _rtB -> PR_l , & _rtP -> PR_l , & _rtX -> PR_l ) ;
rtb_B_30_221_1712 = ( _rtB -> PR_l . B_3_217_1680 + _rtB -> B_30_89_712 ) /
_rtB -> B_30_87_696 ; rtb_B_30_232_1800 = look1_binlxpw ( muDoubleScalarRem (
ssGetT ( S ) - _rtB -> B_30_92_736 , _rtB -> B_30_130_1040 ) , _rtP -> P_145
, _rtP -> P_144 , 2U ) ; rtb_B_30_377_2960 = ( rtb_B_30_221_1712 + _rtB ->
B_30_129_1032 >= rtb_B_30_232_1800 ) ; rtb_B_30_382_2965 = ( _rtP -> P_146 *
rtb_B_30_221_1712 + _rtB -> B_30_129_1032 >= rtb_B_30_232_1800 ) ; _rtB ->
B_30_93_744 [ 0 ] = rtb_B_30_377_2960 ; _rtB -> B_30_93_744 [ 1 ] = !
rtb_B_30_377_2960 ; _rtB -> B_30_93_744 [ 2 ] = rtb_B_30_382_2965 ; _rtB ->
B_30_93_744 [ 3 ] = ! rtb_B_30_382_2965 ; isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { _rtB -> B_30_97_776 = _rtP -> P_147 * _rtB ->
B_30_77_616 ; } _rtB -> B_30_98_784 = ( muDoubleScalarSin ( _rtP -> P_150 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_151 ) * _rtP -> P_148 + _rtP -> P_149 ) *
_rtB -> B_30_97_776 ; _rtB -> B_30_99_792 = ( muDoubleScalarSin ( _rtP ->
P_154 * ssGetTaskTime ( S , 0 ) + _rtP -> P_155 ) * _rtP -> P_152 + _rtP ->
P_153 ) * _rtB -> B_30_97_776 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { _rtB -> B_30_100_800 = _rtP -> P_156 * _rtB -> B_30_89_712 ; }
_rtB -> B_30_101_808 = ( muDoubleScalarSin ( _rtP -> P_159 * ssGetTaskTime (
S , 0 ) + _rtP -> P_160 ) * _rtP -> P_157 + _rtP -> P_158 ) * _rtB ->
B_30_100_800 ; _rtB -> B_30_102_816 = ( muDoubleScalarSin ( _rtP -> P_163 *
ssGetTaskTime ( S , 0 ) + _rtP -> P_164 ) * _rtP -> P_161 + _rtP -> P_162 ) *
_rtB -> B_30_100_800 ; if ( _rtP -> P_206 == 1 ) { rtb_B_30_229_1776 = _rtX
-> Integrator1_CSTATE_l ; } else { rtb_B_30_229_1776 = _rtX ->
Integrator2_CSTATE ; } _rtB -> B_30_103_824 = muDoubleScalarSin (
rtb_B_30_229_1776 ) * _rtB -> B_30_90_720 - _rtB -> B_30_89_712 ; _rtB ->
B_30_104_832 = 0.0 ; _rtB -> B_30_104_832 += _rtP -> P_172 * _rtX ->
uJsD_CSTATE ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
ssCallAccelRunBlock ( S , 30 , 304 , SS_CALL_MDL_OUTPUTS ) ; } if ( _rtB ->
B_30_104_832 > _rtP -> P_173 ) { rtb_B_30_229_1776 = _rtP -> P_173 ; } else
if ( _rtB -> B_30_104_832 < _rtP -> P_174 ) { rtb_B_30_229_1776 = _rtP ->
P_174 ; } else { rtb_B_30_229_1776 = _rtB -> B_30_104_832 ; } _rtB ->
B_30_105_840 = rtb_B_30_229_1776 + _rtB -> B_30_133_1064 ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
30 , 307 , SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_30_106_848 = 0.0 ; _rtB ->
B_30_106_848 += _rtP -> P_176 * _rtX -> uJw0s_CSTATE ; isHit = ssIsSampleHit
( S , 1 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 30 , 309 ,
SS_CALL_MDL_OUTPUTS ) ; } if ( _rtB -> B_30_106_848 > _rtP -> P_177 ) {
rtb_B_30_221_1712 = _rtP -> P_177 ; } else if ( _rtB -> B_30_106_848 < _rtP
-> P_178 ) { rtb_B_30_221_1712 = _rtP -> P_178 ; } else { rtb_B_30_221_1712 =
_rtB -> B_30_106_848 ; } _rtB -> B_30_107_856 = ( rtb_B_30_221_1712 + _rtB ->
B_30_132_1056 ) + _rtB -> B_30_126_1008 ; isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { ssCallAccelRunBlock ( S , 30 , 312 ,
SS_CALL_MDL_OUTPUTS ) ; { if ( _rtDW ->
TAQSigLogging_InsertedFor_Add4_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Add4_at_outport_0_PWORK . AQHandles , ssGetTaskTime
( S , 1 ) , ( char * ) & _rtB -> B_30_105_840 + 0 ) ; } } { if ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK_b . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Add5_at_outport_0_PWORK_b . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_30_107_856 + 0 ) ; } } }
_rtB -> B_30_108_864 = ( _rtB -> B_30_121_968 / _rtB -> B_30_107_856 - _rtB
-> B_30_57_456 / _rtB -> B_30_107_856 ) - rtb_B_30_221_1712 * _rtB ->
B_30_131_1048 ; _rtB -> B_30_109_872 = _rtB -> B_30_121_968 / _rtB ->
B_30_105_840 - _rtB -> B_30_57_456 / _rtB -> B_30_105_840 ; UNUSED_PARAMETER
( tid ) ; } static void mdlOutputsTID2 ( SimStruct * S , int_T tid ) {
B_DC_AC_VSG_T * _rtB ; P_DC_AC_VSG_T * _rtP ; _rtP = ( ( P_DC_AC_VSG_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_DC_AC_VSG_T * ) _ssGetModelBlockIO ( S )
) ; _rtB -> B_30_110_880 = _rtP -> P_179 ; _rtB -> B_30_115_920 = _rtP ->
P_181 ; _rtB -> B_30_116_928 = _rtP -> P_182 ; _rtB -> B_30_138_1104 = ( _rtP
-> P_183 != 0.0 ) ; _rtB -> B_30_139_1105 = ! _rtB -> B_30_138_1104 ; _rtB ->
B_30_140_1106 = ( _rtP -> P_184 != 0.0 ) ; _rtB -> B_30_141_1107 = ! _rtB ->
B_30_140_1106 ; _rtB -> B_30_117_936 = _rtP -> P_185 ; _rtB -> B_30_118_944 =
_rtP -> P_186 ; _rtB -> B_30_119_952 = _rtP -> P_187 ; _rtB -> B_30_120_960 =
_rtP -> P_189 * _rtP -> P_188 ; _rtB -> B_30_121_968 = _rtP -> P_190 ; _rtB
-> B_30_111_888 [ 0 ] = _rtP -> P_180 [ 0 ] ; _rtB -> B_30_122_976 [ 0 ] =
_rtP -> P_191 [ 0 ] ; _rtB -> B_30_111_888 [ 1 ] = _rtP -> P_180 [ 1 ] ; _rtB
-> B_30_122_976 [ 1 ] = _rtP -> P_191 [ 1 ] ; _rtB -> B_30_111_888 [ 2 ] =
_rtP -> P_180 [ 2 ] ; _rtB -> B_30_122_976 [ 2 ] = _rtP -> P_191 [ 2 ] ; _rtB
-> B_30_111_888 [ 3 ] = _rtP -> P_180 [ 3 ] ; _rtB -> B_30_122_976 [ 3 ] =
_rtP -> P_191 [ 3 ] ; _rtB -> B_30_126_1008 = _rtP -> P_192 ; _rtB ->
B_30_127_1016 = _rtP -> P_193 ; _rtB -> B_30_142_1108 = ( _rtP -> P_194 !=
0.0 ) ; _rtB -> B_30_143_1109 = ! _rtB -> B_30_142_1108 ; _rtB ->
B_30_144_1110 = ( _rtP -> P_195 != 0.0 ) ; _rtB -> B_30_145_1111 = ! _rtB ->
B_30_144_1110 ; _rtB -> B_30_128_1024 = _rtP -> P_196 ; _rtB -> B_30_129_1032
= _rtP -> P_197 ; _rtB -> B_30_130_1040 = _rtP -> P_198 ; _rtB ->
B_30_131_1048 = _rtP -> P_199 ; _rtB -> B_30_132_1056 = _rtP -> P_201 * _rtP
-> P_200 ; _rtB -> B_30_133_1064 = _rtP -> P_203 * _rtP -> P_202 ; _rtB ->
B_30_134_1072 = _rtP -> P_204 ; _rtB -> B_30_135_1080 = _rtP -> P_205 ;
UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { B_DC_AC_VSG_T * _rtB ;
DW_DC_AC_VSG_T * _rtDW ; P_DC_AC_VSG_T * _rtP ; int32_T isHit ; _rtDW = ( (
DW_DC_AC_VSG_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( ( P_DC_AC_VSG_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_DC_AC_VSG_T * ) _ssGetModelBlockIO ( S )
) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK . Head = ( ( _rtDW -> TransportDelay_IWORK . Head < (
_rtDW -> TransportDelay_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK
. Head == _rtDW -> TransportDelay_IWORK . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> TransportDelay_IWORK
. CircularBufSize , & _rtDW -> TransportDelay_IWORK . Tail , & _rtDW ->
TransportDelay_IWORK . Head , & _rtDW -> TransportDelay_IWORK . Last ,
simTime - _rtP -> P_7 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T *
) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK . CircularBufSize ) [ _rtDW -> TransportDelay_IWORK .
Head ] = simTime ; ( * uBuffer ) [ _rtDW -> TransportDelay_IWORK . Head ] =
_rtB -> B_30_0_0 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 )
{ _rtDW -> Memory_PreviousInput = _rtB -> B_30_3_24 ; } { real_T * * uBuffer
= ( real_T * * ) & _rtDW -> T_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; _rtDW -> T_IWORK . Head = ( ( _rtDW -> T_IWORK . Head < (
_rtDW -> T_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW -> T_IWORK . Head + 1 )
: 0 ) ; if ( _rtDW -> T_IWORK . Head == _rtDW -> T_IWORK . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T_IWORK .
CircularBufSize , & _rtDW -> T_IWORK . Tail , & _rtDW -> T_IWORK . Head , &
_rtDW -> T_IWORK . Last , simTime - _rtP -> P_12 , uBuffer , ( boolean_T ) 0
, false , & _rtDW -> T_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S , (
char_T * ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer
+ _rtDW -> T_IWORK . CircularBufSize ) [ _rtDW -> T_IWORK . Head ] = simTime
; ( * uBuffer ) [ _rtDW -> T_IWORK . Head ] = _rtB -> B_30_4_32 ; } { real_T
* * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK . Head = ( ( _rtDW ->
T1_IWORK . Head < ( _rtDW -> T1_IWORK . CircularBufSize - 1 ) ) ? ( _rtDW ->
T1_IWORK . Head + 1 ) : 0 ) ; if ( _rtDW -> T1_IWORK . Head == _rtDW ->
T1_IWORK . Tail ) { if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> T1_IWORK . CircularBufSize , & _rtDW -> T1_IWORK . Tail , & _rtDW ->
T1_IWORK . Head , & _rtDW -> T1_IWORK . Last , simTime - _rtP -> P_15 ,
uBuffer , ( boolean_T ) 0 , false , & _rtDW -> T1_IWORK . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + _rtDW -> T1_IWORK . CircularBufSize ) [ _rtDW ->
T1_IWORK . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK . Head ] =
_rtB -> B_30_5_40 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T_PWORK_k . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
T_IWORK_o . Head = ( ( _rtDW -> T_IWORK_o . Head < ( _rtDW -> T_IWORK_o .
CircularBufSize - 1 ) ) ? ( _rtDW -> T_IWORK_o . Head + 1 ) : 0 ) ; if (
_rtDW -> T_IWORK_o . Head == _rtDW -> T_IWORK_o . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T_IWORK_o .
CircularBufSize , & _rtDW -> T_IWORK_o . Tail , & _rtDW -> T_IWORK_o . Head ,
& _rtDW -> T_IWORK_o . Last , simTime - _rtP -> P_18 , uBuffer , ( boolean_T
) 0 , false , & _rtDW -> T_IWORK_o . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, ( char_T * ) "\"tdelay memory allocation error\"" ) ; return ; } } ( *
uBuffer + _rtDW -> T_IWORK_o . CircularBufSize ) [ _rtDW -> T_IWORK_o . Head
] = simTime ; ( * uBuffer ) [ _rtDW -> T_IWORK_o . Head ] = _rtB -> B_30_6_48
; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK_k .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK_p .
Head = ( ( _rtDW -> T1_IWORK_p . Head < ( _rtDW -> T1_IWORK_p .
CircularBufSize - 1 ) ) ? ( _rtDW -> T1_IWORK_p . Head + 1 ) : 0 ) ; if (
_rtDW -> T1_IWORK_p . Head == _rtDW -> T1_IWORK_p . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T1_IWORK_p .
CircularBufSize , & _rtDW -> T1_IWORK_p . Tail , & _rtDW -> T1_IWORK_p . Head
, & _rtDW -> T1_IWORK_p . Last , simTime - _rtP -> P_21 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> T1_IWORK_p . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + _rtDW -> T1_IWORK_p . CircularBufSize ) [ _rtDW ->
T1_IWORK_p . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK_p . Head ]
= _rtB -> B_30_7_56 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0
) { ssCallAccelRunBlock ( S , 30 , 41 , SS_CALL_MDL_UPDATE ) ; }
DC_AC_VSG_TrueRMS_Update ( S , & _rtB -> TrueRMS , & _rtDW -> TrueRMS , &
_rtP -> TrueRMS ) ; DC_AC_VSG_RMS_Update ( S , & _rtB -> RMS , & _rtDW -> RMS
, & _rtP -> RMS ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_a . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_f . Head = ( ( _rtDW -> TransportDelay_IWORK_f
. Head < ( _rtDW -> TransportDelay_IWORK_f . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_f . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_f . Head == _rtDW -> TransportDelay_IWORK_f . Tail ) {
if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_f . CircularBufSize , & _rtDW -> TransportDelay_IWORK_f
. Tail , & _rtDW -> TransportDelay_IWORK_f . Head , & _rtDW ->
TransportDelay_IWORK_f . Last , simTime - _rtP -> P_39 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_f . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_f . Head ] = _rtB -> B_30_24_192 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_a = _rtB -> B_30_27_216 ; } DC_AC_VSG_TrueRMS_Update ( S
, & _rtB -> TrueRMS_o , & _rtDW -> TrueRMS_o , & _rtP -> TrueRMS_o ) ;
DC_AC_VSG_RMS_Update ( S , & _rtB -> RMS_p , & _rtDW -> RMS_p , & _rtP ->
RMS_p ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_h . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_k . Head = ( ( _rtDW -> TransportDelay_IWORK_k
. Head < ( _rtDW -> TransportDelay_IWORK_k . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_k . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_k . Head == _rtDW -> TransportDelay_IWORK_k . Tail ) {
if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_k . CircularBufSize , & _rtDW -> TransportDelay_IWORK_k
. Tail , & _rtDW -> TransportDelay_IWORK_k . Head , & _rtDW ->
TransportDelay_IWORK_k . Last , simTime - _rtP -> P_48 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_k . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_k .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_k . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_k . Head ] = _rtB -> B_30_29_232 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_i = _rtB -> B_30_32_256 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> TransportDelay_PWORK_e . TUbufferPtrs [ 0 ] ; real_T
simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_n . Head = ( ( _rtDW
-> TransportDelay_IWORK_n . Head < ( _rtDW -> TransportDelay_IWORK_n .
CircularBufSize - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_n . Head + 1 ) : 0
) ; if ( _rtDW -> TransportDelay_IWORK_n . Head == _rtDW ->
TransportDelay_IWORK_n . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize , & _rtDW -> TransportDelay_IWORK_n
. Tail , & _rtDW -> TransportDelay_IWORK_n . Head , & _rtDW ->
TransportDelay_IWORK_n . Last , simTime - _rtP -> P_90 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_n . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_n .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_n . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_n . Head ] = _rtB -> B_30_54_432 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_f = _rtB -> B_30_57_456 ; } { real_T * * uBuffer = (
real_T * * ) & _rtDW -> T_PWORK_f . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; _rtDW -> T_IWORK_d . Head = ( ( _rtDW -> T_IWORK_d . Head < (
_rtDW -> T_IWORK_d . CircularBufSize - 1 ) ) ? ( _rtDW -> T_IWORK_d . Head +
1 ) : 0 ) ; if ( _rtDW -> T_IWORK_d . Head == _rtDW -> T_IWORK_d . Tail ) {
if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T_IWORK_d .
CircularBufSize , & _rtDW -> T_IWORK_d . Tail , & _rtDW -> T_IWORK_d . Head ,
& _rtDW -> T_IWORK_d . Last , simTime - _rtP -> P_95 , uBuffer , ( boolean_T
) 0 , false , & _rtDW -> T_IWORK_d . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, ( char_T * ) "\"tdelay memory allocation error\"" ) ; return ; } } ( *
uBuffer + _rtDW -> T_IWORK_d . CircularBufSize ) [ _rtDW -> T_IWORK_d . Head
] = simTime ; ( * uBuffer ) [ _rtDW -> T_IWORK_d . Head ] = _rtB ->
B_30_58_464 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK_o .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK_c .
Head = ( ( _rtDW -> T1_IWORK_c . Head < ( _rtDW -> T1_IWORK_c .
CircularBufSize - 1 ) ) ? ( _rtDW -> T1_IWORK_c . Head + 1 ) : 0 ) ; if (
_rtDW -> T1_IWORK_c . Head == _rtDW -> T1_IWORK_c . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T1_IWORK_c .
CircularBufSize , & _rtDW -> T1_IWORK_c . Tail , & _rtDW -> T1_IWORK_c . Head
, & _rtDW -> T1_IWORK_c . Last , simTime - _rtP -> P_98 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> T1_IWORK_c . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + _rtDW -> T1_IWORK_c . CircularBufSize ) [ _rtDW ->
T1_IWORK_c . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK_c . Head ]
= _rtB -> B_30_59_472 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
T_PWORK_n . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
T_IWORK_k . Head = ( ( _rtDW -> T_IWORK_k . Head < ( _rtDW -> T_IWORK_k .
CircularBufSize - 1 ) ) ? ( _rtDW -> T_IWORK_k . Head + 1 ) : 0 ) ; if (
_rtDW -> T_IWORK_k . Head == _rtDW -> T_IWORK_k . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T_IWORK_k .
CircularBufSize , & _rtDW -> T_IWORK_k . Tail , & _rtDW -> T_IWORK_k . Head ,
& _rtDW -> T_IWORK_k . Last , simTime - _rtP -> P_101 , uBuffer , ( boolean_T
) 0 , false , & _rtDW -> T_IWORK_k . MaxNewBufSize ) ) { ssSetErrorStatus ( S
, ( char_T * ) "\"tdelay memory allocation error\"" ) ; return ; } } ( *
uBuffer + _rtDW -> T_IWORK_k . CircularBufSize ) [ _rtDW -> T_IWORK_k . Head
] = simTime ; ( * uBuffer ) [ _rtDW -> T_IWORK_k . Head ] = _rtB ->
B_30_60_480 ; } { real_T * * uBuffer = ( real_T * * ) & _rtDW -> T1_PWORK_b .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW -> T1_IWORK_e .
Head = ( ( _rtDW -> T1_IWORK_e . Head < ( _rtDW -> T1_IWORK_e .
CircularBufSize - 1 ) ) ? ( _rtDW -> T1_IWORK_e . Head + 1 ) : 0 ) ; if (
_rtDW -> T1_IWORK_e . Head == _rtDW -> T1_IWORK_e . Tail ) { if ( !
DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW -> T1_IWORK_e .
CircularBufSize , & _rtDW -> T1_IWORK_e . Tail , & _rtDW -> T1_IWORK_e . Head
, & _rtDW -> T1_IWORK_e . Last , simTime - _rtP -> P_104 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> T1_IWORK_e . MaxNewBufSize ) ) {
ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" ) ;
return ; } } ( * uBuffer + _rtDW -> T1_IWORK_e . CircularBufSize ) [ _rtDW ->
T1_IWORK_e . Head ] = simTime ; ( * uBuffer ) [ _rtDW -> T1_IWORK_e . Head ]
= _rtB -> B_30_61_488 ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit !=
0 ) { ssCallAccelRunBlock ( S , 30 , 201 , SS_CALL_MDL_UPDATE ) ; }
DC_AC_VSG_TrueRMS_Update ( S , & _rtB -> TrueRMS_c , & _rtDW -> TrueRMS_c , &
_rtP -> TrueRMS_c ) ; DC_AC_VSG_RMS_Update ( S , & _rtB -> RMS_h , & _rtDW ->
RMS_h , & _rtP -> RMS_h ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_f . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_e . Head = ( ( _rtDW -> TransportDelay_IWORK_e
. Head < ( _rtDW -> TransportDelay_IWORK_e . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_e . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_e . Head == _rtDW -> TransportDelay_IWORK_e . Tail ) {
if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_e . CircularBufSize , & _rtDW -> TransportDelay_IWORK_e
. Tail , & _rtDW -> TransportDelay_IWORK_e . Head , & _rtDW ->
TransportDelay_IWORK_e . Last , simTime - _rtP -> P_122 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_e . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_e .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_e . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_e . Head ] = _rtB -> B_30_78_624 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_k = _rtB -> B_30_81_648 ; } DC_AC_VSG_TrueRMS_Update ( S
, & _rtB -> TrueRMS_b , & _rtDW -> TrueRMS_b , & _rtP -> TrueRMS_b ) ;
DC_AC_VSG_RMS_Update ( S , & _rtB -> RMS_m , & _rtDW -> RMS_m , & _rtP ->
RMS_m ) ; { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_l . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
_rtDW -> TransportDelay_IWORK_i . Head = ( ( _rtDW -> TransportDelay_IWORK_i
. Head < ( _rtDW -> TransportDelay_IWORK_i . CircularBufSize - 1 ) ) ? (
_rtDW -> TransportDelay_IWORK_i . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_i . Head == _rtDW -> TransportDelay_IWORK_i . Tail ) {
if ( ! DC_AC_VSG_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_i . CircularBufSize , & _rtDW -> TransportDelay_IWORK_i
. Tail , & _rtDW -> TransportDelay_IWORK_i . Head , & _rtDW ->
TransportDelay_IWORK_i . Last , simTime - _rtP -> P_131 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_i . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_i .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_i . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_i . Head ] = _rtB -> B_30_83_664 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_p = _rtB -> B_30_86_688 ; } UNUSED_PARAMETER ( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID2 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_DC_AC_VSG_T * _rtB ;
DW_DC_AC_VSG_T * _rtDW ; P_DC_AC_VSG_T * _rtP ; XDot_DC_AC_VSG_T * _rtXdot ;
X_DC_AC_VSG_T * _rtX ; _rtDW = ( ( DW_DC_AC_VSG_T * ) ssGetRootDWork ( S ) )
; _rtXdot = ( ( XDot_DC_AC_VSG_T * ) ssGetdX ( S ) ) ; _rtX = ( (
X_DC_AC_VSG_T * ) ssGetContStates ( S ) ) ; _rtP = ( ( P_DC_AC_VSG_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_DC_AC_VSG_T * ) _ssGetModelBlockIO ( S )
) ; _rtXdot -> integrator_CSTATE = _rtB -> B_30_8_64 [ 0 ] ; _rtXdot ->
integ1_CSTATE = _rtB -> B_30_48_384 ; _rtXdot -> Integ2_CSTATE = _rtB ->
B_30_49_392 ; _rtXdot -> integ1_CSTATE_f = _rtB -> B_30_45_360 ; _rtXdot ->
Integ2_CSTATE_k = _rtB -> B_30_46_368 ; DC_AC_VSG_TrueRMS_Deriv ( S , & _rtB
-> TrueRMS , & _rtDW -> TrueRMS , & _rtXdot -> TrueRMS ) ;
DC_AC_VSG_RMS_Deriv ( S , & _rtB -> RMS , & _rtDW -> RMS , & _rtXdot -> RMS )
; _rtXdot -> integrator_CSTATE_j = _rtB -> B_30_23_184 ;
DC_AC_VSG_TrueRMS_Deriv ( S , & _rtB -> TrueRMS_o , & _rtDW -> TrueRMS_o , &
_rtXdot -> TrueRMS_o ) ; DC_AC_VSG_RMS_Deriv ( S , & _rtB -> RMS_p , & _rtDW
-> RMS_p , & _rtXdot -> RMS_p ) ; _rtXdot -> integrator_CSTATE_m = _rtB ->
B_30_8_64 [ 1 ] ; _rtXdot -> TransferFcn_CSTATE = 0.0 ; _rtXdot ->
TransferFcn_CSTATE += _rtP -> P_54 * _rtX -> TransferFcn_CSTATE ; _rtXdot ->
TransferFcn_CSTATE += _rtB -> B_30_50_400 ; DC_AC_VSG_PR_Deriv ( S , & _rtB
-> PR , & _rtP -> PR , & _rtX -> PR , & _rtXdot -> PR ) ; _rtXdot ->
Integrator1_CSTATE = _rtB -> B_30_52_416 ; _rtXdot -> TransferFcn_CSTATE_c =
0.0 ; _rtXdot -> TransferFcn_CSTATE_c += _rtP -> P_85 * _rtX ->
TransferFcn_CSTATE_c ; _rtXdot -> TransferFcn_CSTATE_c += _rtB -> B_30_53_424
; _rtXdot -> integrator_CSTATE_j0 = _rtB -> B_30_62_496 [ 0 ] ; _rtXdot ->
integ1_CSTATE_l = _rtB -> B_30_101_808 ; _rtXdot -> Integ2_CSTATE_b = _rtB ->
B_30_102_816 ; _rtXdot -> integ1_CSTATE_j = _rtB -> B_30_98_784 ; _rtXdot ->
Integ2_CSTATE_e = _rtB -> B_30_99_792 ; DC_AC_VSG_TrueRMS_Deriv ( S , & _rtB
-> TrueRMS_c , & _rtDW -> TrueRMS_c , & _rtXdot -> TrueRMS_c ) ;
DC_AC_VSG_RMS_Deriv ( S , & _rtB -> RMS_h , & _rtDW -> RMS_h , & _rtXdot ->
RMS_h ) ; _rtXdot -> integrator_CSTATE_l = _rtB -> B_30_77_616 ;
DC_AC_VSG_TrueRMS_Deriv ( S , & _rtB -> TrueRMS_b , & _rtDW -> TrueRMS_b , &
_rtXdot -> TrueRMS_b ) ; DC_AC_VSG_RMS_Deriv ( S , & _rtB -> RMS_m , & _rtDW
-> RMS_m , & _rtXdot -> RMS_m ) ; _rtXdot -> integrator_CSTATE_b = _rtB ->
B_30_62_496 [ 1 ] ; _rtXdot -> TransferFcn_CSTATE_a = 0.0 ; _rtXdot ->
TransferFcn_CSTATE_a += _rtP -> P_140 * _rtX -> TransferFcn_CSTATE_a ;
_rtXdot -> TransferFcn_CSTATE_a += _rtB -> B_30_103_824 ; DC_AC_VSG_PR_Deriv
( S , & _rtB -> PR_l , & _rtP -> PR_l , & _rtX -> PR_l , & _rtXdot -> PR_l )
; _rtXdot -> Integrator1_CSTATE_l = _rtB -> B_30_107_856 ; _rtXdot ->
Integrator2_CSTATE = _rtB -> B_30_105_840 ; _rtXdot -> uJsD_CSTATE = 0.0 ;
_rtXdot -> uJsD_CSTATE += _rtP -> P_171 * _rtX -> uJsD_CSTATE ; _rtXdot ->
uJsD_CSTATE += _rtB -> B_30_109_872 ; _rtXdot -> uJw0s_CSTATE = 0.0 ; _rtXdot
-> uJw0s_CSTATE += _rtP -> P_175 * _rtX -> uJw0s_CSTATE ; _rtXdot ->
uJw0s_CSTATE += _rtB -> B_30_108_864 ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 136192700U ) ; ssSetChecksumVal
( S , 1 , 837535544U ) ; ssSetChecksumVal ( S , 2 , 3871479543U ) ;
ssSetChecksumVal ( S , 3 , 2244743021U ) ; { mxArray * slVerStructMat = (
NULL ) ; mxArray * slStrMat = mxCreateString ( "simulink" ) ; char slVerChar
[ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat ,
"ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField (
slVerStructMat , 0 , "Version" ) ; if ( slVerMat == ( NULL ) ) { status = 1 ;
} else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "24.1" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != ( SLSize ) sizeof ( DW_DC_AC_VSG_T ) ) { static char msg [ 256 ] ;
snprintf ( msg , 256 , "Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofDWork ( S ) , ( unsigned long ) sizeof ( DW_DC_AC_VSG_T ) ) ;
ssSetErrorStatus ( S , msg ) ; } if ( ssGetSizeofGlobalBlockIO ( S ) != (
SLSize ) sizeof ( B_DC_AC_VSG_T ) ) { static char msg [ 256 ] ; snprintf (
msg , 256 , "Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofGlobalBlockIO ( S ) , ( unsigned long ) sizeof ( B_DC_AC_VSG_T ) )
; ssSetErrorStatus ( S , msg ) ; } { int ssSizeofParams ; ssGetSizeofParams (
S , & ssSizeofParams ) ; if ( ssSizeofParams != sizeof ( P_DC_AC_VSG_T ) ) {
static char msg [ 256 ] ; snprintf ( msg , 256 ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file (%d vs %lu)." , ssSizeofParams , (
unsigned long ) sizeof ( P_DC_AC_VSG_T ) ) ; ssSetErrorStatus ( S , msg ) ; }
} _ssSetModelRtp ( S , ( real_T * ) & DC_AC_VSG_rtDefaultP ) ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { { SimStruct * childS ;
SysOutputFcn * callSysFcns ; childS = ssGetSFunction ( S , 0 ) ; callSysFcns
= ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; childS = ssGetSFunction ( S , 1 ) ; callSysFcns =
ssGetCallSystemOutputFcnList ( childS ) ; callSysFcns [ 3 + 0 ] = (
SysOutputFcn ) ( NULL ) ; } slAccRegPrmChangeFcn ( S , mdlOutputsTID2 ) ; }
static void mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
