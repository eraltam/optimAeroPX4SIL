#ifndef VehiclePlant_private_h_
#define VehiclePlant_private_h_
#include "rtwtypes.h"
#include "model_reference_types.h"
#include "builtin_typeid_types.h"
#include "multiword_types.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "VehiclePlant_types.h"
#include "environment.h"
#include "sensors.h"
#include "actuatorsC172p.h"
#include "VehiclePlant.h"
#include "sfrtif/sim_shared_utils.h"
#include "sfrtif/sfc_sdi.h"
#include "rtw_modelmap.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "simtarget/slMdlrefSimTargetCoreHeaders.h"
#include "simtarget/slMdlrefSimTargetInstrumentationHeaders.h"
#include "messages/slMessagesSfcnBridge.h"
#include "sl_fileio_rtw.h"
extern const char_T * rt_GetMatSignalLoggingFileName ( void ) ; extern const
char_T * rt_GetMatSigLogSelectorFileName ( void ) ; extern void *
rt_GetOSigstreamManager ( void ) ; extern void * *
rt_GetOSigstreamManagerAddr ( void ) ; extern void * rt_slioCatalogue ( void
) ; extern void * * rt_slioCatalogueAddr ( void ) ; extern void
rt_ssSet_slErrMsg ( void * S , void * diag ) ; extern void
rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) ; extern void
rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) ; extern const char_T
* rt_CreateFullPathToTop ( const char_T * toppath , const char_T * subpath )
; extern boolean_T slIsRapidAcceleratorSimulating ( void ) ; extern void
rt_RAccelReplaceFromFilename ( SimStruct * S , const char_T * blockpath ,
char_T * fileNam ) ; extern void rt_RAccelReplaceToFilename ( SimStruct * S ,
const char_T * blockpath , char_T * fileNam ) ; extern void *
gblLoggingInterval ;
#ifndef rtmCancelTimerToRaiseEvent
#define rtmCancelTimerToRaiseEvent(rtm, timerIndex) simTgtCancelTimerToRaiseEvent(_ssGetRootSS((rtm)->_mdlRefSfcnS), [timerIndex])
#endif
#ifndef rtmDisableTaskUsingRateIndex
#define rtmDisableTaskUsingRateIndex(rtm, sti) simTgtDisableTaskUsingRateIndex(_ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti])
#endif
#ifndef rtmEnableTaskUsingRateIndex
#define rtmEnableTaskUsingRateIndex(rtm, sti) simTgtEnableTaskUsingRateIndex(_ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti])
#endif
#ifndef rtmIsFirstInitCond
#define rtmIsFirstInitCond(rtm) ssIsFirstInitCond((rtm)->_mdlRefSfcnS)
#endif
#ifndef rtmIsMajorTimeStep
#define rtmIsMajorTimeStep(rtm) ssIsMajorTimeStep((rtm)->_mdlRefSfcnS)
#endif
#ifndef rtmIsMinorTimeStep
#define rtmIsMinorTimeStep(rtm) ssIsMinorTimeStep((rtm)->_mdlRefSfcnS)
#endif
#ifndef rtmRaiseEvent
#define rtmRaiseEvent(rtm, runtimeEventIndex) simTgtRaiseEvent(_ssGetRootSS((rtm)->_mdlRefSfcnS), [runtimeEventIndex])
#endif
#ifndef rtmRaiseEventWithEnqueue
#define rtmRaiseEventWithEnqueue(rtm, runtimeEventIndex) simTgtRaiseEventWithEnqueue(_ssGetRootSS((rtm)->_mdlRefSfcnS), [runtimeEventIndex])
#endif
#ifndef rtmRaiseWhenTimerExpiresAfter
#define rtmRaiseWhenTimerExpiresAfter(rtm, timerIndex, isRecurring, afterDur, runtimeEventIndex) simTgtRaiseWhenTimerExpiresAfter(_ssGetRootSS((rtm)->_mdlRefSfcnS), [timerIndex], isRecurring, afterDur, [runtimeEventIndex])
#endif
#ifndef rtmRaiseWhenTimerExpiresAt
#define rtmRaiseWhenTimerExpiresAt(rtm, timerIndex, atT, runtimeEventIndex) simTgtRaiseWhenTimerExpiresAt(_ssGetRootSS((rtm)->_mdlRefSfcnS), [timerIndex], atT, [runtimeEventIndex])
#endif
#ifndef rtmSetTFinal
#define rtmSetTFinal(rtm, val) ((rtm)->Timing.tFinal = (val))
#endif
typedef struct { AircraftForcesMomentsBus pfiutvgoyw ; MassPropertiesBus
mmiiu4qwhn ; real_T oxkktkuqej [ 3 ] ; real_T flyprwsa2y [ 3 ] ; real_T
d3owfntqj2 [ 9 ] ; real_T lk033mmsxj [ 3 ] ; real_T c03x2ztp0k [ 3 ] ; real_T
locht1wrtz [ 3 ] ; real_T pfnjwyno2i [ 3 ] ; real_T fdyhboqfup [ 3 ] ; real_T
jvhu50s4lq ; real_T m51e3edadb ; real_T c4jsp130wr [ 3 ] ; real_T lwea0wv2nm
[ 3 ] ; real_T dyrohorhkv [ 19 ] ; real_T pdaj1sts1i [ 5 ] ; real_T
ch5i5tukpt [ 3 ] ; real_T e1ls4ghjol ; real_T ofmdkk5v14 ; real_T bumwafdava
[ 3 ] ; real_T fzwq1sagca [ 3 ] ; real_T idpp4gxw4w [ 3 ] ; real_T krd2mdf5hw
[ 3 ] ; real_T obcqeyytus [ 3 ] ; real_T pq3sqkzdyp [ 3 ] ; real_T kdahe5jepd
[ 3 ] ; real_T bnnchphmjm [ 9 ] ; real_T fe3exdehyt [ 3 ] ; real_T juuxc4xwe2
; real_T ixlj03kzlb ; real_T ggrreaqpvc ; real_T ny3pdd4bet ; real_T
jakafxt501 ; real_T g2npli3rov ; real_T dcbcf1w1xd ; real_T ohckg0fxit ;
real_T ib0q5vztz3 ; real_T hh0dejejpw [ 3 ] ; real_T g2r0budphd [ 3 ] ;
real_T m3x35nddyc ; real_T nrg1pf5e1i [ 4 ] ; boolean_T ndmob3h5of ; }
n4i4q3iehdh ; typedef struct { AircraftForcesMomentsBus prddltbc3q ;
MassPropertiesBus pq1usl0ji3 ; real_T dv0zw01afq [ 100 ] ; real_T ko10qxkwp1
[ 100 ] ; real_T jqwrh133to [ 100 ] ; real_T n4cn0u2stm [ 100 ] ; real_T
gfshgcqqtd [ 100 ] ; real_T gfi4rwv5uv [ 3 ] ; real_T albizm10c2 [ 3 ] ;
real_T oqzh4go2fj [ 9 ] ; real_T f5cqf0v5ma [ 3 ] ; real_T egmzw14bcz [ 3 ] ;
real_T jer4mfbauo [ 3 ] ; real_T mofdx3lgfc [ 3 ] ; real_T pflzj3aecu [ 3 ] ;
real_T ehq4dn4qy5 ; real_T blxbp5visn ; real_T hpcdwfht1y [ 3 ] ; real_T
oh42dzxsr1 [ 3 ] ; real_T nn2xfon0gm [ 4 ] ; real_T gjsqq5ym1u [ 19 ] ;
real_T napxay3fym [ 5 ] ; real_T lgqfbgeul0 [ 3 ] ; real_T nwn2bda5du ;
real_T avmztxyufi ; real_T loacpgspe4 [ 3 ] ; real_T k4ixaewnry [ 3 ] ;
real_T impciqj2lu [ 3 ] ; real_T pubfzpbksv [ 3 ] ; real_T ocaarswrhs [ 3 ] ;
real_T crr154e30k [ 3 ] ; real_T dvwfvpxzzw [ 3 ] ; real_T hjgxipfal0 [ 9 ] ;
real_T lajfr2on4c [ 3 ] ; void * ocvltjlvxc ; boolean_T amcgr3jf34 ;
lj3yonrq1cm nfw1ojnku2 ; b3bxwyuagcq l4fx0iisxh ; pavvjelnfc2 cvp5v3n2te ; }
j0qkcyobx0j ; typedef struct { ar45jsrhtq jn4jwo3o0t ; bvs0nz5wek hihqsdko3c
; } hdulqcnnpvz ; typedef struct { fgdgb2ruqd jn4jwo3o0t ; affiwl1k5v
hihqsdko3c ; } p1wcsoehleg ; typedef struct { djcyjwlyeb jn4jwo3o0t ;
eknlil050d hihqsdko3c ; } o2ljf02bviv ; typedef struct { const real_T
kdcz4mkd1r ; const real_T khb4fs4fmn ; const real_T k1i30kmscj ; const real_T
e4nd2xo0f0 ; const real_T cudzna2k20 ; const real_T gxirh1fzki [ 3 ] ; const
real_T panwcg4pue [ 3 ] ; } lywj54ogohf ; struct ks3glix3e01_ {
AircraftForcesMomentsBus P_22 ; AircraftForcesMomentsBus P_23 ;
MassPropertiesBus P_24 ; MassPropertiesBus P_25 ; real_T P_26 ; real_T P_27 ;
real_T P_28 ; real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T P_32 ; real_T
P_33 ; real_T P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ; real_T P_38 [
2 ] ; real_T P_39 [ 5 ] ; real_T P_40 [ 2 ] ; real_T P_41 ; real_T P_42 [ 2 ]
; real_T P_43 ; real_T P_44 [ 2 ] ; real_T P_46 [ 2 ] ; real_T P_47 [ 15 ] ;
real_T P_48 [ 2 ] ; real_T P_49 [ 55 ] ; real_T P_50 ; real_T P_51 ; real_T
P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ;
real_T P_58 ; real_T P_59 ; real_T P_60 ; real_T P_61 ; real_T P_62 ; real_T
P_63 ; real_T P_64 ; real_T P_65 ; real_T P_66 ; boolean_T P_67 ; uint8_T
P_68 ; uint8_T P_69 ; uint8_T P_70 ; uint8_T P_71 ; uint8_T P_72 ; } ; struct
d2l3uzft5j { struct SimStruct_tag * * childSfunctions ; struct SimStruct_tag
* _mdlRefSfcnS ; struct { real_T mr_nonContSig0 [ 1 ] ; real_T mr_nonContSig1
[ 1 ] ; real_T mr_nonContSig2 [ 1 ] ; real_T mr_nonContSig3 [ 1 ] ; real_T
mr_nonContSig4 [ 1 ] ; real_T mr_nonContSig5 [ 3 ] ; real_T mr_nonContSig6 [
3 ] ; real_T mr_nonContSig7 [ 3 ] ; real_T mr_nonContSig8 [ 1 ] ; real_T
mr_nonContSig9 [ 3 ] ; real_T mr_nonContSig10 [ 3 ] ; } NonContDerivMemory ;
ssNonContDerivSigInfo nonContDerivSignal [ 11 ] ; SS_SimMode simMode ;
RTWSolverInfo solverInfo ; RTWSolverInfo * solverInfoPtr ; void * sfcnInfo ;
struct { time_T * taskTimePtrs [ 4 ] ; SimStruct childSFunctions [ 1 ] ;
SimStruct * childSFunctionPtrs [ 1 ] ; struct _ssBlkInfo2 blkInfo2 [ 1 ] ;
struct _ssSFcnModelMethods2 methods2 [ 1 ] ; struct _ssSFcnModelMethods3
methods3 [ 1 ] ; struct _ssSFcnModelMethods4 methods4 [ 1 ] ; struct
_ssStatesInfo2 statesInfo2 [ 1 ] ; ssPeriodicStatesInfo periodicStatesInfo [
1 ] ; struct _ssPortInfo2 inputOutputPortInfo2 [ 1 ] ; struct { time_T
sfcnPeriod [ 1 ] ; time_T sfcnOffset [ 1 ] ; int_T sfcnTsMap [ 1 ] ; struct
_ssPortInputs inputPortInfo [ 1 ] ; struct _ssInPortUnit inputPortUnits [ 1 ]
; struct _ssInPortCoSimAttribute inputPortCoSimAttribute [ 1 ] ; real_T const
* UPtrs0 [ 4 ] ; struct _ssPortOutputs outputPortInfo [ 2 ] ; struct
_ssOutPortUnit outputPortUnits [ 2 ] ; struct _ssOutPortCoSimAttribute
outputPortCoSimAttribute [ 2 ] ; uint_T attribs [ 6 ] ; mxArray * params [ 6
] ; struct _ssDWorkRecord dWork [ 4 ] ; struct _ssDWorkAuxRecord dWorkAux [ 4
] ; } Sfcn0 ; } NonInlinedSFcns ; boolean_T zCCacheNeedsReset ; boolean_T
derivCacheNeedsReset ; boolean_T CTOutputIncnstWithState ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; void * dataAddress [ 5 ] ; int32_T * vardimsAddress [ 5
] ; RTWLoggingFcnPtr loggingPtrs [ 5 ] ; rtwCAPI_ModelMappingInfo * childMMI
[ 3 ] ; sysRanDType * systemRan [ 4 ] ; int_T systemTid [ 4 ] ; } DataMapInfo
; struct { uint32_T options ; int_T numContStates ; int_T numU ; int_T numY ;
int_T numSampTimes ; int_T numBlocks ; int_T numBlockIO ; int_T numBlockPrms
; int_T numDwork ; int_T numSFcnPrms ; int_T numSFcns ; int_T numIports ;
int_T numOports ; int_T numNonSampZCs ; int_T sysDirFeedThru ; int_T
rtwGenSfcn ; } Sizes ; struct { time_T stepSize ; time_T tStart ; time_T
tFinal ; time_T timeOfLastOutput ; time_T * sampleTimes ; time_T *
offsetTimes ; int_T * sampleTimeTaskIDPtr ; int_T * sampleHits ; int_T *
perTaskSampleHits ; time_T sampleTimesArray [ 3 ] ; time_T offsetTimesArray [
3 ] ; int_T sampleTimeTaskIDArray [ 3 ] ; int_T sampleHitArray [ 3 ] ; int_T
perTaskSampleHitsArray [ 9 ] ; } Timing ; } ;
#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif
#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif
#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm) ((rtm)->DataMapInfo)
#endif
#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val) ((rtm)->DataMapInfo = (val))
#endif
#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif
#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm) ((rtm)->Timing.tFinal)
#endif
#ifndef rtmGetNonContDerivMemory
#define rtmGetNonContDerivMemory(rtm) ((rtm)->NonContDerivMemory)
#endif
#ifndef rtmSetNonContDerivMemory
#define rtmSetNonContDerivMemory(rtm, val) ((rtm)->NonContDerivMemory = (val))
#endif
#ifndef rtmGetSampleHitArray
#define rtmGetSampleHitArray(rtm) ((rtm)->Timing.sampleHitArray)
#endif
#ifndef rtmGetStepSize
#define rtmGetStepSize(rtm) ((rtm)->Timing.stepSize)
#endif
#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm) ((rtm)->zCCacheNeedsReset)
#endif
#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif
#ifndef rtmGet_TimeOfLastOutput
#define rtmGet_TimeOfLastOutput(rtm) ((rtm)->Timing.timeOfLastOutput)
#endif
#ifndef rtmGetClockTick0
#define rtmGetClockTick0(rtm) ssGetClockTick( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[0])
#endif
#ifndef rtmGetClockTick1
#define rtmGetClockTick1(rtm) ssGetClockTick( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[1])
#endif
#ifndef rtmGetClockTick2
#define rtmGetClockTick2(rtm) ssGetClockTick( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[2])
#endif
#ifndef rtmGetClockTick3
#define rtmGetClockTick3(rtm) ssGetClockTick( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[3])
#endif
#ifndef rtmGetClockTickH0
#define rtmGetClockTickH0(rtm) ssGetClockTickH( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[0])
#endif
#ifndef rtmGetClockTickH1
#define rtmGetClockTickH1(rtm) ssGetClockTickH( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[1])
#endif
#ifndef rtmGetClockTickH2
#define rtmGetClockTickH2(rtm) ssGetClockTickH( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[2])
#endif
#ifndef rtmGetClockTickH3
#define rtmGetClockTickH3(rtm) ssGetClockTickH( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[3])
#endif
#ifndef rtmGetLogOutput
#define rtmGetLogOutput(rtm) ssGetLogOutput((rtm)->_mdlRefSfcnS)
#endif
#ifndef rtmSetSampleHit
#define rtmSetSampleHit(rtm, sti, tid, val) _ssSetSampleHit(_ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti], val)
#endif
#ifndef rtmIsSampleHit
#define rtmIsSampleHit(rtm, sti, tid) ssIsSampleHit( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti], tid)
#endif
#ifndef rtmGetT
#define rtmGetT(rtm) (ssGetT((rtm)->_mdlRefSfcnS))
#endif
#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm) (ssGetTFinal((rtm)->_mdlRefSfcnS))
#endif
#ifndef rtmGetTNextWasAdjusted
#define rtmGetTNextWasAdjusted(rtm, sti) (ssGetTNextWasAdjusted((rtm)->_mdlRefSfcnS,  oga1twjs0f[sti]))
#endif
#ifndef rtmGetTStart
#define rtmGetTStart(rtm) (ssGetTStart((rtm)->_mdlRefSfcnS))
#endif
#ifndef rtmGetTaskTime
#define rtmGetTaskTime(rtm, sti) ssGetTaskTime( _ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti])
#endif
#ifndef rtmSetTaskTime
#define rtmSetTaskTime(rtm, sti, val) _ssSetTaskTime(_ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti], val)
#endif
#ifndef rtmGetTimeOfLastOutput
#define rtmGetTimeOfLastOutput(rtm) (ssGetTimeOfLastOutput((rtm)->_mdlRefSfcnS))
#endif
#ifndef rtmGetVarNextHitTime
#define rtmGetVarNextHitTime(rtm, sti) (_ssGetVarNextHitTime((rtm)->_mdlRefSfcnS,  (int)(ssGetOffsetTime(_ssGetRootSS((rtm)->_mdlRefSfcnS), oga1twjs0f[sti+3]))))
#endif
#ifndef rtmSetVarNextHitTime
#define rtmSetVarNextHitTime(rtm, sti, val) ssSetTimeOfNextVarHit((rtm)->_mdlRefSfcnS,  oga1twjs0f[sti], val)
#endif
typedef struct { hl0stsvgru rtm ; } owiw5mmjy2f ; extern int_T oga1twjs0f [ 4
] ; extern ks3glix3e01 djvd1z4mcz ; extern const lywj54ogohf mdaxxes2p3 ;
extern owiw5mmjy2f owiw5mmjy2 ; extern n4i4q3iehdh go0unc10h34 ; extern
j0qkcyobx0j gqrn4faltve ; extern void JSBSim_SFunction ( SimStruct * rts ) ;
#endif
