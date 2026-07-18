#if !defined(S_FUNCTION_NAME)
#define S_FUNCTION_NAME hexarotor_msf
#endif
#define S_FUNCTION_LEVEL 2
#if !defined(RTW_GENERATED_S_FUNCTION)
#define RTW_GENERATED_S_FUNCTION
#endif
#include <stdio.h>
#include <math.h>
#include "simstruc.h"
#include "fixedpoint.h"
#include "mwstringutil.h"
#define rt_logging_h
#include "hexarotor_types.h"
#include "hexarotor.h"
#include "hexarotor_private.h"
#include "hexAddFm.h"
#include "hexActuator.h"
#include "cgMassInertia.h"
#include "inertialDynamics.h"
#include "gravity.h"
#include "hexGroundContact.h"
#include "hexAero.h"
real_T rtP_C_D [ 19 ] ; real_T rtP_C_L [ 19 ] ; real_T rtP_actuatorDelay_s ;
real_T rtP_airDensityAtStd_kgpm3 ; real_T rtP_aircraftInitialBodyRates_radps
[ 3 ] ; real_T rtP_aircraftInitialEuler_rad [ 3 ] ; real_T
rtP_aircraftInitialPosInNED_m [ 3 ] ; real_T rtP_aircraftInitialVelInBody_mps
[ 3 ] ; real_T rtP_alphaBkpts_rad [ 19 ] ; real_T rtP_rpm2radps ; const char
* rt_GetMatSignalLoggingFileName ( void ) { return NULL ; } const char *
rt_GetMatSigLogSelectorFileName ( void ) { return NULL ; } void *
rt_GetOSigstreamManager ( void ) { return NULL ; } void * rt_slioCatalogue ( void ) { return NULL ; } void * rtwGetPointerFromUniquePtr ( void * uniquePtr ) { return NULL ; } void * CreateDiagnosticAsVoidPtr ( const char * id , int nargs , ... ) { void * voidPtrDiagnostic = NULL ; va_list args ; va_start ( args , nargs ) ; slmrCreateDiagnostic ( id , nargs , args , & voidPtrDiagnostic ) ; va_end ( args ) ; return voidPtrDiagnostic ; } void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * simStrcut = ( SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( simStrcut ) ) { _ssSet_slLocalErrMsg ( simStrcut , diag ) ; } else { _ssDiscardDiagnostic ( simStrcut , diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) { _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) { _ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } const char * rt_CreateFullPathToTop ( const char * toppath , const char * subpath ) { char * fullpath = NULL ; slmrCreateFullPathToTop ( toppath , subpath , & fullpath ) ; return fullpath ; } boolean_T slIsRapidAcceleratorSimulating ( void ) { return false ; } void rt_RAccelReplaceFromFilename ( SimStruct * S , const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void rt_RAccelReplaceToFilename ( SimStruct * S , const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void * slsa_malloc ( size_t s ) { return malloc ( s ) ; } void slsa_free ( void * ptr ) { free ( ptr ) ; }
#define MDL_PROCESS_PARAMETERS
#if defined(MATLAB_MEX_FILE)
static void mdlProcessParameters ( SimStruct * S ) { real_T * GlobalPrm_0 = ( real_T * ) NULL ; real_T * GlobalPrm_1 = ( real_T * ) NULL ; real_T * GlobalPrm_2 = ( real_T * ) NULL ; real_T * GlobalPrm_3 = ( real_T * ) NULL ; real_T * GlobalPrm_4 = ( real_T * ) NULL ; real_T * GlobalPrm_5 = ( real_T * ) NULL ; real_T * GlobalPrm_6 = ( real_T * ) NULL ; real_T * GlobalPrm_7 = ( real_T * ) NULL ; real_T * GlobalPrm_8 = ( real_T * ) NULL ; real_T * GlobalPrm_9 = ( real_T * ) NULL ; if ( ! ssGetModelRefGlobalParamData ( S , 0 , ( void * * ) ( & GlobalPrm_0 ) ) ) return ; if ( GlobalPrm_0 != NULL ) { ( void ) memcpy ( & ( rtP_C_D [ 0 ] ) , GlobalPrm_0 , sizeof ( real_T ) * 19 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 1 , ( void * * ) ( & GlobalPrm_1 ) ) ) return ; if ( GlobalPrm_1 != NULL ) { ( void ) memcpy ( & ( rtP_C_L [ 0 ] ) , GlobalPrm_1 , sizeof ( real_T ) * 19 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 2 , ( void * * ) ( & GlobalPrm_2 ) ) ) return ; if ( GlobalPrm_2 != NULL ) { ( void ) memcpy ( & ( rtP_actuatorDelay_s ) , GlobalPrm_2 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 3 , ( void * * ) ( & GlobalPrm_3 ) ) ) return ; if ( GlobalPrm_3 != NULL ) { ( void ) memcpy ( & ( rtP_airDensityAtStd_kgpm3 ) , GlobalPrm_3 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 4 , ( void * * ) ( & GlobalPrm_4 ) ) ) return ; if ( GlobalPrm_4 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialBodyRates_radps [ 0 ] ) , GlobalPrm_4 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 5 , ( void * * ) ( & GlobalPrm_5 ) ) ) return ; if ( GlobalPrm_5 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialEuler_rad [ 0 ] ) , GlobalPrm_5 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 6 , ( void * * ) ( & GlobalPrm_6 ) ) ) return ; if ( GlobalPrm_6 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialPosInNED_m [ 0 ] ) , GlobalPrm_6 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 7 , ( void * * ) ( & GlobalPrm_7 ) ) ) return ; if ( GlobalPrm_7 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialVelInBody_mps [ 0 ] ) , GlobalPrm_7 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 8 , ( void * * ) ( & GlobalPrm_8 ) ) ) return ; if ( GlobalPrm_8 != NULL ) { ( void ) memcpy ( & ( rtP_alphaBkpts_rad [ 0 ] ) , GlobalPrm_8 , sizeof ( real_T ) * 19 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 9 , ( void * * ) ( & GlobalPrm_9 ) ) ) return ; if ( GlobalPrm_9 != NULL ) { ( void ) memcpy ( & ( rtP_rpm2radps ) , GlobalPrm_9 , sizeof ( real_T ) ) ; } }
#endif
static void mdlInitializeConditions ( SimStruct * S ) { et34rxtuyzb * dw = ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_1_33 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; real_T * o_o_o_B_1_34 = ( real_T * ) ssGetOutputPortSignal ( S , 11 ) ; real_T * o_o_o_o_B_1_35 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; plypikssk0 * localX = ( plypikssk0 * ) ssGetContStates ( S ) ; bc02h231hx ( o_o_o_o_B_1_33 , o_o_o_B_1_34 , o_o_o_o_B_1_35 , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX ) ; } static void mdlReset ( SimStruct * S ) { et34rxtuyzb * dw = ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; plypikssk0 * localX = ( plypikssk0 * ) ssGetContStates ( S ) ; kp54z2ru0m ( & ( dw -> rtdw ) , localX ) ; } static void mdlPeriodicOutputUpdate ( SimStruct * S , int_T tid ) { et34rxtuyzb * dw = ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; real_T const * i_kylr35diri = ( real_T * ) ssGetInputPortSignal ( S , 0 ) ; real_T const * i_oyh1bw2a0y = ( real_T * ) ssGetInputPortSignal ( S , 1 ) ; real_T const * i_hmmw02ig3c = ( real_T * ) ssGetInputPortSignal ( S , 2 ) ; real_T const * i_mtx1ekpvnf = ( real_T * ) ssGetInputPortSignal ( S , 3 ) ; real_T const * i_dlqtv5gz53 = ( real_T * ) ssGetInputPortSignal ( S , 4 ) ; real_T const * i_kmramgh1vg = ( real_T * ) ssGetInputPortSignal ( S , 5 ) ; real_T const * i_bhsfnw4zam = ( real_T * ) ssGetInputPortSignal ( S , 6 ) ; real_T const * i_ibyqpdg44o = ( real_T * ) ssGetInputPortSignal ( S , 7 ) ; real_T const * i_md3nushizo = ( real_T * ) ssGetInputPortSignal ( S , 8 ) ; real_T const * i_bcrlo4izwf = ( real_T * ) ssGetInputPortSignal ( S , 13 ) ; real_T const * i_mdswxopisg = ( real_T * ) ssGetInputPortSignal ( S , 14 ) ; real_T const * i_omz3rxkx5q = ( real_T * ) ssGetInputPortSignal ( S , 19 ) ; boolean_T const * i_eitq5kjv1w = ( boolean_T * ) ssGetInputPortSignal ( S , 20 ) ; boolean_T const * i_g3w4sjh1l0 = ( boolean_T * ) ssGetInputPortSignal ( S , 21 ) ; boolean_T const * i_g1jjd1rew4 = ( boolean_T * ) ssGetInputPortSignal ( S , 22 ) ; boolean_T const * i_mdvaboxr5x = ( boolean_T * ) ssGetInputPortSignal ( S , 23 ) ; boolean_T const * i_h4cpjev0d5 = ( boolean_T * ) ssGetInputPortSignal ( S , 24 ) ; boolean_T const * i_fyjm32bdy3 = ( boolean_T * ) ssGetInputPortSignal ( S , 25 ) ; boolean_T * o_B_1_1 = ( boolean_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_1_2 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_1_3 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_1_4 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_B_1_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_B_1_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_B_1_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_B_1_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; real_T * o_B_1_9 = ( real_T * ) ssGetOutputPortSignal ( S , 8 ) ; real_T * o_B_1_10 = ( real_T * ) ssGetOutputPortSignal ( S , 9 ) ; real_T * o_o_o_o_B_1_33 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; real_T * o_o_o_o_B_1_35 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; real_T * o_B_1_11 = ( real_T * ) ssGetOutputPortSignal ( S , 14 ) ; real_T * o_B_1_12 = ( real_T * ) ssGetOutputPortSignal ( S , 15 ) ; real_T * o_B_1_13 = ( real_T * ) ssGetOutputPortSignal ( S , 16 ) ; real_T * o_B_1_14 = ( real_T * ) ssGetOutputPortSignal ( S , 17 ) ; real_T * o_B_1_15 = ( real_T * ) ssGetOutputPortSignal ( S , 18 ) ; real_T * o_B_1_16 = ( real_T * ) ssGetOutputPortSignal ( S , 19 ) ; real_T * o_B_1_17 = ( real_T * ) ssGetOutputPortSignal ( S , 20 ) ; real_T * o_B_1_18 = ( real_T * ) ssGetOutputPortSignal ( S , 21 ) ; real_T * o_B_1_19 = ( real_T * ) ssGetOutputPortSignal ( S , 22 ) ; real_T * o_B_1_20 = ( real_T * ) ssGetOutputPortSignal ( S , 23 ) ; real_T * o_o_B_1_21 = ( real_T * ) ssGetOutputPortSignal ( S , 24 ) ; real_T * o_o_B_1_22 = ( real_T * ) ssGetOutputPortSignal ( S , 25 ) ; real_T * o_o_B_1_23 = ( real_T * ) ssGetOutputPortSignal ( S , 26 ) ; real_T * o_B_1_24 = ( real_T * ) ssGetOutputPortSignal ( S , 27 ) ; real_T * o_B_1_25 = ( real_T * ) ssGetOutputPortSignal ( S , 28 ) ; real_T * o_B_1_26 = ( real_T * ) ssGetOutputPortSignal ( S , 29 ) ; real_T * o_B_1_27 = ( real_T * ) ssGetOutputPortSignal ( S , 30 ) ; real_T * o_B_1_28 = ( real_T * ) ssGetOutputPortSignal ( S , 31 ) ; real_T * o_B_1_29 = ( real_T * ) ssGetOutputPortSignal ( S , 32 ) ; real_T * o_B_1_30 = ( real_T * ) ssGetOutputPortSignal ( S , 33 ) ; real_T * o_o_B_1_31 = ( real_T * ) ssGetOutputPortSignal ( S , 34 ) ; real_T * o_o_B_1_32 = ( real_T * ) ssGetOutputPortSignal ( S , 35 ) ; plypikssk0 * localX = ( plypikssk0 * ) ssGetContStates ( S ) ; if ( tid == 0 ) { hexarotor ( & ( dw -> rtm ) , i_kylr35diri , i_oyh1bw2a0y , i_hmmw02ig3c , i_mtx1ekpvnf , i_dlqtv5gz53 , i_kmramgh1vg , i_bhsfnw4zam , i_ibyqpdg44o , i_md3nushizo , i_bcrlo4izwf , i_mdswxopisg , i_omz3rxkx5q , i_eitq5kjv1w , i_g3w4sjh1l0 , i_g1jjd1rew4 , i_mdvaboxr5x , i_h4cpjev0d5 , i_fyjm32bdy3 , o_B_1_1 , o_B_1_2 , o_B_1_3 , o_B_1_4 , o_B_1_5 , o_B_1_6 , o_o_B_1_7 , o_o_B_1_8 , o_B_1_9 , o_B_1_10 , o_o_o_o_B_1_33 , o_o_o_o_B_1_35 , o_B_1_11 , o_B_1_12 , o_B_1_13 , o_B_1_14 , o_B_1_15 , o_B_1_16 , o_B_1_17 , o_B_1_18 , o_B_1_19 , o_B_1_20 , o_o_B_1_21 , o_o_B_1_22 , o_o_B_1_23 , o_B_1_24 , o_B_1_25 , o_B_1_26 , o_B_1_27 , o_B_1_28 , o_B_1_29 , o_B_1_30 , o_o_B_1_31 , o_o_B_1_32 , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX ) ; o22h4v45uk ( & ( dw -> rtm ) , o_o_B_1_21 , o_o_B_1_22 , o_o_B_1_23 , o_o_B_1_31 , o_o_B_1_32 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; } } static void mdlInitializeSizes ( SimStruct * S ) { if ( ( S -> mdlInfo -> genericFcn != ( NULL ) ) && ( ! ( S -> mdlInfo -> genericFcn ) ( S , GEN_FCN_CHK_MODELREF_SFUN_HAS_MODEL_BLOCK , - 1 , ( NULL ) ) ) ) { return ; } ssSetNumSFcnParams ( S , 0 ) ; ssFxpSetU32BitRegionCompliant ( S , 1 ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; } ssSetRTWGeneratedSFcn ( S , 2 ) ; ssSetNumContStates ( S , 25 ) ; ssSetNumDiscStates ( S , 0 ) ; ssSetNumPeriodicContStates ( S , 0 ) ; ssSetSymbolicDimsSupport ( S , true ) ; slmrInitializeIOPortDataVectors ( S , 26 , 36 ) ; if ( ! ssSetNumInputPorts ( S , 26 ) ) return ; if ( ! ssSetInputPortVectorDimension ( S , 0 , 1 ) ) return ; ssSetInputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ; ssSetInputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 0 , SS_DOUBLE ) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 0 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 0 , 1 ) ; ssSetInputPortOptimOpts ( S
, 0 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 0 ,
false ) ; ssSetInputPortSampleTime ( S , 0 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 0 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 1 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 1 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 1 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 1 , 1 ) ; ssSetInputPortOptimOpts ( S
, 1 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 1 ,
false ) ; ssSetInputPortSampleTime ( S , 1 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 1 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 2 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 2 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 2 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 2 , 1 ) ; ssSetInputPortOptimOpts ( S
, 2 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 2 ,
false ) ; ssSetInputPortSampleTime ( S , 2 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 2 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 3 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 3 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 3 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 3 , 1 ) ; ssSetInputPortOptimOpts ( S
, 3 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 3 ,
false ) ; ssSetInputPortSampleTime ( S , 3 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 3 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 4 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 4 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 4 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 4 , 1 ) ; ssSetInputPortOptimOpts ( S
, 4 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 4 ,
false ) ; ssSetInputPortSampleTime ( S , 4 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 4 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 5 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 5 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 5 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 5 , 1 ) ; ssSetInputPortOptimOpts ( S
, 5 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 5 ,
false ) ; ssSetInputPortSampleTime ( S , 5 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 5 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 6 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 6 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 6 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 6 , 1 ) ; ssSetInputPortOptimOpts ( S
, 6 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 6 ,
false ) ; ssSetInputPortSampleTime ( S , 6 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 6 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 7 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 7 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 7 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 7 , 1 ) ; ssSetInputPortOptimOpts ( S
, 7 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 7 ,
false ) ; ssSetInputPortSampleTime ( S , 7 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 7 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 8 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 8 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 8 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 8 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 8 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 8 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 8 , 1 ) ; ssSetInputPortOptimOpts ( S
, 8 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 8 ,
false ) ; ssSetInputPortSampleTime ( S , 8 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 8 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 9 , 3 ) ) return
; ssSetInputPortDimensionsMode ( S , 9 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 9 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 9 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 9 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 9 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 9 , 1 ) ; ssSetInputPortOptimOpts ( S
, 9 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 9 ,
false ) ; ssSetInputPortSampleTime ( S , 9 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 9 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 10 , 3 ) )
return ; ssSetInputPortDimensionsMode ( S , 10 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 10 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 10 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 10 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 10 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 10 , 1 ) ; ssSetInputPortOptimOpts ( S
, 10 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 10 ,
false ) ; ssSetInputPortSampleTime ( S , 10 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 10 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 11 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 11 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 11 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 11 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 11 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 11 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 11 , 1 ) ; ssSetInputPortOptimOpts ( S
, 11 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 11 ,
false ) ; ssSetInputPortSampleTime ( S , 11 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 11 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 12 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 12 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 12 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 12 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 12 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 12 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 12 , 1 ) ; ssSetInputPortOptimOpts ( S
, 12 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 12 ,
false ) ; ssSetInputPortSampleTime ( S , 12 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 12 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 13 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 13 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 13 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 13 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 13 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 13 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 13 , 1 ) ; ssSetInputPortOptimOpts ( S
, 13 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 13 ,
false ) ; ssSetInputPortSampleTime ( S , 13 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 13 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 14 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 14 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 14 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 14 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 14 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 14 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 14 , 1 ) ; ssSetInputPortOptimOpts ( S
, 14 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 14 ,
false ) ; ssSetInputPortSampleTime ( S , 14 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 14 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 15 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 15 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 15 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 15 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 15 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 15 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 15 , 1 ) ; ssSetInputPortOptimOpts ( S
, 15 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 15 ,
false ) ; ssSetInputPortSampleTime ( S , 15 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 15 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 16 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 16 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 16 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 16 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 16 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 16 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 16 , 1 ) ; ssSetInputPortOptimOpts ( S
, 16 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 16 ,
false ) ; ssSetInputPortSampleTime ( S , 16 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 16 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 17 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 17 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 17 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 17 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 17 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 17 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 17 , 1 ) ; ssSetInputPortOptimOpts ( S
, 17 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 17 ,
false ) ; ssSetInputPortSampleTime ( S , 17 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 17 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 18 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 18 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 18 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 18 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 18 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 18 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 18 , 1 ) ; ssSetInputPortOptimOpts ( S
, 18 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 18 ,
false ) ; ssSetInputPortSampleTime ( S , 18 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 18 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 19 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 19 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 19 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 19 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 19 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 19 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 19 , 1 ) ; ssSetInputPortOptimOpts ( S
, 19 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 19 ,
false ) ; ssSetInputPortSampleTime ( S , 19 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 19 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 20 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 20 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 20 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 20 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 20 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 20 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 20 , 1 ) ; ssSetInputPortOptimOpts ( S
, 20 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 20 ,
false ) ; ssSetInputPortSampleTime ( S , 20 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 20 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 21 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 21 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 21 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 21 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 21 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 21 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 21 , 1 ) ; ssSetInputPortOptimOpts ( S
, 21 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 21 ,
false ) ; ssSetInputPortSampleTime ( S , 21 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 21 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 22 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 22 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 22 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 22 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 22 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 22 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 22 , 1 ) ; ssSetInputPortOptimOpts ( S
, 22 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 22 ,
false ) ; ssSetInputPortSampleTime ( S , 22 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 22 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 23 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 23 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 23 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 23 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 23 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 23 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 23 , 1 ) ; ssSetInputPortOptimOpts ( S
, 23 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 23 ,
false ) ; ssSetInputPortSampleTime ( S , 23 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 23 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 24 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 24 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 24 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 24 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 24 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 24 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 24 , 1 ) ; ssSetInputPortOptimOpts ( S
, 24 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 24 ,
false ) ; ssSetInputPortSampleTime ( S , 24 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 24 , 0.0 ) ; if ( !
ssSetInputPortVectorDimension ( S , 25 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 25 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 25 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 25 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 25 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 25 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 25 , 1 ) ; ssSetInputPortOptimOpts ( S
, 25 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 25 ,
false ) ; ssSetInputPortSampleTime ( S , 25 , 0.0 ) ;
ssSetInputPortOffsetTime ( S , 25 , 0.0 ) ; if ( ! ssSetNumOutputPorts ( S ,
36 ) ) return ; if ( ! ssSetOutputPortVectorDimension ( S , 0 , 1 ) ) return
; ssSetOutputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 0 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 0 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 0 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 0 , 1 ) ;
ssSetOutputPortOkToMerge ( S , 0 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 0 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 0 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 1 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 1 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 1 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
1 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 1 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 1 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 1 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 1 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 2 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 2 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 2 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
2 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 2 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 2 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 2 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 2 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 3 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 3 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 3 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
3 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 3 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 3 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 3 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 3 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 4 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 4 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 4 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
4 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 4 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 4 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 4 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 4 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 5 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 5 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 5 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
5 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 5 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 5 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 5 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 5 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 6 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 6 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 6 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
6 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 6 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 6 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 6 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 6 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 7 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 7 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 7 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
7 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 7 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 7 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 7 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 7 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortMatrixDimensions ( S , 8 , 3 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 8 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 8 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 8 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 8 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 8 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
8 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 8 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 8 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 8 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 8 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 9 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 9 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 9 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 9 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 9 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 9 , 0.0 ) ; ssSetOutputPortOffsetTime ( S ,
9 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 9 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 9 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 9 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 9 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 10 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 10 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 10 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 10 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 10 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 10 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 10 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 10 , 0 ) ; ssSetOutputPortOkToMerge
( S , 10 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 10 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 10 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 11
, 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 11 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 11 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 11 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 11 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 11 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 11 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 11 , 0 ) ; ssSetOutputPortOkToMerge
( S , 11 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 11 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 11 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortMatrixDimensions ( S ,
12 , 3 , 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 12 ,
FIXED_DIMS_MODE ) ; ssSetOutputPortFrameData ( S , 12 , FRAME_NO ) ; if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 12 , SS_DOUBLE ) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 12 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 12 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 12 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 12 , 0 ) ; ssSetOutputPortOkToMerge
( S , 12 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 12 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 12 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 13
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 13 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 13 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 13 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 13 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 13 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 13 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 13 , 0 ) ; ssSetOutputPortOkToMerge
( S , 13 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 13 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 13 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 14
, 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 14 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 14 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 14 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 14 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 14 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 14 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 14 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 14 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 14 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 14 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 15 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 15 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 15 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 15 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 15 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 15 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 15 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 15 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 15 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 15 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 15 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 16 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 16 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 16 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 16 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 16 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 16 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 16 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 16 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 16 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 16 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 16 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 17 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 17 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 17 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 17 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 17 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 17 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 17 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 17 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 17 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 17 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 17 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 18 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 18 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 18 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 18 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 18 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 18 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 18 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 18 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 18 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 18 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 18 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 19 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 19 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 19 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 19 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 19 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 19 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 19 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 19 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 19 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 19 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 19 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 20 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 20 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 20 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 20 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 20 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 20 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 20 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 20 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 20 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 20 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 20 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 21 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 21 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 21 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 21 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 21 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 21 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 21 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 21 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 21 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 21 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 21 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 22 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 22 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 22 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 22 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 22 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 22 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 22 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 22 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 22 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 22 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 22 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 23 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 23 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 23 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 23 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 23 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 23 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 23 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 23 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 23 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 23 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 23 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 24 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 24 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 24 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 24 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 24 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 24 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 24 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 24 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 24 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 24 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 24 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 25 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 25 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 25 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 25 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 25 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 25 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 25 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 25 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 25 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 25 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 25 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 26 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 26 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 26 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 26 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 26 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 26 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 26 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 26 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 26 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 26 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 26 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 27 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 27 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 27 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 27 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 27 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 27 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 27 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 27 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 27 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 27 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 27 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 28 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 28 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 28 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 28 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 28 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 28 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 28 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 28 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 28 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 28 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 28 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 29 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 29 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 29 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 29 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 29 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 29 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 29 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 29 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 29 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 29 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 29 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 30 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 30 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 30 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 30 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 30 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 30 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 30 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 30 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 30 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 30 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 30 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 31 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 31 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 31 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 31 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 31 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 31 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 31 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 31 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 31 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 31 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 31 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 32 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 32 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 32 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 32 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 32 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 32 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 32 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 32 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 32 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 32 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 32 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 33 , 6 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 33 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 33 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 33 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 33 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 33 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 33 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 33 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 33 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 33 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 33 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 34 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 34 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 34 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 34 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 34 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 34 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 34 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 34 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 34 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 34 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 34 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 35 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 35 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 35 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 35 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 35 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 35 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 35 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 35 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 35 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 35 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 35 , SS_NOT_REUSABLE_AND_GLOBAL ) ;
ssSetSimStateCompliance ( S , USE_CUSTOM_SIM_STATE ) ;
mr_hexarotor_RegisterSimStateChecksum ( S ) ; ssSetNumSampleTimes ( S ,
PORT_BASED_SAMPLE_TIMES ) ; ssSetParameterTuningCompliance ( S , true ) ;
ssSetNumRWork ( S , 0 ) ; ssSetNumIWork ( S , 0 ) ; ssSetNumPWork ( S , 0 ) ;
ssSetNumModes ( S , 0 ) ; { int_T zcsIdx = 0 ; }
ssSetOutputPortIsNonContinuous ( S , 0 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 0 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 1 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 1 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 2 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 2 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 3 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 3 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 4 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 4 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 5 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 5 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 6 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 6 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 7 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 7 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 8 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 8 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 9 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 9 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 10 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 10 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 11 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 11 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 12 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 12 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 13 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 13 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 14 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 14 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 15 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 15 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 16 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 16 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 17 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 17 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 18 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 18 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 19 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 19 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 20 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 20 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 21 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 21 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 22 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 22 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 23 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 23 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 24 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 24 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 25 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 25 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 26 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 26 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 27 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 27 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 28 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 28 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 29 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 29 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 30 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 30 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 31 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 31 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 32 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 32 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 33 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 33 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 34 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 34 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 35 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 35 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 0 , 0 ) ; ssSetInputPortIsNotDerivPort ( S
, 1 , 0 ) ; ssSetInputPortIsNotDerivPort ( S , 2 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 3 , 0 ) ; ssSetInputPortIsNotDerivPort ( S
, 4 , 0 ) ; ssSetInputPortIsNotDerivPort ( S , 5 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 6 , 0 ) ; ssSetInputPortIsNotDerivPort ( S
, 7 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 8 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 9 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 10 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 11 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 12 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 13 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 14 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 15 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 16 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 17 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 18 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 19 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 20 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 21 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 22 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 23 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 24 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 25 , 1 ) ; ssSetModelReferenceSampleTimeInheritanceRule ( S ,
DISALLOW_SAMPLE_TIME_INHERITANCE ) ; ssSetAcceptsFcnCallInputs ( S ) ;
ssSetModelReferenceNormalModeSupport ( S ,
MDL_START_AND_MDL_PROCESS_PARAMS_OK ) ; ssSupportsMultipleExecInstances ( S ,
false ) ; ssRegisterMsgForNotSupportingMultiExecInst ( S ,
 "<diag_root><diag id=\"Simulink:blocks:LoggingBlockDoesNotSupportMultiExecInstancesWithCustomMsg\" pr=\"d\"><arguments><arg type=\"encoded\">aABlAHgAYQByAG8AdABvAHIALwBGAHIAbwBtADEAMwAAAA==</arg><arg type=\"encoded\">PABfAF8AaQBpAFMAUwBfAF8APgA8AC8AXwBfAGkAaQBTAFMAXwBfAD4AAAA=</arg><arg type=\"encoded\">PABfAF8AaQB0AGUAcgBCAGwAawBfAF8APgA8AC8AXwBfAGkAdABlAHIAQgBsAGsAXwBfAD4AAAA=</arg></arguments><hs><h>AAAAAYBn4EA=</h></hs><causes><diag id=\"Simulink:logLoadBlocks:SigLogMultiExecInsNoBusSupportMsg\" pr=\"d\"><msg encoded=\"yes\">TwBuAGwAeQAgAG4AbwBuAC0AYgB1AHMAIABzAGkAZwBuAGEAbABzACAAYwBhAG4AIABiAGUAIABsAG8AZwBnAGUAZAAgAGkAbgBzAGkAZABlACAARgBvAHIAIABFAGEAYwBoACAAcwB1AGIAcwB5AHMAdABlAG0AcwAuAAAA</msg></diag></causes></diag></diag_root>"
) ; ssHasStateInsideForEachSS ( S , true ) ; ssSetOptions ( S ,
SS_OPTION_ALLOW_CONSTANT_PORT_SAMPLE_TIME |
SS_OPTION_SUPPORTS_ALIAS_DATA_TYPES | SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME
| SS_OPTION_EXCEPTION_FREE_CODE | SS_OPTION_WORKS_WITH_CODE_REUSE ) ;
#if SS_SFCN_FOR_SIM
if ( S -> mdlInfo -> genericFcn != ( NULL ) && ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { int_T retVal = 1 ; mr_hexarotor_MdlInfoRegFcn
( S , "hexarotor" , & retVal ) ; if ( ! retVal ) return ; }
#endif
#if SS_SFCN_FOR_SIM
if ( ssSetNumDWork ( S , 1 ) ) { int mdlrefDWTypeId ; ssRegMdlRefDWorkType ( S
, & mdlrefDWTypeId ) ; if ( mdlrefDWTypeId == INVALID_DTYPE_ID ) return ; if
( ! ssSetDataTypeSize ( S , mdlrefDWTypeId , sizeof ( et34rxtuyzb ) ) )
return ; ssSetDWorkDataType ( S , 0 , mdlrefDWTypeId ) ; ssSetDWorkWidth ( S
, 0 , 1 ) ; }
#else
if ( ! ssSetNumDWork ( S , 1 ) ) { return ; }
#endif
slmrRegisterSystemInitializeMethod ( S , mdlInitializeConditions ) ;
slmrRegisterSystemResetMethod ( S , mdlReset ) ;
slmrRegisterPeriodicOutputUpdateMethod ( S , mdlPeriodicOutputUpdate ) ;
ssSetSimulinkVersionGeneratedIn ( S , "26.1" ) ; ssSetNeedAbsoluteTime ( S ,
1 ) ; } static void mdlInitializeSampleTimes ( SimStruct * S ) { return ; }
#define MDL_SET_WORK_WIDTHS
static void mdlSetWorkWidths ( SimStruct * S ) { if ( S -> mdlInfo ->
genericFcn != ( NULL ) ) { ssSetSignalSizesComputeType ( S ,
SS_VARIABLE_SIZE_FROM_INPUT_VALUE_AND_SIZE ) ; } { static const char *
toFileNames [ ] = { "" } ; static const char * fromFileNames [ ] = { "" } ;
if ( ! ssSetModelRefFromFiles ( S , 0 , fromFileNames ) ) return ; if ( !
ssSetModelRefToFiles ( S , 0 , toFileNames ) ) return ; } }
#define MDL_SETUP_RUNTIME_RESOURCES
static void mdlSetupRuntimeResources ( SimStruct * S ) { et34rxtuyzb * dw = ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_1_33 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; real_T * o_o_o_B_1_34 = ( real_T * ) ssGetOutputPortSignal ( S , 11 ) ; real_T * o_o_o_o_B_1_35 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; plypikssk0 * localX = ( plypikssk0 * ) ssGetContStates ( S ) ; ssNonContDerivSigFeedingOutports mr_nonContOutput1 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput2 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput3 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput4 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput5 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput6 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput7 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput8 [ 12 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput9 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput14 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput15 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput16 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput17 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput18 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput19 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput20 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput21 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput22 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput23 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput27 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput28 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput29 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput30 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput31 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput32 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput33 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput34 [ 8 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput35 [ 8 ] ; ssNonContDerivSigFeedingOutports * mr_nonContOutputArray [ 36 ] ; void * sysRanPtr = ( NULL ) ; int_T contextTid = 0 ; ssGetContextSysRanBCPtr ( S , & sysRanPtr ) ; ssGetContextSysTid ( S , & contextTid ) ; if ( contextTid == CONSTANT_TID ) { contextTid = 0 ; } mr_nonContOutputArray [ 0 ] = ( NULL ) ; mr_nonContOutputArray [ 1 ] = mr_nonContOutput1 ; mr_nonContOutputArray [ 2 ] = mr_nonContOutput2 ; mr_nonContOutputArray [ 3 ] = mr_nonContOutput3 ; mr_nonContOutputArray [ 4 ] = mr_nonContOutput4 ; mr_nonContOutputArray [ 5 ] = mr_nonContOutput5 ; mr_nonContOutputArray [ 6 ] = mr_nonContOutput6 ; mr_nonContOutputArray [ 7 ] = mr_nonContOutput7 ; mr_nonContOutputArray [ 8 ] = mr_nonContOutput8 ; mr_nonContOutputArray [ 9 ] = mr_nonContOutput9 ; mr_nonContOutputArray [ 10 ] = ( NULL ) ; mr_nonContOutputArray [ 11 ] = ( NULL ) ; mr_nonContOutputArray [ 12 ] = ( NULL ) ; mr_nonContOutputArray [ 13 ] = ( NULL ) ; mr_nonContOutputArray [ 14 ] = mr_nonContOutput14 ; mr_nonContOutputArray [ 15 ] = mr_nonContOutput15 ; mr_nonContOutputArray [ 16 ] = mr_nonContOutput16 ; mr_nonContOutputArray [ 17 ] = mr_nonContOutput17 ; mr_nonContOutputArray [ 18 ] = mr_nonContOutput18 ; mr_nonContOutputArray [ 19 ] = mr_nonContOutput19 ; mr_nonContOutputArray [ 20 ] = mr_nonContOutput20 ; mr_nonContOutputArray [ 21 ] = mr_nonContOutput21 ; mr_nonContOutputArray [ 22 ] = mr_nonContOutput22 ; mr_nonContOutputArray [ 23 ] = mr_nonContOutput23 ; mr_nonContOutputArray [ 24 ] = ( NULL ) ; mr_nonContOutputArray [ 25 ] = ( NULL ) ; mr_nonContOutputArray [ 26 ] = ( NULL ) ; mr_nonContOutputArray [ 27 ] = mr_nonContOutput27 ; mr_nonContOutputArray [ 28 ] = mr_nonContOutput28 ; mr_nonContOutputArray [ 29 ] = mr_nonContOutput29 ; mr_nonContOutputArray [ 30 ] = mr_nonContOutput30 ; mr_nonContOutputArray [ 31 ] = mr_nonContOutput31 ; mr_nonContOutputArray [ 32 ] = mr_nonContOutput32 ; mr_nonContOutputArray [ 33 ] = mr_nonContOutput33 ; mr_nonContOutputArray [ 34 ] = mr_nonContOutput34 ; mr_nonContOutputArray [ 35 ] = mr_nonContOutput35 ; simTgtAllocOPModelData ( S ) ; axfdkowicg ( o_o_o_o_B_1_33 , o_o_o_B_1_34 , o_o_o_o_B_1_35 , S , mr_nonContOutputArray , slmrGetTopTidFromMdlRefChildTid ( S , 0 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 1 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 2 , true ) , & ( dw -> rtm ) , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX , sysRanPtr , contextTid , ( ( NULL ) ) , ( ( NULL ) ) , 0 , - 1 ) ; ssSetModelMappingInfoPtr ( S , & ( dw -> rtm . DataMapInfo . mmi ) ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 1 , mr_nonContOutput1 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 2 , mr_nonContOutput2 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 3 , mr_nonContOutput3 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 4 , mr_nonContOutput4 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 5 , mr_nonContOutput5 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 6 , mr_nonContOutput6 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 7 , mr_nonContOutput7 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 8 , mr_nonContOutput8 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 9 , mr_nonContOutput9 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 14 , mr_nonContOutput14 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 15 , mr_nonContOutput15 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 16 , mr_nonContOutput16 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 17 , mr_nonContOutput17 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 18 , mr_nonContOutput18 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 19 , mr_nonContOutput19 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 20 , mr_nonContOutput20 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 21 , mr_nonContOutput21 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 22 , mr_nonContOutput22 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 23 , mr_nonContOutput23 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 27 , mr_nonContOutput27 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 28 , mr_nonContOutput28 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 29 , mr_nonContOutput29 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 30 , mr_nonContOutput30 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 31 , mr_nonContOutput31 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 32 , mr_nonContOutput32 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 33 , mr_nonContOutput33 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 34 , mr_nonContOutput34 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 35 , mr_nonContOutput35 ) ) return ; } d4tvbollve ( & ( dw -> rtm ) , & ( dw -> rtdw ) ) ; }
#define MDL_START
static void mdlStart ( SimStruct * S ) { et34rxtuyzb * dw = ( et34rxtuyzb * )
ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_1_33 = ( real_T * )
ssGetOutputPortSignal ( S , 10 ) ; real_T * o_o_o_B_1_34 = ( real_T * )
ssGetOutputPortSignal ( S , 11 ) ; real_T * o_o_o_o_B_1_35 = ( real_T * )
ssGetOutputPortSignal ( S , 12 ) ; mdlProcessParameters ( S ) ; di1idyujsf ( o_o_o_o_B_1_33 , o_o_o_B_1_34 , o_o_o_o_B_1_35 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; } static void mdlOutputs ( SimStruct * S , int_T tid ) { et34rxtuyzb * dw = ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; real_T const * i_kylr35diri = ( real_T * ) ssGetInputPortSignal ( S , 0 ) ; real_T const * i_oyh1bw2a0y = ( real_T * ) ssGetInputPortSignal ( S , 1 ) ; real_T const * i_hmmw02ig3c = ( real_T * ) ssGetInputPortSignal ( S , 2 ) ; real_T const * i_mtx1ekpvnf = ( real_T * ) ssGetInputPortSignal ( S , 3 ) ; real_T const * i_dlqtv5gz53 = ( real_T * ) ssGetInputPortSignal ( S , 4 ) ; real_T const * i_kmramgh1vg = ( real_T * ) ssGetInputPortSignal ( S , 5 ) ; real_T const * i_bhsfnw4zam = ( real_T * ) ssGetInputPortSignal ( S , 6 ) ; real_T const * i_ibyqpdg44o = ( real_T * ) ssGetInputPortSignal ( S , 7 ) ; real_T const * i_md3nushizo = ( real_T * ) ssGetInputPortSignal ( S , 8 ) ; real_T const * i_bcrlo4izwf = ( real_T * ) ssGetInputPortSignal ( S , 13 ) ; real_T const * i_mdswxopisg = ( real_T * ) ssGetInputPortSignal ( S , 14 ) ; real_T const * i_omz3rxkx5q = ( real_T * ) ssGetInputPortSignal ( S , 19 ) ; boolean_T const * i_eitq5kjv1w = ( boolean_T * ) ssGetInputPortSignal ( S , 20 ) ; boolean_T const * i_g3w4sjh1l0 = ( boolean_T * ) ssGetInputPortSignal ( S , 21 ) ; boolean_T const * i_g1jjd1rew4 = ( boolean_T * ) ssGetInputPortSignal ( S , 22 ) ; boolean_T const * i_mdvaboxr5x = ( boolean_T * ) ssGetInputPortSignal ( S , 23 ) ; boolean_T const * i_h4cpjev0d5 = ( boolean_T * ) ssGetInputPortSignal ( S , 24 ) ; boolean_T const * i_fyjm32bdy3 = ( boolean_T * ) ssGetInputPortSignal ( S , 25 ) ; boolean_T * o_B_1_1 = ( boolean_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_1_2 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_1_3 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_1_4 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_B_1_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_B_1_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_o_B_1_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_B_1_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; real_T * o_B_1_9 = ( real_T * ) ssGetOutputPortSignal ( S , 8 ) ; real_T * o_B_1_10 = ( real_T * ) ssGetOutputPortSignal ( S , 9 ) ; real_T * o_o_o_o_B_1_33 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; real_T * o_o_o_o_B_1_35 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; real_T * o_B_1_11 = ( real_T * ) ssGetOutputPortSignal ( S , 14 ) ; real_T * o_B_1_12 = ( real_T * ) ssGetOutputPortSignal ( S , 15 ) ; real_T * o_B_1_13 = ( real_T * ) ssGetOutputPortSignal ( S , 16 ) ; real_T * o_B_1_14 = ( real_T * ) ssGetOutputPortSignal ( S , 17 ) ; real_T * o_B_1_15 = ( real_T * ) ssGetOutputPortSignal ( S , 18 ) ; real_T * o_B_1_16 = ( real_T * ) ssGetOutputPortSignal ( S , 19 ) ; real_T * o_B_1_17 = ( real_T * ) ssGetOutputPortSignal ( S , 20 ) ; real_T * o_B_1_18 = ( real_T * ) ssGetOutputPortSignal ( S , 21 ) ; real_T * o_B_1_19 = ( real_T * ) ssGetOutputPortSignal ( S , 22 ) ; real_T * o_B_1_20 = ( real_T * ) ssGetOutputPortSignal ( S , 23 ) ; real_T * o_o_B_1_21 = ( real_T * ) ssGetOutputPortSignal ( S , 24 ) ; real_T * o_o_B_1_22 = ( real_T * ) ssGetOutputPortSignal ( S , 25 ) ; real_T * o_o_B_1_23 = ( real_T * ) ssGetOutputPortSignal ( S , 26 ) ; real_T * o_B_1_24 = ( real_T * ) ssGetOutputPortSignal ( S , 27 ) ; real_T * o_B_1_25 = ( real_T * ) ssGetOutputPortSignal ( S , 28 ) ; real_T * o_B_1_26 = ( real_T * ) ssGetOutputPortSignal ( S , 29 ) ; real_T * o_B_1_27 = ( real_T * ) ssGetOutputPortSignal ( S , 30 ) ; real_T * o_B_1_28 = ( real_T * ) ssGetOutputPortSignal ( S , 31 ) ; real_T * o_B_1_29 = ( real_T * ) ssGetOutputPortSignal ( S , 32 ) ; real_T * o_B_1_30 = ( real_T * ) ssGetOutputPortSignal ( S , 33 ) ; real_T * o_o_B_1_31 = ( real_T * ) ssGetOutputPortSignal ( S , 34 ) ; real_T * o_o_B_1_32 = ( real_T * ) ssGetOutputPortSignal ( S , 35 ) ; plypikssk0 * localX = ( plypikssk0 * ) ssGetContStates ( S ) ; real_T * o_o_o_B_1_34 = ( real_T * ) ssGetOutputPortSignal ( S , 11 ) ; if ( tid == PARAMETER_TUNING_TID ) { hexarotorTID2 ( o_o_o_o_B_1_33 , o_o_o_B_1_34 , o_o_o_o_B_1_35 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; } if ( tid != CONSTANT_TID && tid != PARAMETER_TUNING_TID ) { if ( ssIsSampleHit ( S , 0 , tid ) || ssIsMinorTimeStep ( S ) ) { hexarotor ( & ( dw -> rtm ) , i_kylr35diri , i_oyh1bw2a0y , i_hmmw02ig3c , i_mtx1ekpvnf , i_dlqtv5gz53 , i_kmramgh1vg , i_bhsfnw4zam , i_ibyqpdg44o , i_md3nushizo , i_bcrlo4izwf , i_mdswxopisg , i_omz3rxkx5q , i_eitq5kjv1w , i_g3w4sjh1l0 , i_g1jjd1rew4 , i_mdvaboxr5x , i_h4cpjev0d5 , i_fyjm32bdy3 , o_B_1_1 , o_B_1_2 , o_B_1_3 , o_B_1_4 , o_B_1_5 , o_B_1_6 , o_o_B_1_7 , o_o_B_1_8 , o_B_1_9 , o_B_1_10 , o_o_o_o_B_1_33 , o_o_o_o_B_1_35 , o_B_1_11 , o_B_1_12 , o_B_1_13 , o_B_1_14 , o_B_1_15 , o_B_1_16 , o_B_1_17 , o_B_1_18 , o_B_1_19 , o_B_1_20 , o_o_B_1_21 , o_o_B_1_22 , o_o_B_1_23 , o_B_1_24 , o_B_1_25 , o_B_1_26 , o_B_1_27 , o_B_1_28 , o_B_1_29 , o_B_1_30 , o_o_B_1_31 , o_o_B_1_32 , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX ) ; } } }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { et34rxtuyzb * dw = ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; real_T * o_o_B_1_21 = ( real_T * ) ssGetOutputPortSignal ( S , 24 ) ; real_T * o_o_B_1_22 = ( real_T * ) ssGetOutputPortSignal ( S , 25 ) ; real_T * o_o_B_1_23 = ( real_T * ) ssGetOutputPortSignal ( S , 26 ) ; real_T * o_o_B_1_31 = ( real_T * ) ssGetOutputPortSignal ( S , 34 ) ; real_T * o_o_B_1_32 = ( real_T * ) ssGetOutputPortSignal ( S , 35 ) ; o22h4v45uk ( & ( dw -> rtm ) , o_o_B_1_21 , o_o_B_1_22 , o_o_B_1_23 , o_o_B_1_31 , o_o_B_1_32 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; return ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { et34rxtuyzb * dw = ( et34rxtuyzb
* ) ssGetDWork ( S , 0 ) ; real_T * o_o_B_1_7 = ( real_T * )
ssGetOutputPortSignal ( S , 6 ) ; real_T * o_o_B_1_8 = ( real_T * )
ssGetOutputPortSignal ( S , 7 ) ; plypikssk0 * localX = ( plypikssk0 * )
ssGetContStates ( S ) ; lpahjwu50g * localXdot = ( lpahjwu50g * ) ssGetdX ( S
) ; btnkmpv1va ( o_o_B_1_7 , o_o_B_1_8 , & ( dw -> rtdw ) , localX ,
localXdot ) ; } static void mdlTerminate ( SimStruct * S ) { et34rxtuyzb * dw
= ( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; o1lq3fucf2 ( & ( dw -> rtdw ) , &
( dw -> rtm ) ) ; simTgtFreeOPModelData ( S ) ; return ; }
#define MDL_CLEANUP_RUNTIME_RESOURCES
static void mdlCleanupRuntimeResources ( SimStruct * S ) { et34rxtuyzb * dw =
( et34rxtuyzb * ) ssGetDWork ( S , 0 ) ; lzgk1uwtzq ( & ( dw -> rtm ) , & ( dw
-> rtdw ) ) ; }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static mxArray * mdlGetSimState ( SimStruct * S ) { static const char *
simStateFieldNames [ 7 ] = { "localX" , "mdlrefDW" , "disallowedStateData" ,
"tNext" , "tNextTid" , "nonContDerivSigInfoPrevVal" , "voidMemoryData" , } ;
mxArray * ss = mxCreateStructMatrix ( 1 , 1 , 7 , simStateFieldNames ) ; {
const plypikssk0 * localX = ( const plypikssk0 * ) ssGetContStates ( S ) ;
const size_t numBytes = sizeof ( plypikssk0 ) ; mxArray * storedX =
mxCreateNumericMatrix ( 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; UINT8_T *
rawData = ( UINT8_T * ) mxGetData ( storedX ) ; memcpy ( & rawData [ 0 ] ,
localX , numBytes ) ; mxSetFieldByNumber ( ss , 0 , 0 , storedX ) ; } {
mxArray * mdlrefDW = mr_hexarotor_GetDWork ( ssGetDWork ( S , 0 ) ) ;
mxSetFieldByNumber ( ss , 0 , 1 , mdlrefDW ) ; } { mxArray * data =
mr_hexarotor_GetSimStateDisallowedBlocks ( ) ; mxSetFieldByNumber ( ss , 0 ,
2 , data ) ; } ; mxSetFieldByNumber ( ss , 0 , 3 , mxCreateDoubleScalar ( ( double ) ssGetTNext ( S ) ) ) ; mxSetFieldByNumber ( ss , 0 , 4 , mxCreateDoubleScalar ( ( double ) ssGetTNextTid ( S ) ) ) ; { mxArray * voidMemData = simTgtSerializeVoidMemoryOP ( S ) ; mxSetFieldByNumber ( ss , 0 , 6 , voidMemData ) ; } return ss ; }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static void mdlSetSimState ( SimStruct * S , const mxArray * ss ) { { mxArray
* voidMemData = mxGetFieldByNumber ( ss , 0 , 6 ) ;
simTgtDeserializeVoidMemoryOP ( S , voidMemData ) ; } { plypikssk0 * localX =
( plypikssk0 * ) ssGetContStates ( S ) ; const size_t numBytes = sizeof ( plypikssk0 ) ; const mxArray * storedX = mxGetFieldByNumber ( ss , 0 , 0 ) ; const UINT8_T * rawData = ( const UINT8_T * ) mxGetData ( storedX ) ; memcpy ( localX , & rawData [ 0 ] , numBytes ) ; } mr_hexarotor_SetDWork ( ssGetDWork ( S , 0 ) , mxGetFieldByNumber ( ss , 0 , 1 ) ) ; ssSetTNext ( S , ( time_T ) mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 3 ) ) ) ; ssSetTNextTid ( S , ( int_T ) mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 4 ) ) ) ; }
#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#include "fixedpoint.c"
#else
#error Assertion failed: file must be compiled as a MEX-file
#endif
