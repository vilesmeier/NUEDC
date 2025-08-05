#include "ThreePhase_Rectifier_acc.h"
#include <stddef.h>
#include <float.h>
#include "mwmathutil.h"
#include "rtwtypes.h"
#include "ThreePhase_Rectifier_acc_types.h"
#include "ThreePhase_Rectifier_acc_private.h"
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
boolean_T ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( int_T *
bufSzPtr , int_T * tailPtr , int_T * headPtr , int_T * lastPtr , real_T
tMinusDelay , real_T * * uBufPtr , boolean_T isfixedbuf , boolean_T
istransportdelay , int_T * maxNewBufSzPtr ) { int_T testIdx ; int_T tail = *
tailPtr ; int_T bufSz = * bufSzPtr ; real_T * tBuf = * uBufPtr + bufSz ;
real_T * xBuf = ( NULL ) ; int_T numBuffer = 2 ; if ( istransportdelay ) {
numBuffer = 3 ; xBuf = * uBufPtr + 2 * bufSz ; } testIdx = ( tail < ( bufSz -
1 ) ) ? ( tail + 1 ) : 0 ; if ( ( tMinusDelay <= tBuf [ testIdx ] ) && !
isfixedbuf ) { int_T j ; real_T * tempT ; real_T * tempU ; real_T * tempX = (
NULL ) ; real_T * uBuf = * uBufPtr ; int_T newBufSz = bufSz + 1024 ; if (
newBufSz > * maxNewBufSzPtr ) { * maxNewBufSzPtr = newBufSz ; } tempU = (
real_T * ) utMalloc ( numBuffer * newBufSz * sizeof ( real_T ) ) ; if ( tempU
== ( NULL ) ) { return ( false ) ; } tempT = tempU + newBufSz ; if (
istransportdelay ) tempX = tempT + newBufSz ; for ( j = tail ; j < bufSz ; j
++ ) { tempT [ j - tail ] = tBuf [ j ] ; tempU [ j - tail ] = uBuf [ j ] ; if
( istransportdelay ) tempX [ j - tail ] = xBuf [ j ] ; } for ( j = 0 ; j <
tail ; j ++ ) { tempT [ j + bufSz - tail ] = tBuf [ j ] ; tempU [ j + bufSz -
tail ] = uBuf [ j ] ; if ( istransportdelay ) tempX [ j + bufSz - tail ] =
xBuf [ j ] ; } if ( * lastPtr > tail ) { * lastPtr -= tail ; } else { *
lastPtr += ( bufSz - tail ) ; } * tailPtr = 0 ; * headPtr = bufSz ; utFree (
uBuf ) ; * bufSzPtr = newBufSz ; * uBufPtr = tempU ; } else { * tailPtr =
testIdx ; } return ( true ) ; } real_T
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( real_T tMinusDelay , real_T
tStart , real_T * uBuf , int_T bufSz , int_T * lastIdx , int_T oldestIdx ,
int_T newIdx , real_T initOutput , boolean_T discrete , boolean_T
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
ThreePhase_Rectifier_RMS_Init ( SimStruct * S , DW_RMS_ThreePhase_Rectifier_T
* localDW , P_RMS_ThreePhase_Rectifier_T * localP ,
X_RMS_ThreePhase_Rectifier_T * localX ) { localX -> integrator_CSTATE_o [ 0 ]
= localP -> P_2 ; localDW -> Memory_PreviousInput [ 0 ] = localP -> P_6 ;
localX -> integrator_CSTATE_fb [ 0 ] = localP -> P_7 ; localDW ->
Memory_PreviousInput_f [ 0 ] = localP -> P_11 ; localX -> integrator_CSTATE_o
[ 1 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [ 1 ] = localP -> P_6
; localX -> integrator_CSTATE_fb [ 1 ] = localP -> P_7 ; localDW ->
Memory_PreviousInput_f [ 1 ] = localP -> P_11 ; localX -> integrator_CSTATE_o
[ 2 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [ 2 ] = localP -> P_6
; localX -> integrator_CSTATE_fb [ 2 ] = localP -> P_7 ; localDW ->
Memory_PreviousInput_f [ 2 ] = localP -> P_11 ; } void
ThreePhase_Rectifier_RMS_Disable ( SimStruct * S ,
DW_RMS_ThreePhase_Rectifier_T * localDW ) { localDW -> RMS_MODE = false ; }
void ThreePhase_Rectifier_RMS ( SimStruct * S , boolean_T rtu_Enable , real_T
rtu_In , real_T rtu_In_h , real_T rtu_In_k , B_RMS_ThreePhase_Rectifier_T *
localB , DW_RMS_ThreePhase_Rectifier_T * localDW ,
P_RMS_ThreePhase_Rectifier_T * localP , X_RMS_ThreePhase_Rectifier_T * localX
, XDis_RMS_ThreePhase_Rectifier_T * localXdis ) { real_T B_6_576_4560 [ 3 ] ;
real_T B_6_580_4592 [ 3 ] ; real_T rtb_B_6_587_4648 ; int32_T isHit ;
boolean_T rtb_B_6_604_4784 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( (
isHit != 0 ) && ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if ( !
localDW -> RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_o ) , 0 , 6 * sizeof ( boolean_T ) ) ; localDW
-> RMS_MODE = true ; } } else { if ( ssGetTaskTime ( S , 1 ) == ssGetTStart (
S ) ) { ( void ) memset ( & ( localXdis -> integrator_CSTATE_o ) , 1 , 6 *
sizeof ( boolean_T ) ) ; } if ( localDW -> RMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_o ) , 1 , 6 * sizeof ( boolean_T ) ) ;
ThreePhase_Rectifier_RMS_Disable ( S , localDW ) ; } } } if ( localDW ->
RMS_MODE ) { localB -> B_6_276_2152 [ 0 ] = localX -> integrator_CSTATE_o [ 0
] ; localB -> B_6_276_2152 [ 1 ] = localX -> integrator_CSTATE_o [ 1 ] ;
localB -> B_6_276_2152 [ 2 ] = localX -> integrator_CSTATE_o [ 2 ] ; { static
const boolean_T isDiscrete [ ] = { false , false , false } ; real_T * *
uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; for ( isHit = 0 ;
isHit < 3 ; isHit ++ ) { tMinusDelay = ( ( localP -> P_3 > 0.0 ) ? localP ->
P_3 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; B_6_576_4560 [ isHit ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] , & localDW ->
TransportDelay_IWORK . Last [ isHit ] , localDW -> TransportDelay_IWORK .
Tail [ isHit ] , localDW -> TransportDelay_IWORK . Head [ isHit ] , localP ->
P_4 , isDiscrete [ isHit ] , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] ) [
localDW -> TransportDelay_IWORK . Head [ isHit ] ] == ssGetT ( S ) ) ) ) ;
uBuffer ++ ; } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
localB -> B_6_279_2176 = localP -> P_5 ; } rtb_B_6_604_4784 = ( ssGetT ( S )
>= localB -> B_6_279_2176 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { localB -> B_6_280_2184 [ 0 ] = localDW -> Memory_PreviousInput
[ 0 ] ; localB -> B_6_280_2184 [ 1 ] = localDW -> Memory_PreviousInput [ 1 ]
; localB -> B_6_280_2184 [ 2 ] = localDW -> Memory_PreviousInput [ 2 ] ; } if
( rtb_B_6_604_4784 ) { localB -> B_6_283_2208 [ 0 ] = ( localB ->
B_6_276_2152 [ 0 ] - B_6_576_4560 [ 0 ] ) * localP -> P_1 ; localB ->
B_6_283_2208 [ 1 ] = ( localB -> B_6_276_2152 [ 1 ] - B_6_576_4560 [ 1 ] ) *
localP -> P_1 ; localB -> B_6_283_2208 [ 2 ] = ( localB -> B_6_276_2152 [ 2 ]
- B_6_576_4560 [ 2 ] ) * localP -> P_1 ; } else { localB -> B_6_283_2208 [ 0
] = localB -> B_6_280_2184 [ 0 ] ; localB -> B_6_283_2208 [ 1 ] = localB ->
B_6_280_2184 [ 1 ] ; localB -> B_6_283_2208 [ 2 ] = localB -> B_6_280_2184 [
2 ] ; } localB -> B_6_286_2232 [ 0 ] = localX -> integrator_CSTATE_fb [ 0 ] ;
localB -> B_6_286_2232 [ 1 ] = localX -> integrator_CSTATE_fb [ 1 ] ; localB
-> B_6_286_2232 [ 2 ] = localX -> integrator_CSTATE_fb [ 2 ] ; { static const
boolean_T isDiscrete [ ] = { false , false , false } ; real_T * * uBuffer = (
real_T * * ) & localDW -> TransportDelay_PWORK_j . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; for ( isHit = 0 ; isHit
< 3 ; isHit ++ ) { tMinusDelay = ( ( localP -> P_8 > 0.0 ) ? localP -> P_8 :
0.0 ) ; tMinusDelay = simTime - tMinusDelay ; B_6_580_4592 [ isHit ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, localDW -> TransportDelay_IWORK_n . CircularBufSize [ isHit ] , & localDW
-> TransportDelay_IWORK_n . Last [ isHit ] , localDW ->
TransportDelay_IWORK_n . Tail [ isHit ] , localDW -> TransportDelay_IWORK_n .
Head [ isHit ] , localP -> P_9 , isDiscrete [ isHit ] , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW -> TransportDelay_IWORK_n
. CircularBufSize [ isHit ] ) [ localDW -> TransportDelay_IWORK_n . Head [
isHit ] ] == ssGetT ( S ) ) ) ) ; uBuffer ++ ; } } isHit = ssIsSampleHit ( S
, 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_6_289_2256 = localP -> P_10 ; }
rtb_B_6_604_4784 = ( ssGetT ( S ) >= localB -> B_6_289_2256 ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_6_290_2264 [ 0
] = localDW -> Memory_PreviousInput_f [ 0 ] ; localB -> B_6_290_2264 [ 1 ] =
localDW -> Memory_PreviousInput_f [ 1 ] ; localB -> B_6_290_2264 [ 2 ] =
localDW -> Memory_PreviousInput_f [ 2 ] ; } if ( rtb_B_6_604_4784 ) { localB
-> B_6_293_2288 [ 0 ] = ( localB -> B_6_286_2232 [ 0 ] - B_6_580_4592 [ 0 ] )
* localP -> P_0 ; localB -> B_6_293_2288 [ 1 ] = ( localB -> B_6_286_2232 [ 1
] - B_6_580_4592 [ 1 ] ) * localP -> P_0 ; localB -> B_6_293_2288 [ 2 ] = (
localB -> B_6_286_2232 [ 2 ] - B_6_580_4592 [ 2 ] ) * localP -> P_0 ; } else
{ localB -> B_6_293_2288 [ 0 ] = localB -> B_6_290_2264 [ 0 ] ; localB ->
B_6_293_2288 [ 1 ] = localB -> B_6_290_2264 [ 1 ] ; localB -> B_6_293_2288 [
2 ] = localB -> B_6_290_2264 [ 2 ] ; } rtb_B_6_587_4648 = muDoubleScalarSin (
localP -> P_14 * ssGetTaskTime ( S , 0 ) + localP -> P_15 ) * localP -> P_12
+ localP -> P_13 ; localB -> B_6_296_2312 [ 0 ] = rtu_In * rtb_B_6_587_4648 ;
localB -> B_6_296_2312 [ 1 ] = rtu_In_h * rtb_B_6_587_4648 ; localB ->
B_6_296_2312 [ 2 ] = rtu_In_k * rtb_B_6_587_4648 ; rtb_B_6_587_4648 =
muDoubleScalarSin ( localP -> P_18 * ssGetTaskTime ( S , 0 ) + localP -> P_19
) * localP -> P_16 + localP -> P_17 ; localB -> B_6_299_2336 [ 0 ] = rtu_In *
rtb_B_6_587_4648 ; localB -> B_6_299_2336 [ 1 ] = rtu_In_h * rtb_B_6_587_4648
; localB -> B_6_299_2336 [ 2 ] = rtu_In_k * rtb_B_6_587_4648 ; localB ->
B_6_302_2360 [ 0 ] = localP -> P_20 * muDoubleScalarHypot ( localB ->
B_6_283_2208 [ 0 ] , localB -> B_6_293_2288 [ 0 ] ) ; localB -> B_6_302_2360
[ 1 ] = localP -> P_20 * muDoubleScalarHypot ( localB -> B_6_283_2208 [ 1 ] ,
localB -> B_6_293_2288 [ 1 ] ) ; localB -> B_6_302_2360 [ 2 ] = localP ->
P_20 * muDoubleScalarHypot ( localB -> B_6_283_2208 [ 2 ] , localB ->
B_6_293_2288 [ 2 ] ) ; if ( ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC (
localDW -> RMS_SubsysRanBC ) ; } } } void ThreePhase_Rectifier_RMS_Update (
SimStruct * S , B_RMS_ThreePhase_Rectifier_T * localB ,
DW_RMS_ThreePhase_Rectifier_T * localDW , P_RMS_ThreePhase_Rectifier_T *
localP ) { int32_T isHit ; if ( localDW -> RMS_MODE ) { { real_T * * uBuffer
= ( real_T * * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head [ 0 ]
= ( ( localDW -> TransportDelay_IWORK . Head [ 0 ] < ( localDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] - 1 ) ) ? ( localDW ->
TransportDelay_IWORK . Head [ 0 ] + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK . Head [ 0 ] == localDW -> TransportDelay_IWORK . Tail [
0 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 0 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_6_276_2152 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 1 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_6_276_2152 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 2 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_6_276_2152 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_6_283_2208 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_6_283_2208 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_6_283_2208 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK_j . TUbufferPtrs [
0 ] ; real_T simTime = ssGetT ( S ) ; localDW -> TransportDelay_IWORK_n .
Head [ 0 ] = ( ( localDW -> TransportDelay_IWORK_n . Head [ 0 ] < ( localDW
-> TransportDelay_IWORK_n . CircularBufSize [ 0 ] - 1 ) ) ? ( localDW ->
TransportDelay_IWORK_n . Head [ 0 ] + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK_n . Head [ 0 ] == localDW -> TransportDelay_IWORK_n .
Tail [ 0 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK_n . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK_n . Tail [ 0 ] , & localDW -> TransportDelay_IWORK_n .
Head [ 0 ] , & localDW -> TransportDelay_IWORK_n . Last [ 0 ] , simTime -
localP -> P_8 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer +
localDW -> TransportDelay_IWORK_n . CircularBufSize [ 0 ] ) [ localDW ->
TransportDelay_IWORK_n . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK_n . Head [ 0 ] ] = localB -> B_6_286_2232 [ 0 ] ;
localDW -> TransportDelay_IWORK_n . Head [ 1 ] = ( ( localDW ->
TransportDelay_IWORK_n . Head [ 1 ] < ( localDW -> TransportDelay_IWORK_n .
CircularBufSize [ 1 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK_n . Head [
1 ] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK_n . Head [ 1 ] ==
localDW -> TransportDelay_IWORK_n . Tail [ 1 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_n . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK_n . Tail [ 1 ] , & localDW -> TransportDelay_IWORK_n .
Head [ 1 ] , & localDW -> TransportDelay_IWORK_n . Last [ 1 ] , simTime -
localP -> P_8 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer +
localDW -> TransportDelay_IWORK_n . CircularBufSize [ 1 ] ) [ localDW ->
TransportDelay_IWORK_n . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK_n . Head [ 1 ] ] = localB -> B_6_286_2232 [ 1 ] ;
localDW -> TransportDelay_IWORK_n . Head [ 2 ] = ( ( localDW ->
TransportDelay_IWORK_n . Head [ 2 ] < ( localDW -> TransportDelay_IWORK_n .
CircularBufSize [ 2 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK_n . Head [
2 ] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK_n . Head [ 2 ] ==
localDW -> TransportDelay_IWORK_n . Tail [ 2 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_n . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK_n . Tail [ 2 ] , & localDW -> TransportDelay_IWORK_n .
Head [ 2 ] , & localDW -> TransportDelay_IWORK_n . Last [ 2 ] , simTime -
localP -> P_8 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer +
localDW -> TransportDelay_IWORK_n . CircularBufSize [ 2 ] ) [ localDW ->
TransportDelay_IWORK_n . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK_n . Head [ 2 ] ] = localB -> B_6_286_2232 [ 2 ] ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput_f [ 0 ] = localB -> B_6_293_2288 [ 0 ] ; localDW ->
Memory_PreviousInput_f [ 1 ] = localB -> B_6_293_2288 [ 1 ] ; localDW ->
Memory_PreviousInput_f [ 2 ] = localB -> B_6_293_2288 [ 2 ] ; } } } void
ThreePhase_Rectifier_RMS_Deriv ( SimStruct * S , B_RMS_ThreePhase_Rectifier_T
* localB , DW_RMS_ThreePhase_Rectifier_T * localDW ,
XDot_RMS_ThreePhase_Rectifier_T * localXdot ) { if ( localDW -> RMS_MODE ) {
localXdot -> integrator_CSTATE_o [ 0 ] = localB -> B_6_296_2312 [ 0 ] ;
localXdot -> integrator_CSTATE_fb [ 0 ] = localB -> B_6_299_2336 [ 0 ] ;
localXdot -> integrator_CSTATE_o [ 1 ] = localB -> B_6_296_2312 [ 1 ] ;
localXdot -> integrator_CSTATE_fb [ 1 ] = localB -> B_6_299_2336 [ 1 ] ;
localXdot -> integrator_CSTATE_o [ 2 ] = localB -> B_6_296_2312 [ 2 ] ;
localXdot -> integrator_CSTATE_fb [ 2 ] = localB -> B_6_299_2336 [ 2 ] ; }
else { { real_T * dx ; int_T i1 ; dx = & ( localXdot -> integrator_CSTATE_o [
0 ] ) ; for ( i1 = 0 ; i1 < 6 ; i1 ++ ) { dx [ i1 ] = 0.0 ; } } } } void
ThreePhase_Rectifier_RMS_Term ( SimStruct * const S ) { UNUSED_PARAMETER ( S
) ; } void ThreePhase_Rectifier_TrueRMS_Init ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_T * localP , X_TrueRMS_ThreePhase_Rectifier_T
* localX ) { localX -> integrator_CSTATE_f [ 0 ] = localP -> P_1 ; localDW ->
Memory_PreviousInput [ 0 ] = localP -> P_5 ; localX -> integrator_CSTATE_f [
1 ] = localP -> P_1 ; localDW -> Memory_PreviousInput [ 1 ] = localP -> P_5 ;
localX -> integrator_CSTATE_f [ 2 ] = localP -> P_1 ; localDW ->
Memory_PreviousInput [ 2 ] = localP -> P_5 ; } void
ThreePhase_Rectifier_TrueRMS_Disable ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_T * localDW ) { localDW -> TrueRMS_MODE =
false ; } void ThreePhase_Rectifier_TrueRMS ( SimStruct * S , boolean_T
rtu_Enable , real_T rtu_In , real_T rtu_In_h , real_T rtu_In_c ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , P_TrueRMS_ThreePhase_Rectifier_T * localP ,
X_TrueRMS_ThreePhase_Rectifier_T * localX ,
XDis_TrueRMS_ThreePhase_Rectifier_T * localXdis ) { real_T B_8_560_4408 [ 3 ]
; real_T B_8_569_4480_idx_0 ; real_T B_8_569_4480_idx_1 ; real_T
B_8_569_4480_idx_2 ; int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if
( ( isHit != 0 ) && ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if (
! localDW -> TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S
) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset (
& ( localXdis -> integrator_CSTATE_f ) , 0 , 3 * sizeof ( boolean_T ) ) ;
localDW -> TrueRMS_MODE = true ; } } else { if ( ssGetTaskTime ( S , 1 ) ==
ssGetTStart ( S ) ) { ( void ) memset ( & ( localXdis -> integrator_CSTATE_f
) , 1 , 3 * sizeof ( boolean_T ) ) ; } if ( localDW -> TrueRMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_f ) , 1 , 3 * sizeof ( boolean_T ) ) ;
ThreePhase_Rectifier_TrueRMS_Disable ( S , localDW ) ; } } } if ( localDW ->
TrueRMS_MODE ) { localB -> B_8_260_2024 [ 0 ] = localX -> integrator_CSTATE_f
[ 0 ] ; localB -> B_8_260_2024 [ 1 ] = localX -> integrator_CSTATE_f [ 1 ] ;
localB -> B_8_260_2024 [ 2 ] = localX -> integrator_CSTATE_f [ 2 ] ; { static
const boolean_T isDiscrete [ ] = { false , false , false } ; real_T * *
uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0
] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; for ( isHit = 0 ;
isHit < 3 ; isHit ++ ) { tMinusDelay = ( ( localP -> P_2 > 0.0 ) ? localP ->
P_2 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; B_8_560_4408 [ isHit ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] , & localDW ->
TransportDelay_IWORK . Last [ isHit ] , localDW -> TransportDelay_IWORK .
Tail [ isHit ] , localDW -> TransportDelay_IWORK . Head [ isHit ] , localP ->
P_3 , isDiscrete [ isHit ] , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] ) [
localDW -> TransportDelay_IWORK . Head [ isHit ] ] == ssGetT ( S ) ) ) ) ;
uBuffer ++ ; } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
localB -> B_8_263_2048 = localP -> P_4 ; localB -> B_8_264_2056 [ 0 ] =
localDW -> Memory_PreviousInput [ 0 ] ; localB -> B_8_264_2056 [ 1 ] =
localDW -> Memory_PreviousInput [ 1 ] ; localB -> B_8_264_2056 [ 2 ] =
localDW -> Memory_PreviousInput [ 2 ] ; } if ( ssGetT ( S ) >= localB ->
B_8_263_2048 ) { localB -> B_8_267_2080 [ 0 ] = ( localB -> B_8_260_2024 [ 0
] - B_8_560_4408 [ 0 ] ) * localP -> P_0 ; localB -> B_8_267_2080 [ 1 ] = (
localB -> B_8_260_2024 [ 1 ] - B_8_560_4408 [ 1 ] ) * localP -> P_0 ; localB
-> B_8_267_2080 [ 2 ] = ( localB -> B_8_260_2024 [ 2 ] - B_8_560_4408 [ 2 ] )
* localP -> P_0 ; } else { localB -> B_8_267_2080 [ 0 ] = localB ->
B_8_264_2056 [ 0 ] ; localB -> B_8_267_2080 [ 1 ] = localB -> B_8_264_2056 [
1 ] ; localB -> B_8_267_2080 [ 2 ] = localB -> B_8_264_2056 [ 2 ] ; } localB
-> B_8_270_2104 [ 0 ] = rtu_In * rtu_In ; localB -> B_8_270_2104 [ 1 ] =
rtu_In_h * rtu_In_h ; localB -> B_8_270_2104 [ 2 ] = rtu_In_c * rtu_In_c ; if
( ssIsModeUpdateTimeStep ( S ) ) { if ( localB -> B_8_267_2080 [ 0 ] >=
localP -> P_6 ) { localDW -> Saturationtoavoidnegativesqrt_MODE [ 0 ] = 1 ; }
else if ( localB -> B_8_267_2080 [ 0 ] > localP -> P_7 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] = 0 ; } else { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] = - 1 ; } if ( localB ->
B_8_267_2080 [ 1 ] >= localP -> P_6 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] = 1 ; } else if ( localB ->
B_8_267_2080 [ 1 ] > localP -> P_7 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] = 0 ; } else { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] = - 1 ; } if ( localB ->
B_8_267_2080 [ 2 ] >= localP -> P_6 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] = 1 ; } else if ( localB ->
B_8_267_2080 [ 2 ] > localP -> P_7 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] = 0 ; } else { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] = - 1 ; } } if ( localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] == 1 ) { B_8_569_4480_idx_0 = localP
-> P_6 ; } else if ( localDW -> Saturationtoavoidnegativesqrt_MODE [ 0 ] == -
1 ) { B_8_569_4480_idx_0 = localP -> P_7 ; } else { B_8_569_4480_idx_0 =
localB -> B_8_267_2080 [ 0 ] ; } if ( localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] == 1 ) { B_8_569_4480_idx_1 = localP
-> P_6 ; } else if ( localDW -> Saturationtoavoidnegativesqrt_MODE [ 1 ] == -
1 ) { B_8_569_4480_idx_1 = localP -> P_7 ; } else { B_8_569_4480_idx_1 =
localB -> B_8_267_2080 [ 1 ] ; } if ( localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] == 1 ) { B_8_569_4480_idx_2 = localP
-> P_6 ; } else if ( localDW -> Saturationtoavoidnegativesqrt_MODE [ 2 ] == -
1 ) { B_8_569_4480_idx_2 = localP -> P_7 ; } else { B_8_569_4480_idx_2 =
localB -> B_8_267_2080 [ 2 ] ; } if ( ssIsMajorTimeStep ( S ) != 0 ) { if (
localDW -> Sqrt_DWORK1 != 0 ) { ssSetBlockStateForSolverChangedAtMajorStep (
S ) ; ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; localDW ->
Sqrt_DWORK1 = 0 ; } localB -> B_8_273_2128 [ 0 ] = muDoubleScalarSqrt (
B_8_569_4480_idx_0 ) ; localB -> B_8_273_2128 [ 1 ] = muDoubleScalarSqrt (
B_8_569_4480_idx_1 ) ; localB -> B_8_273_2128 [ 2 ] = muDoubleScalarSqrt (
B_8_569_4480_idx_2 ) ; } else { if ( B_8_569_4480_idx_0 < 0.0 ) { localB ->
B_8_273_2128 [ 0 ] = - muDoubleScalarSqrt ( muDoubleScalarAbs (
B_8_569_4480_idx_0 ) ) ; localDW -> Sqrt_DWORK1 = 1 ; } else { localB ->
B_8_273_2128 [ 0 ] = muDoubleScalarSqrt ( B_8_569_4480_idx_0 ) ; } if (
B_8_569_4480_idx_1 < 0.0 ) { localB -> B_8_273_2128 [ 1 ] = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( B_8_569_4480_idx_1 ) ) ; localDW ->
Sqrt_DWORK1 = 1 ; } else { localB -> B_8_273_2128 [ 1 ] = muDoubleScalarSqrt
( B_8_569_4480_idx_1 ) ; } if ( B_8_569_4480_idx_2 < 0.0 ) { localB ->
B_8_273_2128 [ 2 ] = - muDoubleScalarSqrt ( muDoubleScalarAbs (
B_8_569_4480_idx_2 ) ) ; localDW -> Sqrt_DWORK1 = 1 ; } else { localB ->
B_8_273_2128 [ 2 ] = muDoubleScalarSqrt ( B_8_569_4480_idx_2 ) ; } } if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW -> TrueRMS_SubsysRanBC
) ; } } } void ThreePhase_Rectifier_TrueRMS_Update ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , P_TrueRMS_ThreePhase_Rectifier_T * localP ) { int32_T isHit ; if
( localDW -> TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW
-> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S )
; localDW -> TransportDelay_IWORK . Head [ 0 ] = ( ( localDW ->
TransportDelay_IWORK . Head [ 0 ] < ( localDW -> TransportDelay_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head [ 0
] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK . Head [ 0 ] == localDW
-> TransportDelay_IWORK . Tail [ 0 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_2 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 0 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_8_260_2024 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_2 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 1 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_8_260_2024 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_2 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 2 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_8_260_2024 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_8_267_2080 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_8_267_2080 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_8_267_2080 [ 2 ] ; } } } void
ThreePhase_Rectifier_TrueRMS_Deriv ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , XDot_TrueRMS_ThreePhase_Rectifier_T * localXdot ) { if ( localDW
-> TrueRMS_MODE ) { localXdot -> integrator_CSTATE_f [ 0 ] = localB ->
B_8_270_2104 [ 0 ] ; localXdot -> integrator_CSTATE_f [ 1 ] = localB ->
B_8_270_2104 [ 1 ] ; localXdot -> integrator_CSTATE_f [ 2 ] = localB ->
B_8_270_2104 [ 2 ] ; } else { { real_T * dx ; int_T i1 ; dx = & ( localXdot
-> integrator_CSTATE_f [ 0 ] ) ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { dx [ i1 ]
= 0.0 ; } } } } void ThreePhase_Rectifier_TrueRMS_ZC ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_T * localB , DW_TrueRMS_ThreePhase_Rectifier_T
* localDW , P_TrueRMS_ThreePhase_Rectifier_T * localP ,
ZCV_TrueRMS_ThreePhase_Rectifier_T * localZCSV ) { if ( localDW ->
TrueRMS_MODE ) { localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC_i [ 0 ]
= localB -> B_8_267_2080 [ 0 ] - localP -> P_6 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC_m [ 0 ] = localB -> B_8_267_2080 [ 0
] - localP -> P_7 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC_i [
1 ] = localB -> B_8_267_2080 [ 1 ] - localP -> P_6 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC_m [ 1 ] = localB -> B_8_267_2080 [ 1
] - localP -> P_7 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC_i [
2 ] = localB -> B_8_267_2080 [ 2 ] - localP -> P_6 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC_m [ 2 ] = localB -> B_8_267_2080 [ 2
] - localP -> P_7 ; } else { { real_T * zcsv = & ( localZCSV ->
Saturationtoavoidnegativesqrt_UprLim_ZC_i [ 0 ] ) ; int_T i ; for ( i = 0 ; i
< 6 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } } void
ThreePhase_Rectifier_TrueRMS_Term ( SimStruct * const S ) { UNUSED_PARAMETER
( S ) ; } void ThreePhase_Rectifier_RMS_p_Init ( SimStruct * S ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW , P_RMS_ThreePhase_Rectifier_f_T *
localP , X_RMS_ThreePhase_Rectifier_j_T * localX ) { localX ->
integrator_CSTATE_h [ 0 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [
0 ] = localP -> P_6 ; localX -> integrator_CSTATE_i [ 0 ] = localP -> P_7 ;
localDW -> Memory_PreviousInput_p [ 0 ] = localP -> P_11 ; localX ->
integrator_CSTATE_h [ 1 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [
1 ] = localP -> P_6 ; localX -> integrator_CSTATE_i [ 1 ] = localP -> P_7 ;
localDW -> Memory_PreviousInput_p [ 1 ] = localP -> P_11 ; localX ->
integrator_CSTATE_h [ 2 ] = localP -> P_2 ; localDW -> Memory_PreviousInput [
2 ] = localP -> P_6 ; localX -> integrator_CSTATE_i [ 2 ] = localP -> P_7 ;
localDW -> Memory_PreviousInput_p [ 2 ] = localP -> P_11 ; } void
ThreePhase_Rectifier_RMS_l_Disable ( SimStruct * S ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW ) { localDW -> RMS_MODE = false ; }
void ThreePhase_Rectifier_RMS_k ( SimStruct * S , boolean_T rtu_Enable ,
const real_T rtu_In [ 3 ] , B_RMS_ThreePhase_Rectifier_n_T * localB ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW , P_RMS_ThreePhase_Rectifier_f_T *
localP , X_RMS_ThreePhase_Rectifier_j_T * localX ,
XDis_RMS_ThreePhase_Rectifier_g_T * localXdis ) { real_T B_17_482_3776 [ 3 ]
; real_T B_17_486_3808 [ 3 ] ; real_T rtb_B_17_493_3864 ; int32_T isHit ;
boolean_T rtb_B_17_510_4000 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( (
isHit != 0 ) && ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) { if ( !
localDW -> RMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) != ssGetTStart ( S ) ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } ( void ) memset ( & (
localXdis -> integrator_CSTATE_h ) , 0 , 6 * sizeof ( boolean_T ) ) ; localDW
-> RMS_MODE = true ; } } else { if ( ssGetTaskTime ( S , 1 ) == ssGetTStart (
S ) ) { ( void ) memset ( & ( localXdis -> integrator_CSTATE_h ) , 1 , 6 *
sizeof ( boolean_T ) ) ; } if ( localDW -> RMS_MODE ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; ( void ) memset ( & (
localXdis -> integrator_CSTATE_h ) , 1 , 6 * sizeof ( boolean_T ) ) ;
ThreePhase_Rectifier_RMS_l_Disable ( S , localDW ) ; } } } if ( localDW ->
RMS_MODE ) { localB -> B_17_186_1432 [ 0 ] = localX -> integrator_CSTATE_h [
0 ] ; localB -> B_17_186_1432 [ 1 ] = localX -> integrator_CSTATE_h [ 1 ] ;
localB -> B_17_186_1432 [ 2 ] = localX -> integrator_CSTATE_h [ 2 ] ; {
static const boolean_T isDiscrete [ ] = { false , false , false } ; real_T *
* uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [
0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; for ( isHit = 0 ;
isHit < 3 ; isHit ++ ) { tMinusDelay = ( ( localP -> P_3 > 0.0 ) ? localP ->
P_3 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ; B_17_482_3776 [ isHit ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] , & localDW ->
TransportDelay_IWORK . Last [ isHit ] , localDW -> TransportDelay_IWORK .
Tail [ isHit ] , localDW -> TransportDelay_IWORK . Head [ isHit ] , localP ->
P_4 , isDiscrete [ isHit ] , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] ) [
localDW -> TransportDelay_IWORK . Head [ isHit ] ] == ssGetT ( S ) ) ) ) ;
uBuffer ++ ; } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
localB -> B_17_189_1456 = localP -> P_5 ; } rtb_B_17_510_4000 = ( ssGetT ( S
) >= localB -> B_17_189_1456 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { localB -> B_17_190_1464 [ 0 ] = localDW ->
Memory_PreviousInput [ 0 ] ; localB -> B_17_190_1464 [ 1 ] = localDW ->
Memory_PreviousInput [ 1 ] ; localB -> B_17_190_1464 [ 2 ] = localDW ->
Memory_PreviousInput [ 2 ] ; } if ( rtb_B_17_510_4000 ) { localB ->
B_17_193_1488 [ 0 ] = ( localB -> B_17_186_1432 [ 0 ] - B_17_482_3776 [ 0 ] )
* localP -> P_1 ; localB -> B_17_193_1488 [ 1 ] = ( localB -> B_17_186_1432 [
1 ] - B_17_482_3776 [ 1 ] ) * localP -> P_1 ; localB -> B_17_193_1488 [ 2 ] =
( localB -> B_17_186_1432 [ 2 ] - B_17_482_3776 [ 2 ] ) * localP -> P_1 ; }
else { localB -> B_17_193_1488 [ 0 ] = localB -> B_17_190_1464 [ 0 ] ; localB
-> B_17_193_1488 [ 1 ] = localB -> B_17_190_1464 [ 1 ] ; localB ->
B_17_193_1488 [ 2 ] = localB -> B_17_190_1464 [ 2 ] ; } localB ->
B_17_196_1512 [ 0 ] = localX -> integrator_CSTATE_i [ 0 ] ; localB ->
B_17_196_1512 [ 1 ] = localX -> integrator_CSTATE_i [ 1 ] ; localB ->
B_17_196_1512 [ 2 ] = localX -> integrator_CSTATE_i [ 2 ] ; { static const
boolean_T isDiscrete [ ] = { false , false , false } ; real_T * * uBuffer = (
real_T * * ) & localDW -> TransportDelay_PWORK_g . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; real_T tMinusDelay ; for ( isHit = 0 ; isHit
< 3 ; isHit ++ ) { tMinusDelay = ( ( localP -> P_8 > 0.0 ) ? localP -> P_8 :
0.0 ) ; tMinusDelay = simTime - tMinusDelay ; B_17_486_3808 [ isHit ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, localDW -> TransportDelay_IWORK_a . CircularBufSize [ isHit ] , & localDW
-> TransportDelay_IWORK_a . Last [ isHit ] , localDW ->
TransportDelay_IWORK_a . Tail [ isHit ] , localDW -> TransportDelay_IWORK_a .
Head [ isHit ] , localP -> P_9 , isDiscrete [ isHit ] , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + localDW -> TransportDelay_IWORK_a
. CircularBufSize [ isHit ] ) [ localDW -> TransportDelay_IWORK_a . Head [
isHit ] ] == ssGetT ( S ) ) ) ) ; uBuffer ++ ; } } isHit = ssIsSampleHit ( S
, 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_17_199_1536 = localP -> P_10 ; }
rtb_B_17_510_4000 = ( ssGetT ( S ) >= localB -> B_17_199_1536 ) ; isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localB -> B_17_200_1544 [ 0
] = localDW -> Memory_PreviousInput_p [ 0 ] ; localB -> B_17_200_1544 [ 1 ] =
localDW -> Memory_PreviousInput_p [ 1 ] ; localB -> B_17_200_1544 [ 2 ] =
localDW -> Memory_PreviousInput_p [ 2 ] ; } if ( rtb_B_17_510_4000 ) { localB
-> B_17_203_1568 [ 0 ] = ( localB -> B_17_196_1512 [ 0 ] - B_17_486_3808 [ 0
] ) * localP -> P_0 ; localB -> B_17_203_1568 [ 1 ] = ( localB ->
B_17_196_1512 [ 1 ] - B_17_486_3808 [ 1 ] ) * localP -> P_0 ; localB ->
B_17_203_1568 [ 2 ] = ( localB -> B_17_196_1512 [ 2 ] - B_17_486_3808 [ 2 ] )
* localP -> P_0 ; } else { localB -> B_17_203_1568 [ 0 ] = localB ->
B_17_200_1544 [ 0 ] ; localB -> B_17_203_1568 [ 1 ] = localB -> B_17_200_1544
[ 1 ] ; localB -> B_17_203_1568 [ 2 ] = localB -> B_17_200_1544 [ 2 ] ; }
rtb_B_17_493_3864 = muDoubleScalarSin ( localP -> P_14 * ssGetTaskTime ( S ,
0 ) + localP -> P_15 ) * localP -> P_12 + localP -> P_13 ; localB ->
B_17_206_1592 [ 0 ] = rtu_In [ 0 ] * rtb_B_17_493_3864 ; localB ->
B_17_206_1592 [ 1 ] = rtu_In [ 1 ] * rtb_B_17_493_3864 ; localB ->
B_17_206_1592 [ 2 ] = rtu_In [ 2 ] * rtb_B_17_493_3864 ; rtb_B_17_493_3864 =
muDoubleScalarSin ( localP -> P_18 * ssGetTaskTime ( S , 0 ) + localP -> P_19
) * localP -> P_16 + localP -> P_17 ; localB -> B_17_209_1616 [ 0 ] = rtu_In
[ 0 ] * rtb_B_17_493_3864 ; localB -> B_17_209_1616 [ 1 ] = rtu_In [ 1 ] *
rtb_B_17_493_3864 ; localB -> B_17_209_1616 [ 2 ] = rtu_In [ 2 ] *
rtb_B_17_493_3864 ; localB -> B_17_212_1640 [ 0 ] = localP -> P_20 *
muDoubleScalarHypot ( localB -> B_17_193_1488 [ 0 ] , localB -> B_17_203_1568
[ 0 ] ) ; localB -> B_17_212_1640 [ 1 ] = localP -> P_20 *
muDoubleScalarHypot ( localB -> B_17_193_1488 [ 1 ] , localB -> B_17_203_1568
[ 1 ] ) ; localB -> B_17_212_1640 [ 2 ] = localP -> P_20 *
muDoubleScalarHypot ( localB -> B_17_193_1488 [ 2 ] , localB -> B_17_203_1568
[ 2 ] ) ; if ( ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW ->
RMS_SubsysRanBC ) ; } } } void ThreePhase_Rectifier_RMS_j_Update ( SimStruct
* S , B_RMS_ThreePhase_Rectifier_n_T * localB ,
DW_RMS_ThreePhase_Rectifier_i_T * localDW , P_RMS_ThreePhase_Rectifier_f_T *
localP ) { int32_T isHit ; if ( localDW -> RMS_MODE ) { { real_T * * uBuffer
= ( real_T * * ) & localDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; localDW -> TransportDelay_IWORK . Head [ 0 ]
= ( ( localDW -> TransportDelay_IWORK . Head [ 0 ] < ( localDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] - 1 ) ) ? ( localDW ->
TransportDelay_IWORK . Head [ 0 ] + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK . Head [ 0 ] == localDW -> TransportDelay_IWORK . Tail [
0 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( &
localDW -> TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 0 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_17_186_1432 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 1 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_17_186_1432 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_3 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 2 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_17_186_1432 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_17_193_1488 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_17_193_1488 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_17_193_1488 [ 2 ] ; } { real_T * *
uBuffer = ( real_T * * ) & localDW -> TransportDelay_PWORK_g . TUbufferPtrs [
0 ] ; real_T simTime = ssGetT ( S ) ; localDW -> TransportDelay_IWORK_a .
Head [ 0 ] = ( ( localDW -> TransportDelay_IWORK_a . Head [ 0 ] < ( localDW
-> TransportDelay_IWORK_a . CircularBufSize [ 0 ] - 1 ) ) ? ( localDW ->
TransportDelay_IWORK_a . Head [ 0 ] + 1 ) : 0 ) ; if ( localDW ->
TransportDelay_IWORK_a . Head [ 0 ] == localDW -> TransportDelay_IWORK_a .
Tail [ 0 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK_a . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK_a . Tail [ 0 ] , & localDW -> TransportDelay_IWORK_a .
Head [ 0 ] , & localDW -> TransportDelay_IWORK_a . Last [ 0 ] , simTime -
localP -> P_8 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_a . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer +
localDW -> TransportDelay_IWORK_a . CircularBufSize [ 0 ] ) [ localDW ->
TransportDelay_IWORK_a . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK_a . Head [ 0 ] ] = localB -> B_17_196_1512 [ 0 ] ;
localDW -> TransportDelay_IWORK_a . Head [ 1 ] = ( ( localDW ->
TransportDelay_IWORK_a . Head [ 1 ] < ( localDW -> TransportDelay_IWORK_a .
CircularBufSize [ 1 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK_a . Head [
1 ] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK_a . Head [ 1 ] ==
localDW -> TransportDelay_IWORK_a . Tail [ 1 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_a . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK_a . Tail [ 1 ] , & localDW -> TransportDelay_IWORK_a .
Head [ 1 ] , & localDW -> TransportDelay_IWORK_a . Last [ 1 ] , simTime -
localP -> P_8 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_a . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer +
localDW -> TransportDelay_IWORK_a . CircularBufSize [ 1 ] ) [ localDW ->
TransportDelay_IWORK_a . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW
-> TransportDelay_IWORK_a . Head [ 1 ] ] = localB -> B_17_196_1512 [ 1 ] ;
localDW -> TransportDelay_IWORK_a . Head [ 2 ] = ( ( localDW ->
TransportDelay_IWORK_a . Head [ 2 ] < ( localDW -> TransportDelay_IWORK_a .
CircularBufSize [ 2 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK_a . Head [
2 ] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK_a . Head [ 2 ] ==
localDW -> TransportDelay_IWORK_a . Tail [ 2 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK_a . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK_a . Tail [ 2 ] , & localDW -> TransportDelay_IWORK_a .
Head [ 2 ] , & localDW -> TransportDelay_IWORK_a . Last [ 2 ] , simTime -
localP -> P_8 , uBuffer , ( boolean_T ) 0 , false , & localDW ->
TransportDelay_IWORK_a . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer +
localDW -> TransportDelay_IWORK_a . CircularBufSize [ 2 ] ) [ localDW ->
TransportDelay_IWORK_a . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK_a . Head [ 2 ] ] = localB -> B_17_196_1512 [ 2 ] ; }
isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput_p [ 0 ] = localB -> B_17_203_1568 [ 0 ] ; localDW ->
Memory_PreviousInput_p [ 1 ] = localB -> B_17_203_1568 [ 1 ] ; localDW ->
Memory_PreviousInput_p [ 2 ] = localB -> B_17_203_1568 [ 2 ] ; } } } void
ThreePhase_Rectifier_RMS_h_Deriv ( SimStruct * S ,
B_RMS_ThreePhase_Rectifier_n_T * localB , DW_RMS_ThreePhase_Rectifier_i_T *
localDW , XDot_RMS_ThreePhase_Rectifier_n_T * localXdot ) { if ( localDW ->
RMS_MODE ) { localXdot -> integrator_CSTATE_h [ 0 ] = localB -> B_17_206_1592
[ 0 ] ; localXdot -> integrator_CSTATE_i [ 0 ] = localB -> B_17_209_1616 [ 0
] ; localXdot -> integrator_CSTATE_h [ 1 ] = localB -> B_17_206_1592 [ 1 ] ;
localXdot -> integrator_CSTATE_i [ 1 ] = localB -> B_17_209_1616 [ 1 ] ;
localXdot -> integrator_CSTATE_h [ 2 ] = localB -> B_17_206_1592 [ 2 ] ;
localXdot -> integrator_CSTATE_i [ 2 ] = localB -> B_17_209_1616 [ 2 ] ; }
else { { real_T * dx ; int_T i1 ; dx = & ( localXdot -> integrator_CSTATE_h [
0 ] ) ; for ( i1 = 0 ; i1 < 6 ; i1 ++ ) { dx [ i1 ] = 0.0 ; } } } } void
ThreePhase_Rectifier_RMS_i_Term ( SimStruct * const S ) { UNUSED_PARAMETER (
S ) ; } void ThreePhase_Rectifier_TrueRMS_p_Init ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ,
X_TrueRMS_ThreePhase_Rectifier_f_T * localX ) { localX -> integrator_CSTATE_k
[ 0 ] = localP -> P_1 ; localDW -> Memory_PreviousInput [ 0 ] = localP -> P_5
; localX -> integrator_CSTATE_k [ 1 ] = localP -> P_1 ; localDW ->
Memory_PreviousInput [ 1 ] = localP -> P_5 ; localX -> integrator_CSTATE_k [
2 ] = localP -> P_1 ; localDW -> Memory_PreviousInput [ 2 ] = localP -> P_5 ;
} void ThreePhase_Rectifier_TrueRMS_e_Disable ( SimStruct * S ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ) { localDW -> TrueRMS_MODE =
false ; } void ThreePhase_Rectifier_TrueRMS_e ( SimStruct * S , boolean_T
rtu_Enable , const real_T rtu_In [ 3 ] , B_TrueRMS_ThreePhase_Rectifier_e_T *
localB , DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ,
X_TrueRMS_ThreePhase_Rectifier_f_T * localX ,
XDis_TrueRMS_ThreePhase_Rectifier_p_T * localXdis ) { real_T B_19_466_3624 [
3 ] ; real_T B_19_475_3696_idx_0 ; real_T B_19_475_3696_idx_1 ; real_T
B_19_475_3696_idx_2 ; int32_T isHit ; isHit = ssIsSampleHit ( S , 1 , 0 ) ;
if ( ( isHit != 0 ) && ssIsModeUpdateTimeStep ( S ) ) { if ( rtu_Enable ) {
if ( ! localDW -> TrueRMS_MODE ) { if ( ssGetTaskTime ( S , 1 ) !=
ssGetTStart ( S ) ) { ssSetBlockStateForSolverChangedAtMajorStep ( S ) ; } (
void ) memset ( & ( localXdis -> integrator_CSTATE_k ) , 0 , 3 * sizeof (
boolean_T ) ) ; localDW -> TrueRMS_MODE = true ; } } else { if (
ssGetTaskTime ( S , 1 ) == ssGetTStart ( S ) ) { ( void ) memset ( & (
localXdis -> integrator_CSTATE_k ) , 1 , 3 * sizeof ( boolean_T ) ) ; } if (
localDW -> TrueRMS_MODE ) { ssSetBlockStateForSolverChangedAtMajorStep ( S )
; ( void ) memset ( & ( localXdis -> integrator_CSTATE_k ) , 1 , 3 * sizeof (
boolean_T ) ) ; ThreePhase_Rectifier_TrueRMS_e_Disable ( S , localDW ) ; } }
} if ( localDW -> TrueRMS_MODE ) { localB -> B_19_170_1304 [ 0 ] = localX ->
integrator_CSTATE_k [ 0 ] ; localB -> B_19_170_1304 [ 1 ] = localX ->
integrator_CSTATE_k [ 1 ] ; localB -> B_19_170_1304 [ 2 ] = localX ->
integrator_CSTATE_k [ 2 ] ; { static const boolean_T isDiscrete [ ] = { false
, false , false } ; real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay ; for ( isHit = 0 ; isHit < 3 ; isHit ++ ) { tMinusDelay =
( ( localP -> P_2 > 0.0 ) ? localP -> P_2 : 0.0 ) ; tMinusDelay = simTime -
tMinusDelay ; B_19_466_3624 [ isHit ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] , & localDW ->
TransportDelay_IWORK . Last [ isHit ] , localDW -> TransportDelay_IWORK .
Tail [ isHit ] , localDW -> TransportDelay_IWORK . Head [ isHit ] , localP ->
P_3 , isDiscrete [ isHit ] , ( boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( *
uBuffer + localDW -> TransportDelay_IWORK . CircularBufSize [ isHit ] ) [
localDW -> TransportDelay_IWORK . Head [ isHit ] ] == ssGetT ( S ) ) ) ) ;
uBuffer ++ ; } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
localB -> B_19_173_1328 = localP -> P_4 ; localB -> B_19_174_1336 [ 0 ] =
localDW -> Memory_PreviousInput [ 0 ] ; localB -> B_19_174_1336 [ 1 ] =
localDW -> Memory_PreviousInput [ 1 ] ; localB -> B_19_174_1336 [ 2 ] =
localDW -> Memory_PreviousInput [ 2 ] ; } if ( ssGetT ( S ) >= localB ->
B_19_173_1328 ) { localB -> B_19_177_1360 [ 0 ] = ( localB -> B_19_170_1304 [
0 ] - B_19_466_3624 [ 0 ] ) * localP -> P_0 ; localB -> B_19_177_1360 [ 1 ] =
( localB -> B_19_170_1304 [ 1 ] - B_19_466_3624 [ 1 ] ) * localP -> P_0 ;
localB -> B_19_177_1360 [ 2 ] = ( localB -> B_19_170_1304 [ 2 ] -
B_19_466_3624 [ 2 ] ) * localP -> P_0 ; } else { localB -> B_19_177_1360 [ 0
] = localB -> B_19_174_1336 [ 0 ] ; localB -> B_19_177_1360 [ 1 ] = localB ->
B_19_174_1336 [ 1 ] ; localB -> B_19_177_1360 [ 2 ] = localB -> B_19_174_1336
[ 2 ] ; } localB -> B_19_180_1384 [ 0 ] = rtu_In [ 0 ] * rtu_In [ 0 ] ;
localB -> B_19_180_1384 [ 1 ] = rtu_In [ 1 ] * rtu_In [ 1 ] ; localB ->
B_19_180_1384 [ 2 ] = rtu_In [ 2 ] * rtu_In [ 2 ] ; if (
ssIsModeUpdateTimeStep ( S ) ) { if ( localB -> B_19_177_1360 [ 0 ] >= localP
-> P_6 ) { localDW -> Saturationtoavoidnegativesqrt_MODE [ 0 ] = 1 ; } else
if ( localB -> B_19_177_1360 [ 0 ] > localP -> P_7 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] = 0 ; } else { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] = - 1 ; } if ( localB ->
B_19_177_1360 [ 1 ] >= localP -> P_6 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] = 1 ; } else if ( localB ->
B_19_177_1360 [ 1 ] > localP -> P_7 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] = 0 ; } else { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] = - 1 ; } if ( localB ->
B_19_177_1360 [ 2 ] >= localP -> P_6 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] = 1 ; } else if ( localB ->
B_19_177_1360 [ 2 ] > localP -> P_7 ) { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] = 0 ; } else { localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] = - 1 ; } } if ( localDW ->
Saturationtoavoidnegativesqrt_MODE [ 0 ] == 1 ) { B_19_475_3696_idx_0 =
localP -> P_6 ; } else if ( localDW -> Saturationtoavoidnegativesqrt_MODE [ 0
] == - 1 ) { B_19_475_3696_idx_0 = localP -> P_7 ; } else {
B_19_475_3696_idx_0 = localB -> B_19_177_1360 [ 0 ] ; } if ( localDW ->
Saturationtoavoidnegativesqrt_MODE [ 1 ] == 1 ) { B_19_475_3696_idx_1 =
localP -> P_6 ; } else if ( localDW -> Saturationtoavoidnegativesqrt_MODE [ 1
] == - 1 ) { B_19_475_3696_idx_1 = localP -> P_7 ; } else {
B_19_475_3696_idx_1 = localB -> B_19_177_1360 [ 1 ] ; } if ( localDW ->
Saturationtoavoidnegativesqrt_MODE [ 2 ] == 1 ) { B_19_475_3696_idx_2 =
localP -> P_6 ; } else if ( localDW -> Saturationtoavoidnegativesqrt_MODE [ 2
] == - 1 ) { B_19_475_3696_idx_2 = localP -> P_7 ; } else {
B_19_475_3696_idx_2 = localB -> B_19_177_1360 [ 2 ] ; } if (
ssIsMajorTimeStep ( S ) != 0 ) { if ( localDW -> Sqrt_DWORK1 != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; localDW ->
Sqrt_DWORK1 = 0 ; } localB -> B_19_183_1408 [ 0 ] = muDoubleScalarSqrt (
B_19_475_3696_idx_0 ) ; localB -> B_19_183_1408 [ 1 ] = muDoubleScalarSqrt (
B_19_475_3696_idx_1 ) ; localB -> B_19_183_1408 [ 2 ] = muDoubleScalarSqrt (
B_19_475_3696_idx_2 ) ; } else { if ( B_19_475_3696_idx_0 < 0.0 ) { localB ->
B_19_183_1408 [ 0 ] = - muDoubleScalarSqrt ( muDoubleScalarAbs (
B_19_475_3696_idx_0 ) ) ; localDW -> Sqrt_DWORK1 = 1 ; } else { localB ->
B_19_183_1408 [ 0 ] = muDoubleScalarSqrt ( B_19_475_3696_idx_0 ) ; } if (
B_19_475_3696_idx_1 < 0.0 ) { localB -> B_19_183_1408 [ 1 ] = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( B_19_475_3696_idx_1 ) ) ; localDW ->
Sqrt_DWORK1 = 1 ; } else { localB -> B_19_183_1408 [ 1 ] = muDoubleScalarSqrt
( B_19_475_3696_idx_1 ) ; } if ( B_19_475_3696_idx_2 < 0.0 ) { localB ->
B_19_183_1408 [ 2 ] = - muDoubleScalarSqrt ( muDoubleScalarAbs (
B_19_475_3696_idx_2 ) ) ; localDW -> Sqrt_DWORK1 = 1 ; } else { localB ->
B_19_183_1408 [ 2 ] = muDoubleScalarSqrt ( B_19_475_3696_idx_2 ) ; } } if (
ssIsModeUpdateTimeStep ( S ) ) { srUpdateBC ( localDW -> TrueRMS_SubsysRanBC
) ; } } } void ThreePhase_Rectifier_TrueRMS_c_Update ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ) { int32_T isHit ; if ( localDW
-> TrueRMS_MODE ) { { real_T * * uBuffer = ( real_T * * ) & localDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
localDW -> TransportDelay_IWORK . Head [ 0 ] = ( ( localDW ->
TransportDelay_IWORK . Head [ 0 ] < ( localDW -> TransportDelay_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( localDW -> TransportDelay_IWORK . Head [ 0
] + 1 ) : 0 ) ; if ( localDW -> TransportDelay_IWORK . Head [ 0 ] == localDW
-> TransportDelay_IWORK . Tail [ 0 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & localDW ->
TransportDelay_IWORK . CircularBufSize [ 0 ] , & localDW ->
TransportDelay_IWORK . Tail [ 0 ] , & localDW -> TransportDelay_IWORK . Head
[ 0 ] , & localDW -> TransportDelay_IWORK . Last [ 0 ] , simTime - localP ->
P_2 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 0 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 0 ] ] = localB -> B_19_170_1304 [ 0 ] ; localDW
-> TransportDelay_IWORK . Head [ 1 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 1 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 1 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 1 ] == localDW -> TransportDelay_IWORK .
Tail [ 1 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 1 ] , & localDW ->
TransportDelay_IWORK . Tail [ 1 ] , & localDW -> TransportDelay_IWORK . Head
[ 1 ] , & localDW -> TransportDelay_IWORK . Last [ 1 ] , simTime - localP ->
P_2 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 1 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ localDW ->
TransportDelay_IWORK . Head [ 1 ] ] = localB -> B_19_170_1304 [ 1 ] ; localDW
-> TransportDelay_IWORK . Head [ 2 ] = ( ( localDW -> TransportDelay_IWORK .
Head [ 2 ] < ( localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] - 1 )
) ? ( localDW -> TransportDelay_IWORK . Head [ 2 ] + 1 ) : 0 ) ; if ( localDW
-> TransportDelay_IWORK . Head [ 2 ] == localDW -> TransportDelay_IWORK .
Tail [ 2 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& localDW -> TransportDelay_IWORK . CircularBufSize [ 2 ] , & localDW ->
TransportDelay_IWORK . Tail [ 2 ] , & localDW -> TransportDelay_IWORK . Head
[ 2 ] , & localDW -> TransportDelay_IWORK . Last [ 2 ] , simTime - localP ->
P_2 , uBuffer , ( boolean_T ) 0 , false , & localDW -> TransportDelay_IWORK .
MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T * )
"\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + localDW
-> TransportDelay_IWORK . CircularBufSize [ 2 ] ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ localDW ->
TransportDelay_IWORK . Head [ 2 ] ] = localB -> B_19_170_1304 [ 2 ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { localDW ->
Memory_PreviousInput [ 0 ] = localB -> B_19_177_1360 [ 0 ] ; localDW ->
Memory_PreviousInput [ 1 ] = localB -> B_19_177_1360 [ 1 ] ; localDW ->
Memory_PreviousInput [ 2 ] = localB -> B_19_177_1360 [ 2 ] ; } } } void
ThreePhase_Rectifier_TrueRMS_e_Deriv ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
XDot_TrueRMS_ThreePhase_Rectifier_n_T * localXdot ) { if ( localDW ->
TrueRMS_MODE ) { localXdot -> integrator_CSTATE_k [ 0 ] = localB ->
B_19_180_1384 [ 0 ] ; localXdot -> integrator_CSTATE_k [ 1 ] = localB ->
B_19_180_1384 [ 1 ] ; localXdot -> integrator_CSTATE_k [ 2 ] = localB ->
B_19_180_1384 [ 2 ] ; } else { { real_T * dx ; int_T i1 ; dx = & ( localXdot
-> integrator_CSTATE_k [ 0 ] ) ; for ( i1 = 0 ; i1 < 3 ; i1 ++ ) { dx [ i1 ]
= 0.0 ; } } } } void ThreePhase_Rectifier_TrueRMS_a_ZC ( SimStruct * S ,
B_TrueRMS_ThreePhase_Rectifier_e_T * localB ,
DW_TrueRMS_ThreePhase_Rectifier_j_T * localDW ,
P_TrueRMS_ThreePhase_Rectifier_d_T * localP ,
ZCV_TrueRMS_ThreePhase_Rectifier_k_T * localZCSV ) { if ( localDW ->
TrueRMS_MODE ) { localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 0 ] =
localB -> B_19_177_1360 [ 0 ] - localP -> P_6 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 0 ] = localB -> B_19_177_1360 [ 0 ]
- localP -> P_7 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 1 ]
= localB -> B_19_177_1360 [ 1 ] - localP -> P_6 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 1 ] = localB -> B_19_177_1360 [ 1 ]
- localP -> P_7 ; localZCSV -> Saturationtoavoidnegativesqrt_UprLim_ZC [ 2 ]
= localB -> B_19_177_1360 [ 2 ] - localP -> P_6 ; localZCSV ->
Saturationtoavoidnegativesqrt_LwrLim_ZC [ 2 ] = localB -> B_19_177_1360 [ 2 ]
- localP -> P_7 ; } else { { real_T * zcsv = & ( localZCSV ->
Saturationtoavoidnegativesqrt_UprLim_ZC [ 0 ] ) ; int_T i ; for ( i = 0 ; i <
6 ; i ++ ) { zcsv [ i ] = 0.0 ; } } } } void
ThreePhase_Rectifier_TrueRMS_o_Term ( SimStruct * const S ) {
UNUSED_PARAMETER ( S ) ; } static void mdlOutputs ( SimStruct * S , int_T tid
) { real_T B_25_305_2384 [ 6 ] ; real_T B_25_315_2464 [ 2 ] ; real_T
B_25_327_2560 ; real_T B_25_332_2600 [ 3 ] ; real_T B_25_339_2656 [ 3 ] ;
B_ThreePhase_Rectifier_T * _rtB ; DW_ThreePhase_Rectifier_T * _rtDW ;
P_ThreePhase_Rectifier_T * _rtP ; XDis_ThreePhase_Rectifier_T * _rtXdis ;
X_ThreePhase_Rectifier_T * _rtX ; real_T B_25_336_2632_idx_0 ; real_T
B_25_336_2632_idx_1 ; real_T B_25_336_2632_idx_2 ; real_T B_25_343_2688_idx_0
; real_T B_25_343_2688_idx_1 ; real_T B_25_399_3136_0 ; real_T
B_25_399_3136_idx_2 ; real_T B_25_72_576_0 ; real_T rtb_B_1_385_3024 ; real_T
rtb_B_25_312_2440 ; real_T rtb_B_25_314_2456 ; int32_T i ; int32_T isHit ;
boolean_T rtb_B_25_408_3208 ; _rtDW = ( ( DW_ThreePhase_Rectifier_T * )
ssGetRootDWork ( S ) ) ; _rtXdis = ( ( XDis_ThreePhase_Rectifier_T * )
ssGetContStateDisabled ( S ) ) ; _rtX = ( ( X_ThreePhase_Rectifier_T * )
ssGetContStates ( S ) ) ; _rtP = ( ( P_ThreePhase_Rectifier_T * )
ssGetModelRtp ( S ) ) ; _rtB = ( ( B_ThreePhase_Rectifier_T * )
_ssGetModelBlockIO ( S ) ) ; _rtB -> B_25_0_0 = muDoubleScalarSin ( _rtP ->
P_16 * ssGetTaskTime ( S , 0 ) + _rtP -> P_17 ) * _rtP -> P_14 + _rtP -> P_15
; _rtB -> B_25_1_8 = muDoubleScalarSin ( _rtP -> P_20 * ssGetTaskTime ( S , 0
) + _rtP -> P_21 ) * _rtP -> P_18 + _rtP -> P_19 ; _rtB -> B_25_2_16 =
muDoubleScalarSin ( _rtP -> P_24 * ssGetTaskTime ( S , 0 ) + _rtP -> P_25 ) *
_rtP -> P_22 + _rtP -> P_23 ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if (
isHit != 0 ) { ssCallAccelRunBlock ( S , 25 , 3 , SS_CALL_MDL_OUTPUTS ) ;
_rtB -> B_25_20_160 = _rtP -> P_36 * _rtB -> B_25_3_24 [ 8 ] ;
ssCallAccelRunBlock ( S , 25 , 5 , SS_CALL_MDL_OUTPUTS ) ; } for ( i = 0 ; i
< 6 ; i ++ ) { _rtB -> B_25_21_168 [ i ] = _rtX -> integrator_CSTATE [ i ] ;
} { static const boolean_T isDiscrete [ ] = { false , false , false , false ,
false , false } ; real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay ; for ( i = 0 ; i < 6 ; i ++ ) { tMinusDelay = ( ( _rtP ->
P_38 > 0.0 ) ? _rtP -> P_38 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ;
B_25_305_2384 [ i ] = ThreePhase_Rectifier_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK .
CircularBufSize [ i ] , & _rtDW -> TransportDelay_IWORK . Last [ i ] , _rtDW
-> TransportDelay_IWORK . Tail [ i ] , _rtDW -> TransportDelay_IWORK . Head [
i ] , _rtP -> P_39 , isDiscrete [ i ] , ( boolean_T ) ( ssIsMinorTimeStep ( S
) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK . CircularBufSize [ i ] )
[ _rtDW -> TransportDelay_IWORK . Head [ i ] ] == ssGetT ( S ) ) ) ) ;
uBuffer ++ ; } } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_25_27_216 = _rtP -> P_40 ; } rtb_B_25_408_3208 = ( ssGetT ( S ) >=
_rtB -> B_25_27_216 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; for ( i = 0 ; i
< 6 ; i ++ ) { if ( isHit != 0 ) { _rtB -> B_25_28_224 [ i ] = _rtDW ->
Memory_PreviousInput [ i ] ; } if ( rtb_B_25_408_3208 ) { _rtB -> B_25_34_272
[ i ] = ( _rtB -> B_25_21_168 [ i ] - B_25_305_2384 [ i ] ) * _rtP -> P_0 ; }
else { _rtB -> B_25_34_272 [ i ] = _rtB -> B_25_28_224 [ i ] ; } }
ssCallAccelRunBlock ( S , 25 , 14 , SS_CALL_MDL_OUTPUTS ) ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { rtb_B_25_312_2440 = _rtB ->
B_25_110_880 - _rtB -> B_25_20_160 ; _rtB -> B_25_40_320 = _rtP -> P_42 *
rtb_B_25_312_2440 ; } rtb_B_25_314_2456 = _rtB -> B_25_40_320 + _rtX ->
Integrator_CSTATE ; { static const boolean_T isDiscrete [ ] = { true , true }
; real_T * * uBuffer = ( real_T * * ) & _rtDW -> VariableTimeDelay1_PWORK .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T appliedDelay ; {
int_T i1 ; real_T * y0 = & B_25_315_2464 [ 0 ] ; int_T * iw_Tail = & _rtDW ->
VariableTimeDelay1_IWORK . Tail [ 0 ] ; int_T * iw_Head = & _rtDW ->
VariableTimeDelay1_IWORK . Head [ 0 ] ; int_T * iw_Last = & _rtDW ->
VariableTimeDelay1_IWORK . Last [ 0 ] ; int_T * iw_CircularBufSize = & _rtDW
-> VariableTimeDelay1_IWORK . CircularBufSize [ 0 ] ; for ( i1 = 0 ; i1 < 2 ;
i1 ++ ) { appliedDelay = _rtB -> B_25_107_856 ; if ( appliedDelay > _rtP ->
P_44 ) { appliedDelay = _rtP -> P_44 ; } if ( appliedDelay < 0.0 ) {
appliedDelay = 0.0 ; } y0 [ i1 ] =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( simTime - appliedDelay , 0.0
, * uBuffer , iw_CircularBufSize [ i1 ] , & iw_Last [ i1 ] , iw_Tail [ i1 ] ,
iw_Head [ i1 ] , _rtP -> P_45 , isDiscrete [ i1 ] , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + iw_CircularBufSize [ i1 ] ) [
iw_Head [ i1 ] ] == ssGetT ( S ) ) ) ) ; uBuffer ++ ; } } } isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_25_41_328 = _rtP
-> P_47 * _rtB -> B_25_3_24 [ 9 ] ; _rtB -> B_25_42_336 = _rtP -> P_48 * _rtB
-> B_25_3_24 [ 10 ] ; } _rtB -> B_25_43_344 [ 0 ] = _rtP -> P_46 *
B_25_315_2464 [ 0 ] * rtb_B_25_314_2456 - _rtB -> B_25_41_328 ; _rtB ->
B_25_43_344 [ 1 ] = _rtP -> P_46 * B_25_315_2464 [ 1 ] * rtb_B_25_314_2456 -
_rtB -> B_25_42_336 ; _rtB -> B_1_111_888 = _rtP -> P_3 * _rtB -> B_25_43_344
[ 0 ] ; _rtB -> B_1_112_896 = _rtP -> P_4 * _rtB -> B_25_43_344 [ 1 ] ; _rtB
-> B_1_113_904 = ( _rtP -> P_6 [ 0 ] * _rtX -> TransferFcn_CSTATE [ 0 ] +
_rtP -> P_6 [ 1 ] * _rtX -> TransferFcn_CSTATE [ 1 ] ) + _rtP -> P_1 * _rtB
-> B_25_43_344 [ 0 ] ; _rtB -> B_1_114_912 = ( _rtP -> P_8 [ 0 ] * _rtX ->
TransferFcn1_CSTATE [ 0 ] + _rtP -> P_8 [ 1 ] * _rtX -> TransferFcn1_CSTATE [
1 ] ) + _rtP -> P_2 * _rtB -> B_25_43_344 [ 1 ] ; if ( ssIsModeUpdateTimeStep
( S ) ) { if ( _rtB -> B_1_113_904 >= _rtP -> P_9 ) { _rtDW ->
Saturation_MODE [ 0 ] = 1 ; } else if ( _rtB -> B_1_113_904 > _rtP -> P_10 )
{ _rtDW -> Saturation_MODE [ 0 ] = 0 ; } else { _rtDW -> Saturation_MODE [ 0
] = - 1 ; } if ( _rtB -> B_1_114_912 >= _rtP -> P_9 ) { _rtDW ->
Saturation_MODE [ 1 ] = 1 ; } else if ( _rtB -> B_1_114_912 > _rtP -> P_10 )
{ _rtDW -> Saturation_MODE [ 1 ] = 0 ; } else { _rtDW -> Saturation_MODE [ 1
] = - 1 ; } } if ( _rtDW -> Saturation_MODE [ 0 ] == 1 ) { _rtB ->
B_1_115_920 [ 0 ] = _rtP -> P_9 ; } else if ( _rtDW -> Saturation_MODE [ 0 ]
== - 1 ) { _rtB -> B_1_115_920 [ 0 ] = _rtP -> P_10 ; } else { _rtB ->
B_1_115_920 [ 0 ] = _rtB -> B_1_113_904 ; } if ( _rtDW -> Saturation_MODE [ 1
] == 1 ) { _rtB -> B_1_115_920 [ 1 ] = _rtP -> P_9 ; } else if ( _rtDW ->
Saturation_MODE [ 1 ] == - 1 ) { _rtB -> B_1_115_920 [ 1 ] = _rtP -> P_10 ; }
else { _rtB -> B_1_115_920 [ 1 ] = _rtB -> B_1_114_912 ; }
ssCallAccelRunBlock ( S , 25 , 26 , SS_CALL_MDL_OUTPUTS ) ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { rtb_B_25_314_2456 = _rtP ->
P_49 * _rtB -> B_25_3_24 [ 6 ] ; rtb_B_1_385_3024 = _rtP -> P_51 * _rtB ->
B_25_3_24 [ 7 ] ; _rtB -> B_25_45_360 [ 0 ] = ( _rtP -> P_50 *
rtb_B_25_314_2456 + rtb_B_1_385_3024 ) * _rtP -> P_53 ; _rtB -> B_25_45_360 [
1 ] = ( ( 0.0 - rtb_B_25_314_2456 ) - _rtP -> P_52 * rtb_B_1_385_3024 ) *
_rtP -> P_53 ; _rtB -> B_25_47_376 = ( 0.0 - _rtB -> B_25_45_360 [ 0 ] ) -
_rtB -> B_25_45_360 [ 1 ] ; _rtB -> B_25_48_384 = ( 0.0 - _rtB -> B_25_41_328
) - _rtB -> B_25_42_336 ; ssCallAccelRunBlock ( S , 25 , 36 ,
SS_CALL_MDL_OUTPUTS ) ; } _rtB -> B_25_49_392 = _rtX -> integrator_CSTATE_j ;
{ real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_n .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; real_T tMinusDelay =
simTime - _rtP -> P_55 ; B_25_327_2560 =
ThreePhase_Rectifier_acc_rt_TDelayInterpolate ( tMinusDelay , 0.0 , * uBuffer
, _rtDW -> TransportDelay_IWORK_f . CircularBufSize , & _rtDW ->
TransportDelay_IWORK_f . Last , _rtDW -> TransportDelay_IWORK_f . Tail ,
_rtDW -> TransportDelay_IWORK_f . Head , _rtP -> P_56 , 0 , ( boolean_T ) (
ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] == ssGetT ( S )
) ) ) ; } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_25_50_400 = _rtP -> P_57 ; } rtb_B_25_408_3208 = ( ssGetT ( S ) >= _rtB ->
B_25_50_400 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_25_51_408 = _rtDW -> Memory_PreviousInput_g ; } if (
rtb_B_25_408_3208 ) { _rtB -> B_25_52_416 = ( _rtB -> B_25_49_392 -
B_25_327_2560 ) * _rtP -> P_11 ; } else { _rtB -> B_25_52_416 = _rtB ->
B_25_51_408 ; } ssCallAccelRunBlock ( S , 25 , 51 , SS_CALL_MDL_OUTPUTS ) ;
ThreePhase_Rectifier_TrueRMS ( S , _rtB -> B_25_119_938 , _rtB -> B_25_45_360
[ 0 ] , _rtB -> B_25_47_376 , _rtB -> B_25_45_360 [ 1 ] , & _rtB -> TrueRMS_f
, & _rtDW -> TrueRMS_f , & _rtP -> TrueRMS_f , & _rtX -> TrueRMS_f , &
_rtXdis -> TrueRMS_f ) ; ThreePhase_Rectifier_RMS ( S , _rtB -> B_25_120_939
, _rtB -> B_25_45_360 [ 0 ] , _rtB -> B_25_47_376 , _rtB -> B_25_45_360 [ 1 ]
, & _rtB -> RMS_g , & _rtDW -> RMS_g , & _rtP -> RMS_g , & _rtX -> RMS_g , &
_rtXdis -> RMS_g ) ; if ( _rtB -> B_25_119_938 ) { rtb_B_25_314_2456 = _rtB
-> TrueRMS_f . B_8_273_2128 [ 0 ] ; rtb_B_1_385_3024 = _rtB -> TrueRMS_f .
B_8_273_2128 [ 1 ] ; B_25_399_3136_idx_2 = _rtB -> TrueRMS_f . B_8_273_2128 [
2 ] ; } else { rtb_B_25_314_2456 = _rtB -> RMS_g . B_6_302_2360 [ 0 ] ;
rtb_B_1_385_3024 = _rtB -> RMS_g . B_6_302_2360 [ 1 ] ; B_25_399_3136_idx_2 =
_rtB -> RMS_g . B_6_302_2360 [ 2 ] ; } ThreePhase_Rectifier_TrueRMS ( S ,
_rtB -> B_25_117_936 , _rtB -> B_25_41_328 , _rtB -> B_25_48_384 , _rtB ->
B_25_42_336 , & _rtB -> TrueRMS , & _rtDW -> TrueRMS , & _rtP -> TrueRMS , &
_rtX -> TrueRMS , & _rtXdis -> TrueRMS ) ; ThreePhase_Rectifier_RMS ( S ,
_rtB -> B_25_118_937 , _rtB -> B_25_41_328 , _rtB -> B_25_48_384 , _rtB ->
B_25_42_336 , & _rtB -> RMS , & _rtDW -> RMS , & _rtP -> RMS , & _rtX -> RMS
, & _rtXdis -> RMS ) ; if ( _rtB -> B_25_117_936 ) { B_25_336_2632_idx_0 =
_rtB -> TrueRMS . B_8_273_2128 [ 0 ] ; } else { B_25_336_2632_idx_0 = _rtB ->
RMS . B_6_302_2360 [ 0 ] ; } rtb_B_25_314_2456 *= B_25_336_2632_idx_0 ; if (
_rtB -> B_25_117_936 ) { B_25_336_2632_idx_0 = _rtB -> TrueRMS . B_8_273_2128
[ 1 ] ; } else { B_25_336_2632_idx_0 = _rtB -> RMS . B_6_302_2360 [ 1 ] ; }
rtb_B_1_385_3024 *= B_25_336_2632_idx_0 ; if ( _rtB -> B_25_117_936 ) {
B_25_336_2632_idx_0 = _rtB -> TrueRMS . B_8_273_2128 [ 2 ] ; } else {
B_25_336_2632_idx_0 = _rtB -> RMS . B_6_302_2360 [ 2 ] ; }
B_25_399_3136_idx_2 *= B_25_336_2632_idx_0 ; _rtB -> B_25_53_424 = (
rtb_B_25_314_2456 + rtb_B_1_385_3024 ) + B_25_399_3136_idx_2 ; _rtB ->
B_25_54_432 = _rtB -> B_25_52_416 / _rtB -> B_25_53_424 ; ssCallAccelRunBlock
( S , 25 , 61 , SS_CALL_MDL_OUTPUTS ) ; ssCallAccelRunBlock ( S , 25 , 62 ,
SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_25_55_440 [ 0 ] = _rtX ->
integrator_CSTATE_p [ 0 ] ; _rtB -> B_25_55_440 [ 1 ] = _rtX ->
integrator_CSTATE_p [ 1 ] ; _rtB -> B_25_55_440 [ 2 ] = _rtX ->
integrator_CSTATE_p [ 2 ] ; { static const boolean_T isDiscrete [ ] = { false
, false , false } ; real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_i . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay ; for ( i = 0 ; i < 3 ; i ++ ) { tMinusDelay = ( ( _rtP ->
P_60 > 0.0 ) ? _rtP -> P_60 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ;
B_25_332_2600 [ i ] = ThreePhase_Rectifier_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_n .
CircularBufSize [ i ] , & _rtDW -> TransportDelay_IWORK_n . Last [ i ] ,
_rtDW -> TransportDelay_IWORK_n . Tail [ i ] , _rtDW ->
TransportDelay_IWORK_n . Head [ i ] , _rtP -> P_61 , isDiscrete [ i ] , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW ->
TransportDelay_IWORK_n . CircularBufSize [ i ] ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ i ] ] == ssGetT ( S ) ) ) ) ; uBuffer ++ ; }
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_25_58_464 = _rtP -> P_62 ; } rtb_B_25_408_3208 = ( ssGetT ( S ) >= _rtB ->
B_25_58_464 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_25_59_472 [ 0 ] = _rtDW -> Memory_PreviousInput_o [ 0 ] ; _rtB ->
B_25_59_472 [ 1 ] = _rtDW -> Memory_PreviousInput_o [ 1 ] ; _rtB ->
B_25_59_472 [ 2 ] = _rtDW -> Memory_PreviousInput_o [ 2 ] ; } if (
rtb_B_25_408_3208 ) { _rtB -> B_25_62_496 [ 0 ] = ( _rtB -> B_25_55_440 [ 0 ]
- B_25_332_2600 [ 0 ] ) * _rtP -> P_12 ; _rtB -> B_25_62_496 [ 1 ] = ( _rtB
-> B_25_55_440 [ 1 ] - B_25_332_2600 [ 1 ] ) * _rtP -> P_12 ; _rtB ->
B_25_62_496 [ 2 ] = ( _rtB -> B_25_55_440 [ 2 ] - B_25_332_2600 [ 2 ] ) *
_rtP -> P_12 ; } else { _rtB -> B_25_62_496 [ 0 ] = _rtB -> B_25_59_472 [ 0 ]
; _rtB -> B_25_62_496 [ 1 ] = _rtB -> B_25_59_472 [ 1 ] ; _rtB -> B_25_62_496
[ 2 ] = _rtB -> B_25_59_472 [ 2 ] ; } ThreePhase_Rectifier_TrueRMS_e ( S ,
_rtB -> B_25_121_940 , _rtB -> B_25_62_496 , & _rtB -> TrueRMS_e , & _rtDW ->
TrueRMS_e , & _rtP -> TrueRMS_e , & _rtX -> TrueRMS_e , & _rtXdis ->
TrueRMS_e ) ; ThreePhase_Rectifier_RMS_k ( S , _rtB -> B_25_122_941 , _rtB ->
B_25_62_496 , & _rtB -> RMS_k , & _rtDW -> RMS_k , & _rtP -> RMS_k , & _rtX
-> RMS_k , & _rtXdis -> RMS_k ) ; if ( _rtB -> B_25_121_940 ) {
rtb_B_25_314_2456 = _rtB -> TrueRMS_e . B_19_183_1408 [ 0 ] ;
rtb_B_1_385_3024 = _rtB -> TrueRMS_e . B_19_183_1408 [ 1 ] ;
B_25_399_3136_idx_2 = _rtB -> TrueRMS_e . B_19_183_1408 [ 2 ] ; } else {
rtb_B_25_314_2456 = _rtB -> RMS_k . B_17_212_1640 [ 0 ] ; rtb_B_1_385_3024 =
_rtB -> RMS_k . B_17_212_1640 [ 1 ] ; B_25_399_3136_idx_2 = _rtB -> RMS_k .
B_17_212_1640 [ 2 ] ; } B_25_336_2632_idx_0 = rtb_B_25_314_2456 *
rtb_B_25_314_2456 ; _rtB -> B_25_65_520 [ 0 ] = _rtX -> integrator_CSTATE_l [
0 ] ; B_25_336_2632_idx_1 = rtb_B_1_385_3024 * rtb_B_1_385_3024 ; _rtB ->
B_25_65_520 [ 1 ] = _rtX -> integrator_CSTATE_l [ 1 ] ; B_25_336_2632_idx_2 =
B_25_399_3136_idx_2 * B_25_399_3136_idx_2 ; _rtB -> B_25_65_520 [ 2 ] = _rtX
-> integrator_CSTATE_l [ 2 ] ; { static const boolean_T isDiscrete [ ] = {
false , false , false } ; real_T * * uBuffer = ( real_T * * ) & _rtDW ->
TransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ;
real_T tMinusDelay ; for ( i = 0 ; i < 3 ; i ++ ) { tMinusDelay = ( ( _rtP ->
P_65 > 0.0 ) ? _rtP -> P_65 : 0.0 ) ; tMinusDelay = simTime - tMinusDelay ;
B_25_339_2656 [ i ] = ThreePhase_Rectifier_acc_rt_TDelayInterpolate (
tMinusDelay , 0.0 , * uBuffer , _rtDW -> TransportDelay_IWORK_h .
CircularBufSize [ i ] , & _rtDW -> TransportDelay_IWORK_h . Last [ i ] ,
_rtDW -> TransportDelay_IWORK_h . Tail [ i ] , _rtDW ->
TransportDelay_IWORK_h . Head [ i ] , _rtP -> P_66 , isDiscrete [ i ] , (
boolean_T ) ( ssIsMinorTimeStep ( S ) && ( ( * uBuffer + _rtDW ->
TransportDelay_IWORK_h . CircularBufSize [ i ] ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ i ] ] == ssGetT ( S ) ) ) ) ; uBuffer ++ ; }
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtB ->
B_25_68_544 = _rtP -> P_67 ; } rtb_B_25_408_3208 = ( ssGetT ( S ) >= _rtB ->
B_25_68_544 ) ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) {
_rtB -> B_25_69_552 [ 0 ] = _rtDW -> Memory_PreviousInput_j [ 0 ] ; _rtB ->
B_25_69_552 [ 1 ] = _rtDW -> Memory_PreviousInput_j [ 1 ] ; _rtB ->
B_25_69_552 [ 2 ] = _rtDW -> Memory_PreviousInput_j [ 2 ] ; } if (
rtb_B_25_408_3208 ) { _rtB -> B_25_72_576 [ 0 ] = ( _rtB -> B_25_65_520 [ 0 ]
- B_25_339_2656 [ 0 ] ) * _rtP -> P_13 ; _rtB -> B_25_72_576 [ 1 ] = ( _rtB
-> B_25_65_520 [ 1 ] - B_25_339_2656 [ 1 ] ) * _rtP -> P_13 ; _rtB ->
B_25_72_576 [ 2 ] = ( _rtB -> B_25_65_520 [ 2 ] - B_25_339_2656 [ 2 ] ) *
_rtP -> P_13 ; } else { _rtB -> B_25_72_576 [ 0 ] = _rtB -> B_25_69_552 [ 0 ]
; _rtB -> B_25_72_576 [ 1 ] = _rtB -> B_25_69_552 [ 1 ] ; _rtB -> B_25_72_576
[ 2 ] = _rtB -> B_25_69_552 [ 2 ] ; } B_25_343_2688_idx_0 = _rtB ->
B_25_72_576 [ 0 ] * _rtB -> B_25_72_576 [ 0 ] ; B_25_343_2688_idx_1 = _rtB ->
B_25_72_576 [ 1 ] * _rtB -> B_25_72_576 [ 1 ] ; B_25_72_576_0 = _rtB ->
B_25_72_576 [ 2 ] ; ThreePhase_Rectifier_TrueRMS_e ( S , _rtB -> B_25_123_942
, _rtB -> B_25_62_496 , & _rtB -> TrueRMS_eh , & _rtDW -> TrueRMS_eh , & _rtP
-> TrueRMS_eh , & _rtX -> TrueRMS_eh , & _rtXdis -> TrueRMS_eh ) ;
ThreePhase_Rectifier_RMS_k ( S , _rtB -> B_25_124_943 , _rtB -> B_25_62_496 ,
& _rtB -> RMS_kr , & _rtDW -> RMS_kr , & _rtP -> RMS_kr , & _rtX -> RMS_kr ,
& _rtXdis -> RMS_kr ) ; if ( _rtB -> B_25_123_942 ) { rtb_B_25_314_2456 =
_rtB -> TrueRMS_eh . B_19_183_1408 [ 0 ] ; rtb_B_1_385_3024 = _rtB ->
TrueRMS_eh . B_19_183_1408 [ 1 ] ; B_25_399_3136_idx_2 = _rtB -> TrueRMS_eh .
B_19_183_1408 [ 2 ] ; } else { rtb_B_25_314_2456 = _rtB -> RMS_kr .
B_17_212_1640 [ 0 ] ; rtb_B_1_385_3024 = _rtB -> RMS_kr . B_17_212_1640 [ 1 ]
; B_25_399_3136_idx_2 = _rtB -> RMS_kr . B_17_212_1640 [ 2 ] ; }
B_25_399_3136_0 = rtb_B_25_314_2456 * rtb_B_25_314_2456 ; B_25_343_2688_idx_0
= ( B_25_336_2632_idx_0 - B_25_343_2688_idx_0 ) - B_25_399_3136_0 ; if (
B_25_399_3136_0 > _rtP -> P_69 ) { B_25_399_3136_0 = _rtP -> P_69 ; } else if
( B_25_399_3136_0 < _rtP -> P_70 ) { B_25_399_3136_0 = _rtP -> P_70 ; }
B_25_399_3136_0 = B_25_343_2688_idx_0 / B_25_399_3136_0 ; if (
B_25_399_3136_0 > _rtP -> P_71 ) { B_25_399_3136_0 = _rtP -> P_71 ; } else if
( B_25_399_3136_0 < _rtP -> P_72 ) { B_25_399_3136_0 = _rtP -> P_72 ; }
rtb_B_25_314_2456 = B_25_399_3136_0 ; B_25_336_2632_idx_0 = B_25_399_3136_0 ;
B_25_399_3136_0 = rtb_B_1_385_3024 * rtb_B_1_385_3024 ; B_25_343_2688_idx_0 =
( B_25_336_2632_idx_1 - B_25_343_2688_idx_1 ) - B_25_399_3136_0 ; if (
B_25_399_3136_0 > _rtP -> P_69 ) { B_25_399_3136_0 = _rtP -> P_69 ; } else if
( B_25_399_3136_0 < _rtP -> P_70 ) { B_25_399_3136_0 = _rtP -> P_70 ; }
B_25_399_3136_0 = B_25_343_2688_idx_0 / B_25_399_3136_0 ; if (
B_25_399_3136_0 > _rtP -> P_71 ) { B_25_399_3136_0 = _rtP -> P_71 ; } else if
( B_25_399_3136_0 < _rtP -> P_72 ) { B_25_399_3136_0 = _rtP -> P_72 ; }
rtb_B_1_385_3024 = B_25_399_3136_0 ; B_25_336_2632_idx_1 = B_25_399_3136_0 ;
B_25_399_3136_0 = B_25_399_3136_idx_2 * B_25_399_3136_idx_2 ;
B_25_343_2688_idx_0 = ( B_25_336_2632_idx_2 - B_25_72_576_0 * B_25_72_576_0 )
- B_25_399_3136_0 ; if ( B_25_399_3136_0 > _rtP -> P_69 ) { B_25_399_3136_0 =
_rtP -> P_69 ; } else if ( B_25_399_3136_0 < _rtP -> P_70 ) { B_25_399_3136_0
= _rtP -> P_70 ; } B_25_399_3136_0 = B_25_343_2688_idx_0 / B_25_399_3136_0 ;
if ( B_25_399_3136_0 > _rtP -> P_71 ) { B_25_399_3136_0 = _rtP -> P_71 ; }
else if ( B_25_399_3136_0 < _rtP -> P_72 ) { B_25_399_3136_0 = _rtP -> P_72 ;
} if ( ssIsMajorTimeStep ( S ) != 0 ) { if ( _rtDW -> Sqrt_DWORK1 != 0 ) {
ssSetBlockStateForSolverChangedAtMajorStep ( S ) ;
ssSetContTimeOutputInconsistentWithStateAtMajorStep ( S ) ; _rtDW ->
Sqrt_DWORK1 = 0 ; } rtb_B_25_314_2456 = muDoubleScalarSqrt (
rtb_B_25_314_2456 ) ; rtb_B_1_385_3024 = muDoubleScalarSqrt (
rtb_B_1_385_3024 ) ; B_25_399_3136_idx_2 = muDoubleScalarSqrt (
B_25_399_3136_0 ) ; } else { if ( rtb_B_25_314_2456 < 0.0 ) {
rtb_B_25_314_2456 = - muDoubleScalarSqrt ( muDoubleScalarAbs (
rtb_B_25_314_2456 ) ) ; } else { rtb_B_25_314_2456 = muDoubleScalarSqrt (
rtb_B_25_314_2456 ) ; } if ( B_25_336_2632_idx_0 < 0.0 ) { _rtDW ->
Sqrt_DWORK1 = 1 ; } if ( rtb_B_1_385_3024 < 0.0 ) { rtb_B_1_385_3024 = -
muDoubleScalarSqrt ( muDoubleScalarAbs ( rtb_B_1_385_3024 ) ) ; } else {
rtb_B_1_385_3024 = muDoubleScalarSqrt ( rtb_B_1_385_3024 ) ; } if (
B_25_336_2632_idx_1 < 0.0 ) { _rtDW -> Sqrt_DWORK1 = 1 ; } if (
B_25_399_3136_0 < 0.0 ) { B_25_399_3136_idx_2 = - muDoubleScalarSqrt (
muDoubleScalarAbs ( B_25_399_3136_0 ) ) ; _rtDW -> Sqrt_DWORK1 = 1 ; } else {
B_25_399_3136_idx_2 = muDoubleScalarSqrt ( B_25_399_3136_0 ) ; } } _rtB ->
B_25_75_600 = ( ( rtb_B_25_314_2456 + rtb_B_1_385_3024 ) +
B_25_399_3136_idx_2 ) * _rtP -> P_73 ; ssCallAccelRunBlock ( S , 25 , 95 ,
SS_CALL_MDL_OUTPUTS ) ; isHit = ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0
) { _rtB -> B_25_76_608 = _rtP -> P_74 * _rtB -> B_25_20_160 ; } _rtB ->
B_25_77_616 [ 0 ] = ( _rtB -> B_25_45_360 [ 0 ] - _rtB -> B_1_115_920 [ 0 ] )
/ _rtB -> B_25_76_608 ; _rtB -> B_25_77_616 [ 1 ] = ( _rtB -> B_25_45_360 [ 1
] - _rtB -> B_1_115_920 [ 1 ] ) / _rtB -> B_25_76_608 ; _rtB -> B_25_79_632 =
( 0.0 - _rtB -> B_25_77_616 [ 0 ] ) - _rtB -> B_25_77_616 [ 1 ] ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { _rtB -> B_25_80_640 = _rtP
-> P_75 * rtb_B_25_312_2440 ; } _rtB -> B_25_81_648 [ 0 ] = _rtB ->
B_25_41_328 ; _rtB -> B_25_81_648 [ 1 ] = _rtB -> B_25_48_384 ; _rtB ->
B_25_81_648 [ 2 ] = _rtB -> B_25_42_336 ; isHit = ssIsSampleHit ( S , 2 , 0 )
; if ( isHit != 0 ) { _rtB -> B_25_84_672 = ( _rtB -> B_25_45_360 [ 0 ] *
_rtB -> B_25_41_328 + _rtB -> B_25_47_376 * _rtB -> B_25_48_384 ) + _rtB ->
B_25_45_360 [ 1 ] * _rtB -> B_25_42_336 ; } ssCallAccelRunBlock ( S , 25 ,
110 , SS_CALL_MDL_OUTPUTS ) ; _rtB -> B_25_85_680 [ 0 ] = ( _rtB ->
B_25_77_616 [ 0 ] + _rtB -> B_25_108_864 ) * _rtP -> P_76 ; _rtB ->
B_25_85_680 [ 1 ] = ( _rtB -> B_25_79_632 + _rtB -> B_25_108_864 ) * _rtP ->
P_76 ; _rtB -> B_25_85_680 [ 2 ] = ( _rtB -> B_25_77_616 [ 1 ] + _rtB ->
B_25_108_864 ) * _rtP -> P_76 ; isHit = ssIsSampleHit ( S , 1 , 0 ) ; if (
isHit != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_Gain1_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_Gain1_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_25_85_680 [ 0 ] + 0 ) ; } }
} if ( ssIsModeUpdateTimeStep ( S ) ) { rtb_B_25_312_2440 = _rtB ->
B_25_85_680 [ 0 ] ; _rtDW -> MinMax1_MODE = 0 ; for ( i = 0 ; i < 3 ; i ++ )
{ if ( ( rtb_B_25_312_2440 != rtb_B_25_312_2440 ) || ( _rtB -> B_25_85_680 [
i ] > rtb_B_25_312_2440 ) ) { rtb_B_25_312_2440 = _rtB -> B_25_85_680 [ i ] ;
_rtDW -> MinMax1_MODE = i ; } } _rtB -> B_25_88_704 = rtb_B_25_312_2440 ; }
else { _rtB -> B_25_88_704 = _rtB -> B_25_85_680 [ _rtDW -> MinMax1_MODE ] ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_MinMax1_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_MinMax1_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_25_88_704 + 0 ) ; } } } if (
ssIsModeUpdateTimeStep ( S ) ) { rtb_B_25_312_2440 = _rtB -> B_25_85_680 [ 0
] ; _rtDW -> MinMax_MODE = 0 ; for ( i = 0 ; i < 3 ; i ++ ) { if ( (
rtb_B_25_312_2440 != rtb_B_25_312_2440 ) || ( _rtB -> B_25_85_680 [ i ] <
rtb_B_25_312_2440 ) ) { rtb_B_25_312_2440 = _rtB -> B_25_85_680 [ i ] ; _rtDW
-> MinMax_MODE = i ; } } _rtB -> B_25_89_712 = rtb_B_25_312_2440 ; } else {
_rtB -> B_25_89_712 = _rtB -> B_25_85_680 [ _rtDW -> MinMax_MODE ] ; } isHit
= ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { { if ( _rtDW ->
TAQSigLogging_InsertedFor_MinMax_at_outport_0_PWORK . AQHandles &&
ssGetLogOutput ( S ) ) { sdiWriteSignal ( _rtDW ->
TAQSigLogging_InsertedFor_MinMax_at_outport_0_PWORK . AQHandles ,
ssGetTaskTime ( S , 1 ) , ( char * ) & _rtB -> B_25_89_712 + 0 ) ; } } }
rtb_B_25_312_2440 = ( ( _rtB -> B_25_108_864 - _rtB -> B_25_88_704 ) - _rtB
-> B_25_89_712 ) * _rtP -> P_77 ; _rtB -> B_25_90_720 [ 0 ] = _rtB ->
B_25_85_680 [ 0 ] + rtb_B_25_312_2440 ; _rtB -> B_25_90_720 [ 1 ] = _rtB ->
B_25_85_680 [ 1 ] + rtb_B_25_312_2440 ; _rtB -> B_25_90_720 [ 2 ] = _rtB ->
B_25_85_680 [ 2 ] + rtb_B_25_312_2440 ; _rtB -> B_25_94_752 = look1_binlxpw (
muDoubleScalarRem ( ssGetT ( S ) - _rtB -> B_25_93_744 , _rtB -> B_25_109_872
) , _rtP -> P_79 , _rtP -> P_78 , 2U ) ; isHit = ssIsSampleHit ( S , 1 , 0 )
; if ( isHit != 0 ) { if ( ssIsModeUpdateTimeStep ( S ) ) { _rtDW ->
RelationalOperator7_Mode = ( _rtB -> B_25_90_720 [ 0 ] >= _rtB -> B_25_94_752
) ; _rtDW -> RelationalOperator6_Mode = ( _rtB -> B_25_90_720 [ 1 ] >= _rtB
-> B_25_94_752 ) ; _rtDW -> RelationalOperator2_Mode = ( _rtB -> B_25_90_720
[ 2 ] >= _rtB -> B_25_94_752 ) ; } _rtB -> B_25_95_760 [ 0 ] = _rtDW ->
RelationalOperator7_Mode ; _rtB -> B_25_95_760 [ 1 ] = ! _rtDW ->
RelationalOperator7_Mode ; _rtB -> B_25_95_760 [ 2 ] = _rtDW ->
RelationalOperator6_Mode ; _rtB -> B_25_95_760 [ 3 ] = ! _rtDW ->
RelationalOperator6_Mode ; _rtB -> B_25_95_760 [ 4 ] = _rtDW ->
RelationalOperator2_Mode ; _rtB -> B_25_95_760 [ 5 ] = ! _rtDW ->
RelationalOperator2_Mode ; } UNUSED_PARAMETER ( tid ) ; } static void
mdlOutputsTID3 ( SimStruct * S , int_T tid ) { B_ThreePhase_Rectifier_T *
_rtB ; P_ThreePhase_Rectifier_T * _rtP ; real_T tmp ; int32_T i ; _rtP = ( (
P_ThreePhase_Rectifier_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_ThreePhase_Rectifier_T * ) _ssGetModelBlockIO ( S ) ) ; for ( i = 0 ; i < 6
; i ++ ) { _rtB -> B_25_101_808 [ i ] = _rtP -> P_80 [ i ] ; } if ( _rtP ->
P_84 > _rtP -> P_86 ) { tmp = _rtP -> P_82 * muDoubleScalarCos ( _rtP -> P_81
) * _rtP -> P_83 ; } else { tmp = _rtP -> P_82 * muDoubleScalarCos ( _rtP ->
P_81 ) * _rtP -> P_85 ; } _rtB -> B_25_107_856 = tmp + _rtP -> P_87 ;
ssCallAccelRunBlock ( S , 25 , 169 , SS_CALL_MDL_OUTPUTS ) ; _rtB ->
B_25_117_936 = ( _rtP -> P_88 != 0.0 ) ; _rtB -> B_25_118_937 = ! _rtB ->
B_25_117_936 ; _rtB -> B_25_119_938 = ( _rtP -> P_89 != 0.0 ) ; _rtB ->
B_25_120_939 = ! _rtB -> B_25_119_938 ; _rtB -> B_25_121_940 = ( _rtP -> P_90
!= 0.0 ) ; _rtB -> B_25_122_941 = ! _rtB -> B_25_121_940 ; _rtB ->
B_25_123_942 = ( _rtP -> P_91 != 0.0 ) ; _rtB -> B_25_124_943 = ! _rtB ->
B_25_123_942 ; _rtB -> B_25_108_864 = _rtP -> P_92 ; _rtB -> B_25_109_872 =
_rtP -> P_93 ; _rtB -> B_25_110_880 = _rtP -> P_94 ; UNUSED_PARAMETER ( tid )
; }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) {
B_ThreePhase_Rectifier_T * _rtB ; DW_ThreePhase_Rectifier_T * _rtDW ;
P_ThreePhase_Rectifier_T * _rtP ; int32_T isHit ; _rtDW = ( (
DW_ThreePhase_Rectifier_T * ) ssGetRootDWork ( S ) ) ; _rtP = ( (
P_ThreePhase_Rectifier_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_ThreePhase_Rectifier_T * ) _ssGetModelBlockIO ( S ) ) ; isHit =
ssIsSampleHit ( S , 2 , 0 ) ; if ( isHit != 0 ) { ssCallAccelRunBlock ( S ,
25 , 3 , SS_CALL_MDL_UPDATE ) ; } { real_T * * uBuffer = ( real_T * * ) &
_rtDW -> TransportDelay_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT
( S ) ; for ( isHit = 0 ; isHit < 6 ; isHit ++ ) { _rtDW ->
TransportDelay_IWORK . Head [ isHit ] = ( ( _rtDW -> TransportDelay_IWORK .
Head [ isHit ] < ( _rtDW -> TransportDelay_IWORK . CircularBufSize [ isHit ]
- 1 ) ) ? ( _rtDW -> TransportDelay_IWORK . Head [ isHit ] + 1 ) : 0 ) ; if (
_rtDW -> TransportDelay_IWORK . Head [ isHit ] == _rtDW ->
TransportDelay_IWORK . Tail [ isHit ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK . CircularBufSize [ isHit ] , & _rtDW ->
TransportDelay_IWORK . Tail [ isHit ] , & _rtDW -> TransportDelay_IWORK .
Head [ isHit ] , & _rtDW -> TransportDelay_IWORK . Last [ isHit ] , simTime -
_rtP -> P_38 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T *
) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK . CircularBufSize [ isHit ] ) [ _rtDW ->
TransportDelay_IWORK . Head [ isHit ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> TransportDelay_IWORK . Head [ isHit ] ] = _rtB -> B_25_21_168 [ isHit ] ;
} } isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { for ( isHit = 0
; isHit < 6 ; isHit ++ ) { _rtDW -> Memory_PreviousInput [ isHit ] = _rtB ->
B_25_34_272 [ isHit ] ; } } { real_T * * uBuffer = ( real_T * * ) & _rtDW ->
VariableTimeDelay1_PWORK . TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S )
; _rtDW -> VariableTimeDelay1_IWORK . Head [ 0 ] = ( ( _rtDW ->
VariableTimeDelay1_IWORK . Head [ 0 ] < ( _rtDW -> VariableTimeDelay1_IWORK .
CircularBufSize [ 0 ] - 1 ) ) ? ( _rtDW -> VariableTimeDelay1_IWORK . Head [
0 ] + 1 ) : 0 ) ; if ( _rtDW -> VariableTimeDelay1_IWORK . Head [ 0 ] ==
_rtDW -> VariableTimeDelay1_IWORK . Tail [ 0 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTimeDelay1_IWORK . CircularBufSize [ 0 ] , & _rtDW ->
VariableTimeDelay1_IWORK . Tail [ 0 ] , & _rtDW -> VariableTimeDelay1_IWORK .
Head [ 0 ] , & _rtDW -> VariableTimeDelay1_IWORK . Last [ 0 ] , simTime -
_rtP -> P_44 , uBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
VariableTimeDelay1_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * uBuffer + _rtDW ->
VariableTimeDelay1_IWORK . CircularBufSize [ 0 ] ) [ _rtDW ->
VariableTimeDelay1_IWORK . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW
-> VariableTimeDelay1_IWORK . Head [ 0 ] ] = _rtB -> B_25_45_360 [ 0 ] ;
_rtDW -> VariableTimeDelay1_IWORK . Head [ 1 ] = ( ( _rtDW ->
VariableTimeDelay1_IWORK . Head [ 1 ] < ( _rtDW -> VariableTimeDelay1_IWORK .
CircularBufSize [ 1 ] - 1 ) ) ? ( _rtDW -> VariableTimeDelay1_IWORK . Head [
1 ] + 1 ) : 0 ) ; if ( _rtDW -> VariableTimeDelay1_IWORK . Head [ 1 ] ==
_rtDW -> VariableTimeDelay1_IWORK . Tail [ 1 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
VariableTimeDelay1_IWORK . CircularBufSize [ 1 ] , & _rtDW ->
VariableTimeDelay1_IWORK . Tail [ 1 ] , & _rtDW -> VariableTimeDelay1_IWORK .
Head [ 1 ] , & _rtDW -> VariableTimeDelay1_IWORK . Last [ 1 ] , simTime -
_rtP -> P_44 , uBuffer , ( boolean_T ) 0 , ( boolean_T ) 0 , & _rtDW ->
VariableTimeDelay1_IWORK . MaxNewBufSize ) ) { ssSetErrorStatus ( S ,
"vtdelay memory allocation error" ) ; return ; } } ( * uBuffer + _rtDW ->
VariableTimeDelay1_IWORK . CircularBufSize [ 1 ] ) [ _rtDW ->
VariableTimeDelay1_IWORK . Head [ 1 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
VariableTimeDelay1_IWORK . Head [ 1 ] ] = _rtB -> B_25_45_360 [ 1 ] ; } {
real_T * * uBuffer = ( real_T * * ) & _rtDW -> TransportDelay_PWORK_n .
TUbufferPtrs [ 0 ] ; real_T simTime = ssGetT ( S ) ; _rtDW ->
TransportDelay_IWORK_f . Head = ( ( _rtDW -> TransportDelay_IWORK_f . Head <
( _rtDW -> TransportDelay_IWORK_f . CircularBufSize - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_f . Head + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_f . Head == _rtDW -> TransportDelay_IWORK_f . Tail ) {
if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_f . CircularBufSize , & _rtDW -> TransportDelay_IWORK_f
. Tail , & _rtDW -> TransportDelay_IWORK_f . Head , & _rtDW ->
TransportDelay_IWORK_f . Last , simTime - _rtP -> P_55 , uBuffer , (
boolean_T ) 0 , false , & _rtDW -> TransportDelay_IWORK_f . MaxNewBufSize ) )
{ ssSetErrorStatus ( S , ( char_T * ) "\"tdelay memory allocation error\"" )
; return ; } } ( * uBuffer + _rtDW -> TransportDelay_IWORK_f .
CircularBufSize ) [ _rtDW -> TransportDelay_IWORK_f . Head ] = simTime ; ( *
uBuffer ) [ _rtDW -> TransportDelay_IWORK_f . Head ] = _rtB -> B_25_49_392 ;
} isHit = ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_g = _rtB -> B_25_52_416 ; }
ThreePhase_Rectifier_TrueRMS_Update ( S , & _rtB -> TrueRMS_f , & _rtDW ->
TrueRMS_f , & _rtP -> TrueRMS_f ) ; ThreePhase_Rectifier_RMS_Update ( S , &
_rtB -> RMS_g , & _rtDW -> RMS_g , & _rtP -> RMS_g ) ;
ThreePhase_Rectifier_TrueRMS_Update ( S , & _rtB -> TrueRMS , & _rtDW ->
TrueRMS , & _rtP -> TrueRMS ) ; ThreePhase_Rectifier_RMS_Update ( S , & _rtB
-> RMS , & _rtDW -> RMS , & _rtP -> RMS ) ; { real_T * * uBuffer = ( real_T *
* ) & _rtDW -> TransportDelay_PWORK_i . TUbufferPtrs [ 0 ] ; real_T simTime =
ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_n . Head [ 0 ] = ( ( _rtDW ->
TransportDelay_IWORK_n . Head [ 0 ] < ( _rtDW -> TransportDelay_IWORK_n .
CircularBufSize [ 0 ] - 1 ) ) ? ( _rtDW -> TransportDelay_IWORK_n . Head [ 0
] + 1 ) : 0 ) ; if ( _rtDW -> TransportDelay_IWORK_n . Head [ 0 ] == _rtDW ->
TransportDelay_IWORK_n . Tail [ 0 ] ) { if ( !
ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( & _rtDW ->
TransportDelay_IWORK_n . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_n . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_n .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_n . Last [ 0 ] , simTime - _rtP
-> P_60 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK_n . CircularBufSize [ 0 ] ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 0 ] ] = _rtB -> B_25_55_440 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_n . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_n .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_n . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_n . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_n .
Tail [ 1 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_n . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_n .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_n . Last [ 1 ] , simTime - _rtP
-> P_60 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK_n . CircularBufSize [ 1 ] ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 1 ] ] = _rtB -> B_25_55_440 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_n . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_n .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_n . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_n . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_n .
Tail [ 2 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& _rtDW -> TransportDelay_IWORK_n . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_n . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_n .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_n . Last [ 2 ] , simTime - _rtP
-> P_60 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_n . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK_n . CircularBufSize [ 2 ] ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_n . Head [ 2 ] ] = _rtB -> B_25_55_440 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_o [ 0 ] = _rtB -> B_25_62_496 [ 0 ] ; _rtDW ->
Memory_PreviousInput_o [ 1 ] = _rtB -> B_25_62_496 [ 1 ] ; _rtDW ->
Memory_PreviousInput_o [ 2 ] = _rtB -> B_25_62_496 [ 2 ] ; }
ThreePhase_Rectifier_TrueRMS_c_Update ( S , & _rtB -> TrueRMS_e , & _rtDW ->
TrueRMS_e , & _rtP -> TrueRMS_e ) ; ThreePhase_Rectifier_RMS_j_Update ( S , &
_rtB -> RMS_k , & _rtDW -> RMS_k , & _rtP -> RMS_k ) ; { real_T * * uBuffer =
( real_T * * ) & _rtDW -> TransportDelay_PWORK_d . TUbufferPtrs [ 0 ] ;
real_T simTime = ssGetT ( S ) ; _rtDW -> TransportDelay_IWORK_h . Head [ 0 ]
= ( ( _rtDW -> TransportDelay_IWORK_h . Head [ 0 ] < ( _rtDW ->
TransportDelay_IWORK_h . CircularBufSize [ 0 ] - 1 ) ) ? ( _rtDW ->
TransportDelay_IWORK_h . Head [ 0 ] + 1 ) : 0 ) ; if ( _rtDW ->
TransportDelay_IWORK_h . Head [ 0 ] == _rtDW -> TransportDelay_IWORK_h . Tail
[ 0 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf ( &
_rtDW -> TransportDelay_IWORK_h . CircularBufSize [ 0 ] , & _rtDW ->
TransportDelay_IWORK_h . Tail [ 0 ] , & _rtDW -> TransportDelay_IWORK_h .
Head [ 0 ] , & _rtDW -> TransportDelay_IWORK_h . Last [ 0 ] , simTime - _rtP
-> P_65 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_h . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK_h . CircularBufSize [ 0 ] ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ 0 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ 0 ] ] = _rtB -> B_25_65_520 [ 0 ] ; _rtDW ->
TransportDelay_IWORK_h . Head [ 1 ] = ( ( _rtDW -> TransportDelay_IWORK_h .
Head [ 1 ] < ( _rtDW -> TransportDelay_IWORK_h . CircularBufSize [ 1 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_h . Head [ 1 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_h . Head [ 1 ] == _rtDW -> TransportDelay_IWORK_h .
Tail [ 1 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& _rtDW -> TransportDelay_IWORK_h . CircularBufSize [ 1 ] , & _rtDW ->
TransportDelay_IWORK_h . Tail [ 1 ] , & _rtDW -> TransportDelay_IWORK_h .
Head [ 1 ] , & _rtDW -> TransportDelay_IWORK_h . Last [ 1 ] , simTime - _rtP
-> P_65 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_h . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK_h . CircularBufSize [ 1 ] ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ 1 ] ] = simTime ; ( * uBuffer ++ ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ 1 ] ] = _rtB -> B_25_65_520 [ 1 ] ; _rtDW ->
TransportDelay_IWORK_h . Head [ 2 ] = ( ( _rtDW -> TransportDelay_IWORK_h .
Head [ 2 ] < ( _rtDW -> TransportDelay_IWORK_h . CircularBufSize [ 2 ] - 1 )
) ? ( _rtDW -> TransportDelay_IWORK_h . Head [ 2 ] + 1 ) : 0 ) ; if ( _rtDW
-> TransportDelay_IWORK_h . Head [ 2 ] == _rtDW -> TransportDelay_IWORK_h .
Tail [ 2 ] ) { if ( ! ThreePhase_Rectifier_acc_rt_TDelayUpdateTailOrGrowBuf (
& _rtDW -> TransportDelay_IWORK_h . CircularBufSize [ 2 ] , & _rtDW ->
TransportDelay_IWORK_h . Tail [ 2 ] , & _rtDW -> TransportDelay_IWORK_h .
Head [ 2 ] , & _rtDW -> TransportDelay_IWORK_h . Last [ 2 ] , simTime - _rtP
-> P_65 , uBuffer , ( boolean_T ) 0 , false , & _rtDW ->
TransportDelay_IWORK_h . MaxNewBufSize ) ) { ssSetErrorStatus ( S , ( char_T
* ) "\"tdelay memory allocation error\"" ) ; return ; } } ( * uBuffer + _rtDW
-> TransportDelay_IWORK_h . CircularBufSize [ 2 ] ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ 2 ] ] = simTime ; ( * uBuffer ) [ _rtDW ->
TransportDelay_IWORK_h . Head [ 2 ] ] = _rtB -> B_25_65_520 [ 2 ] ; } isHit =
ssIsSampleHit ( S , 1 , 0 ) ; if ( isHit != 0 ) { _rtDW ->
Memory_PreviousInput_j [ 0 ] = _rtB -> B_25_72_576 [ 0 ] ; _rtDW ->
Memory_PreviousInput_j [ 1 ] = _rtB -> B_25_72_576 [ 1 ] ; _rtDW ->
Memory_PreviousInput_j [ 2 ] = _rtB -> B_25_72_576 [ 2 ] ; }
ThreePhase_Rectifier_TrueRMS_c_Update ( S , & _rtB -> TrueRMS_eh , & _rtDW ->
TrueRMS_eh , & _rtP -> TrueRMS_eh ) ; ThreePhase_Rectifier_RMS_j_Update ( S ,
& _rtB -> RMS_kr , & _rtDW -> RMS_kr , & _rtP -> RMS_kr ) ; UNUSED_PARAMETER
( tid ) ; }
#define MDL_UPDATE
static void mdlUpdateTID3 ( SimStruct * S , int_T tid ) { UNUSED_PARAMETER (
tid ) ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { B_ThreePhase_Rectifier_T *
_rtB ; DW_ThreePhase_Rectifier_T * _rtDW ; P_ThreePhase_Rectifier_T * _rtP ;
XDot_ThreePhase_Rectifier_T * _rtXdot ; X_ThreePhase_Rectifier_T * _rtX ;
int32_T i ; _rtDW = ( ( DW_ThreePhase_Rectifier_T * ) ssGetRootDWork ( S ) )
; _rtXdot = ( ( XDot_ThreePhase_Rectifier_T * ) ssGetdX ( S ) ) ; _rtX = ( (
X_ThreePhase_Rectifier_T * ) ssGetContStates ( S ) ) ; _rtP = ( (
P_ThreePhase_Rectifier_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_ThreePhase_Rectifier_T * ) _ssGetModelBlockIO ( S ) ) ; for ( i = 0 ; i < 6
; i ++ ) { _rtXdot -> integrator_CSTATE [ i ] = _rtB -> B_25_95_760 [ i ] ; }
_rtXdot -> Integrator_CSTATE = _rtB -> B_25_80_640 ; { } _rtXdot ->
TransferFcn_CSTATE [ 0 ] = 0.0 ; _rtXdot -> TransferFcn_CSTATE [ 0 ] += _rtP
-> P_5 [ 0 ] * _rtX -> TransferFcn_CSTATE [ 0 ] ; _rtXdot ->
TransferFcn_CSTATE [ 1 ] = 0.0 ; _rtXdot -> TransferFcn_CSTATE [ 0 ] += _rtP
-> P_5 [ 1 ] * _rtX -> TransferFcn_CSTATE [ 1 ] ; _rtXdot ->
TransferFcn_CSTATE [ 1 ] += _rtX -> TransferFcn_CSTATE [ 0 ] ; _rtXdot ->
TransferFcn_CSTATE [ 0 ] += _rtB -> B_1_111_888 ; _rtXdot ->
TransferFcn1_CSTATE [ 0 ] = 0.0 ; _rtXdot -> TransferFcn1_CSTATE [ 0 ] +=
_rtP -> P_7 [ 0 ] * _rtX -> TransferFcn1_CSTATE [ 0 ] ; _rtXdot ->
TransferFcn1_CSTATE [ 1 ] = 0.0 ; _rtXdot -> TransferFcn1_CSTATE [ 0 ] +=
_rtP -> P_7 [ 1 ] * _rtX -> TransferFcn1_CSTATE [ 1 ] ; _rtXdot ->
TransferFcn1_CSTATE [ 1 ] += _rtX -> TransferFcn1_CSTATE [ 0 ] ; _rtXdot ->
TransferFcn1_CSTATE [ 0 ] += _rtB -> B_1_112_896 ; _rtXdot ->
integrator_CSTATE_j = _rtB -> B_25_84_672 ;
ThreePhase_Rectifier_TrueRMS_Deriv ( S , & _rtB -> TrueRMS_f , & _rtDW ->
TrueRMS_f , & _rtXdot -> TrueRMS_f ) ; ThreePhase_Rectifier_RMS_Deriv ( S , &
_rtB -> RMS_g , & _rtDW -> RMS_g , & _rtXdot -> RMS_g ) ;
ThreePhase_Rectifier_TrueRMS_Deriv ( S , & _rtB -> TrueRMS , & _rtDW ->
TrueRMS , & _rtXdot -> TrueRMS ) ; ThreePhase_Rectifier_RMS_Deriv ( S , &
_rtB -> RMS , & _rtDW -> RMS , & _rtXdot -> RMS ) ; _rtXdot ->
integrator_CSTATE_p [ 0 ] = _rtB -> B_25_81_648 [ 0 ] ; _rtXdot ->
integrator_CSTATE_p [ 1 ] = _rtB -> B_25_81_648 [ 1 ] ; _rtXdot ->
integrator_CSTATE_p [ 2 ] = _rtB -> B_25_81_648 [ 2 ] ;
ThreePhase_Rectifier_TrueRMS_e_Deriv ( S , & _rtB -> TrueRMS_e , & _rtDW ->
TrueRMS_e , & _rtXdot -> TrueRMS_e ) ; ThreePhase_Rectifier_RMS_h_Deriv ( S ,
& _rtB -> RMS_k , & _rtDW -> RMS_k , & _rtXdot -> RMS_k ) ; _rtXdot ->
integrator_CSTATE_l [ 0 ] = _rtB -> B_25_62_496 [ 0 ] ; _rtXdot ->
integrator_CSTATE_l [ 1 ] = _rtB -> B_25_62_496 [ 1 ] ; _rtXdot ->
integrator_CSTATE_l [ 2 ] = _rtB -> B_25_62_496 [ 2 ] ;
ThreePhase_Rectifier_TrueRMS_e_Deriv ( S , & _rtB -> TrueRMS_eh , & _rtDW ->
TrueRMS_eh , & _rtXdot -> TrueRMS_eh ) ; ThreePhase_Rectifier_RMS_h_Deriv ( S
, & _rtB -> RMS_kr , & _rtDW -> RMS_kr , & _rtXdot -> RMS_kr ) ; }
#define MDL_ZERO_CROSSINGS
static void mdlZeroCrossings ( SimStruct * S ) { B_ThreePhase_Rectifier_T *
_rtB ; DW_ThreePhase_Rectifier_T * _rtDW ; P_ThreePhase_Rectifier_T * _rtP ;
ZCV_ThreePhase_Rectifier_T * _rtZCSV ; real_T maxV ; _rtDW = ( (
DW_ThreePhase_Rectifier_T * ) ssGetRootDWork ( S ) ) ; _rtZCSV = ( (
ZCV_ThreePhase_Rectifier_T * ) ssGetSolverZcSignalVector ( S ) ) ; _rtP = ( (
P_ThreePhase_Rectifier_T * ) ssGetModelRtp ( S ) ) ; _rtB = ( (
B_ThreePhase_Rectifier_T * ) _ssGetModelBlockIO ( S ) ) ; _rtZCSV ->
Saturation_UprLim_ZC [ 0 ] = _rtB -> B_1_113_904 - _rtP -> P_9 ; _rtZCSV ->
Saturation_LwrLim_ZC [ 0 ] = _rtB -> B_1_113_904 - _rtP -> P_10 ; _rtZCSV ->
Saturation_UprLim_ZC [ 1 ] = _rtB -> B_1_114_912 - _rtP -> P_9 ; _rtZCSV ->
Saturation_LwrLim_ZC [ 1 ] = _rtB -> B_1_114_912 - _rtP -> P_10 ;
ThreePhase_Rectifier_TrueRMS_ZC ( S , & _rtB -> TrueRMS_f , & _rtDW ->
TrueRMS_f , & _rtP -> TrueRMS_f , & _rtZCSV -> TrueRMS_f ) ;
ThreePhase_Rectifier_TrueRMS_ZC ( S , & _rtB -> TrueRMS , & _rtDW -> TrueRMS
, & _rtP -> TrueRMS , & _rtZCSV -> TrueRMS ) ;
ThreePhase_Rectifier_TrueRMS_a_ZC ( S , & _rtB -> TrueRMS_e , & _rtDW ->
TrueRMS_e , & _rtP -> TrueRMS_e , & _rtZCSV -> TrueRMS_e ) ;
ThreePhase_Rectifier_TrueRMS_a_ZC ( S , & _rtB -> TrueRMS_eh , & _rtDW ->
TrueRMS_eh , & _rtP -> TrueRMS_eh , & _rtZCSV -> TrueRMS_eh ) ; maxV = _rtB
-> B_25_85_680 [ 0 ] ; if ( _rtB -> B_25_85_680 [ 0 ] != _rtB -> B_25_85_680
[ 0 ] ) { maxV = _rtB -> B_25_85_680 [ 0 ] ; } if ( ( maxV != maxV ) || (
_rtB -> B_25_85_680 [ 1 ] > maxV ) ) { maxV = _rtB -> B_25_85_680 [ 1 ] ; }
if ( ( maxV != maxV ) || ( _rtB -> B_25_85_680 [ 2 ] > maxV ) ) { maxV = _rtB
-> B_25_85_680 [ 2 ] ; } _rtZCSV -> MinMax1_MinmaxInput_ZC = maxV - _rtB ->
B_25_85_680 [ _rtDW -> MinMax1_MODE ] ; maxV = _rtB -> B_25_85_680 [ 0 ] ; if
( _rtB -> B_25_85_680 [ 0 ] != _rtB -> B_25_85_680 [ 0 ] ) { maxV = _rtB ->
B_25_85_680 [ 0 ] ; } if ( ( maxV != maxV ) || ( _rtB -> B_25_85_680 [ 1 ] <
maxV ) ) { maxV = _rtB -> B_25_85_680 [ 1 ] ; } if ( ( maxV != maxV ) || (
_rtB -> B_25_85_680 [ 2 ] < maxV ) ) { maxV = _rtB -> B_25_85_680 [ 2 ] ; }
_rtZCSV -> MinMax_MinmaxInput_ZC = maxV - _rtB -> B_25_85_680 [ _rtDW ->
MinMax_MODE ] ; _rtZCSV -> RelationalOperator7_RelopInput_ZC = _rtB ->
B_25_90_720 [ 0 ] - _rtB -> B_25_94_752 ; _rtZCSV ->
RelationalOperator6_RelopInput_ZC = _rtB -> B_25_90_720 [ 1 ] - _rtB ->
B_25_94_752 ; _rtZCSV -> RelationalOperator2_RelopInput_ZC = _rtB ->
B_25_90_720 [ 2 ] - _rtB -> B_25_94_752 ; } static void mdlInitializeSizes (
SimStruct * S ) { ssSetChecksumVal ( S , 0 , 1476554251U ) ; ssSetChecksumVal
( S , 1 , 829550979U ) ; ssSetChecksumVal ( S , 2 , 783730817U ) ;
ssSetChecksumVal ( S , 3 , 2767968599U ) ; { mxArray * slVerStructMat = (
NULL ) ; mxArray * slStrMat = mxCreateString ( "simulink" ) ; char slVerChar
[ 10 ] ; int status = mexCallMATLAB ( 1 , & slVerStructMat , 1 , & slStrMat ,
"ver" ) ; if ( status == 0 ) { mxArray * slVerMat = mxGetField (
slVerStructMat , 0 , "Version" ) ; if ( slVerMat == ( NULL ) ) { status = 1 ;
} else { status = mxGetString ( slVerMat , slVerChar , 10 ) ; } }
mxDestroyArray ( slStrMat ) ; mxDestroyArray ( slVerStructMat ) ; if ( (
status == 1 ) || ( strcmp ( slVerChar , "24.1" ) != 0 ) ) { return ; } }
ssSetOptions ( S , SS_OPTION_EXCEPTION_FREE_CODE ) ; if ( ssGetSizeofDWork (
S ) != ( SLSize ) sizeof ( DW_ThreePhase_Rectifier_T ) ) { static char msg [
256 ] ; snprintf ( msg , 256 , "Unexpected error: Internal DWork sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofDWork ( S ) , ( unsigned long ) sizeof ( DW_ThreePhase_Rectifier_T
) ) ; ssSetErrorStatus ( S , msg ) ; } if ( ssGetSizeofGlobalBlockIO ( S ) !=
( SLSize ) sizeof ( B_ThreePhase_Rectifier_T ) ) { static char msg [ 256 ] ;
snprintf ( msg , 256 , "Unexpected error: Internal BlockIO sizes do "
"not match for accelerator mex file (%ld vs %lu)." , ( signed long )
ssGetSizeofGlobalBlockIO ( S ) , ( unsigned long ) sizeof (
B_ThreePhase_Rectifier_T ) ) ; ssSetErrorStatus ( S , msg ) ; } { int
ssSizeofParams ; ssGetSizeofParams ( S , & ssSizeofParams ) ; if (
ssSizeofParams != sizeof ( P_ThreePhase_Rectifier_T ) ) { static char msg [
256 ] ; snprintf ( msg , 256 ,
"Unexpected error: Internal Parameters sizes do "
"not match for accelerator mex file (%d vs %lu)." , ssSizeofParams , (
unsigned long ) sizeof ( P_ThreePhase_Rectifier_T ) ) ; ssSetErrorStatus ( S
, msg ) ; } } _ssSetModelRtp ( S , ( real_T * ) &
ThreePhase_Rectifier_rtDefaultP ) ; } static void mdlInitializeSampleTimes (
SimStruct * S ) { slAccRegPrmChangeFcn ( S , mdlOutputsTID3 ) ; } static void
mdlTerminate ( SimStruct * S ) { }
#include "simulink.c"
