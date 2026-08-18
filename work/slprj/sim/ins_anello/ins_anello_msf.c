#if !defined(S_FUNCTION_NAME)
#define S_FUNCTION_NAME ins_anello_msf
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
#include "ins_anello_types.h"
#include "ins_anello.h"
#include "ins_anello_private.h"
struct_EwMtsSUDbGvLjUwS89lioH rtP_vehicle ; real_T
rtP_anelloSiPhOGTemperatureCounts ; uint8_T rtP_anelloOutputMode ; const char
* rt_GetMatSignalLoggingFileName ( void ) { return NULL ; } const char *
rt_GetMatSigLogSelectorFileName ( void ) { return NULL ; } void *
rt_GetOSigstreamManager ( void ) { return NULL ; } void * rt_slioCatalogue ( void ) { return NULL ; } void * rtwGetPointerFromUniquePtr ( void * uniquePtr ) { return NULL ; } void * CreateDiagnosticAsVoidPtr ( const char * id , int nargs , ... ) { void * voidPtrDiagnostic = NULL ; va_list args ; va_start ( args , nargs ) ; slmrCreateDiagnostic ( id , nargs , args , & voidPtrDiagnostic ) ; va_end ( args ) ; return voidPtrDiagnostic ; } void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * simStrcut = ( SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( simStrcut ) ) { _ssSet_slLocalErrMsg ( simStrcut , diag ) ; } else { _ssDiscardDiagnostic ( simStrcut , diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) { _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) { _ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } const char * rt_CreateFullPathToTop ( const char * toppath , const char * subpath ) { char * fullpath = NULL ; slmrCreateFullPathToTop ( toppath , subpath , & fullpath ) ; return fullpath ; } boolean_T slIsRapidAcceleratorSimulating ( void ) { return false ; } void rt_RAccelReplaceFromFilename ( SimStruct * S , const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void rt_RAccelReplaceToFilename ( SimStruct * S , const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void * slsa_malloc ( size_t s ) { return malloc ( s ) ; } void slsa_free ( void * ptr ) { free ( ptr ) ; }
#define MDL_PROCESS_PARAMETERS
#if defined(MATLAB_MEX_FILE)
static void mdlProcessParameters ( SimStruct * S ) {
struct_EwMtsSUDbGvLjUwS89lioH * GlobalPrm_0 = ( struct_EwMtsSUDbGvLjUwS89lioH
* ) NULL ; real_T * GlobalPrm_1 = ( real_T * ) NULL ; uint8_T * GlobalPrm_2 =
( uint8_T * ) NULL ; if ( ! ssGetModelRefGlobalParamData ( S , 0 , ( void * *
) ( & GlobalPrm_0 ) ) ) return ; if ( GlobalPrm_0 != NULL ) { ( void ) memcpy
( & ( rtP_vehicle ) , GlobalPrm_0 , sizeof ( struct_EwMtsSUDbGvLjUwS89lioH )
) ; } if ( ! ssGetModelRefGlobalParamData ( S , 1 , ( void * * ) ( &
GlobalPrm_1 ) ) ) return ; if ( GlobalPrm_1 != NULL ) { ( void ) memcpy ( & ( rtP_anelloSiPhOGTemperatureCounts ) , GlobalPrm_1 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 2 , ( void * * ) ( & GlobalPrm_2 ) ) ) return ; if ( GlobalPrm_2 != NULL ) { ( void ) memcpy ( & ( rtP_anelloOutputMode ) , GlobalPrm_2 , sizeof ( uint8_T ) ) ; } }
#endif
static void mdlInitializeConditions ( SimStruct * S ) { doifm542cwv * dw = ( doifm542cwv * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_3_13 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; dwtc0bnvtm ( o_o_o_o_B_3_13 , & ( dw -> rtdw ) ) ; } static void mdlReset ( SimStruct * S ) { doifm542cwv * dw = ( doifm542cwv * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_3_13 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; kfkrrajk1l ( o_o_o_o_B_3_13 , & ( dw -> rtdw ) ) ; } static void mdlInitializeSizes ( SimStruct * S ) { if ( ( S -> mdlInfo -> genericFcn != ( NULL ) ) && ( ! ( S -> mdlInfo -> genericFcn ) ( S , GEN_FCN_CHK_MODELREF_SFUN_HAS_MODEL_BLOCK , - 1 , ( NULL ) ) ) ) { return ; } ssSetNumSFcnParams ( S , 0 ) ; ssFxpSetU32BitRegionCompliant ( S , 1 ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; } ssSetRTWGeneratedSFcn ( S , 2 ) ; ssSetNumContStates ( S , 0 ) ; ssSetNumDiscStates ( S , 0 ) ; ssSetSymbolicDimsSupport ( S , true ) ; slmrInitializeIOPortDataVectors ( S , 22 , 15 ) ; if ( ! ssSetNumInputPorts ( S , 22 ) ) return ; if ( ! ssSetInputPortVectorDimension ( S , 0 , 1 ) ) return ; ssSetInputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ; ssSetInputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 0 , SS_DOUBLE ) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 0 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 0 , 1 ) ; ssSetInputPortOptimOpts ( S
, 0 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 0 ,
false ) ; ssSetInputPortSampleTime ( S , 0 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 1 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 1 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 1 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 1 , 1 ) ; ssSetInputPortOptimOpts ( S
, 1 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 1 ,
false ) ; ssSetInputPortSampleTime ( S , 1 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 2 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 2 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 2 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 2 , 1 ) ; ssSetInputPortOptimOpts ( S
, 2 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 2 ,
false ) ; ssSetInputPortSampleTime ( S , 2 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 3 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 3 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 3 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 3 , 1 ) ; ssSetInputPortOptimOpts ( S
, 3 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 3 ,
false ) ; ssSetInputPortSampleTime ( S , 3 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 4 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 4 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 4 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 4 , 1 ) ; ssSetInputPortOptimOpts ( S
, 4 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 4 ,
false ) ; ssSetInputPortSampleTime ( S , 4 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 5 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 5 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 5 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 5 , 1 ) ; ssSetInputPortOptimOpts ( S
, 5 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 5 ,
false ) ; ssSetInputPortSampleTime ( S , 5 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 6 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 6 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 6 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 6 , 1 ) ; ssSetInputPortOptimOpts ( S
, 6 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 6 ,
false ) ; ssSetInputPortSampleTime ( S , 6 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 7 , 1 ) ) return ;
ssSetInputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 7 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 7 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 7 , 1 ) ; ssSetInputPortOptimOpts ( S
, 7 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 7 ,
false ) ; ssSetInputPortSampleTime ( S , 7 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 8 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 8 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 8 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 8 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 8 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 8 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 8 , 1 ) ; ssSetInputPortOptimOpts ( S
, 8 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 8 ,
false ) ; ssSetInputPortSampleTime ( S , 8 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 9 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 9 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 9 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 9 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 9 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 9 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 9 , 1 ) ; ssSetInputPortOptimOpts ( S
, 9 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 9 ,
false ) ; ssSetInputPortSampleTime ( S , 9 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 10 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 10 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 10 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 10 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 10 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 10 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 10 , 1 ) ; ssSetInputPortOptimOpts ( S
, 10 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 10 ,
false ) ; ssSetInputPortSampleTime ( S , 10 , - 1 ) ; if ( !
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
, 11 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 11 ,
false ) ; ssSetInputPortSampleTime ( S , 11 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 12 , 3 ) ) return ;
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
false ) ; ssSetInputPortSampleTime ( S , 12 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 13 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 13 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 13 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 13 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 13 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 13 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 13 , 1 ) ; ssSetInputPortOptimOpts ( S
, 13 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 13 ,
false ) ; ssSetInputPortSampleTime ( S , 13 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 14 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 14 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 14 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 14 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 14 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 14 , 0 ) ;
ssSetInputPortRequiredContiguous ( S , 14 , 1 ) ; ssSetInputPortOptimOpts ( S
, 14 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 14 ,
false ) ; ssSetInputPortSampleTime ( S , 14 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 15 , 3 ) ) return ;
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
false ) ; ssSetInputPortSampleTime ( S , 15 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 16 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 16 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 16 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 16 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 16 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 16 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 16 , 1 ) ; ssSetInputPortOptimOpts ( S
, 16 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 16 ,
false ) ; ssSetInputPortSampleTime ( S , 16 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 17 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 17 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 17 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 17 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 17 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 17 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 17 , 1 ) ; ssSetInputPortOptimOpts ( S
, 17 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 17 ,
false ) ; ssSetInputPortSampleTime ( S , 17 , - 1 ) ; if ( !
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
false ) ; ssSetInputPortSampleTime ( S , 18 , - 1 ) ; if ( !
ssSetInputPortMatrixDimensions ( S , 19 , 3 , 3 ) ) return ;
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
false ) ; ssSetInputPortSampleTime ( S , 19 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 20 , 3 ) ) return ;
ssSetInputPortDimensionsMode ( S , 20 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 20 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 20 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 20 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 20 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 20 , 1 ) ; ssSetInputPortOptimOpts ( S
, 20 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 20 ,
false ) ; ssSetInputPortSampleTime ( S , 20 , - 1 ) ; if ( !
ssSetInputPortVectorDimension ( S , 21 , 6 ) ) return ;
ssSetInputPortDimensionsMode ( S , 21 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 21 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 21 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 21 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 21 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 21 , 1 ) ; ssSetInputPortOptimOpts ( S
, 21 , SS_NOT_REUSABLE_AND_LOCAL ) ; ssSetInputPortOverWritable ( S , 21 ,
false ) ; ssSetInputPortSampleTime ( S , 21 , - 1 ) ; if ( !
ssSetNumOutputPorts ( S , 15 ) ) return ; if ( !
ssSetOutputPortVectorDimension ( S , 0 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 0 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 0 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 0 , 0 ) ; ssSetOutputPortDiscreteValuedOutput
( S , 0 , 0 ) ; ssSetOutputPortOkToMerge ( S , 0 , SS_OK_TO_MERGE_CONDITIONAL
) ; ssSetOutputPortICAttributes ( S , 0 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 0 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 1 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 1 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 1 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 1 , 0 ) ; ssSetOutputPortOkToMerge
( S , 1 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 1
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 1 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 2 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 2 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 2 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 2 , 0 ) ; ssSetOutputPortOkToMerge
( S , 2 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 2
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 2 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 3 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 3 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 3 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 3 , 0 ) ; ssSetOutputPortOkToMerge
( S , 3 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 3
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 3 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 4 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 4 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 4 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 4 , 0 ) ; ssSetOutputPortOkToMerge
( S , 4 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 4
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 4 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 5 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 5 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 5 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 5 , 0 ) ; ssSetOutputPortDiscreteValuedOutput
( S , 5 , 0 ) ; ssSetOutputPortOkToMerge ( S , 5 , SS_OK_TO_MERGE_CONDITIONAL
) ; ssSetOutputPortICAttributes ( S , 5 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 5 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 6 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 6 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 6 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 6 , 0 ) ; ssSetOutputPortOkToMerge
( S , 6 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 6
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 6 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 7 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 7 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 7 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 7 , 0 ) ; ssSetOutputPortOkToMerge
( S , 7 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 7
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 7 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 8 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 8 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 8 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 8 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 8 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 8 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 8 , 0 ) ; ssSetOutputPortOkToMerge
( S , 8 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 8
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 8 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 9 ,
1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 9 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 9 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 9 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 9 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 9 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 9 , 0 ) ; ssSetOutputPortOkToMerge
( S , 9 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S , 9
, false , false , false ) ; ssSetOutputPortOptimOpts ( S , 9 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 10
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 10 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 10 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 10 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 10 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 10 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 10 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 10 , 0 ) ; ssSetOutputPortOkToMerge
( S , 10 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S ,
10 , false , false , false ) ; ssSetOutputPortOptimOpts ( S , 10 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 11
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 11 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 11 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 11 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 11 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 11 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 11 , 0 ) ; ssSetOutputPortOkToMerge
( S , 11 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S ,
11 , false , false , false ) ; ssSetOutputPortOptimOpts ( S , 11 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 12
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 12 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 12 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 12 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 12 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 12 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 12 , 0 ) ; ssSetOutputPortOkToMerge
( S , 12 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S ,
12 , false , false , false ) ; ssSetOutputPortOptimOpts ( S , 12 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 13
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 13 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 13 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 13 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 13 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 13 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 13 , 0 ) ; ssSetOutputPortOkToMerge
( S , 13 , SS_OK_TO_MERGE_CONDITIONAL ) ; ssSetOutputPortICAttributes ( S ,
13 , false , false , false ) ; ssSetOutputPortOptimOpts ( S , 13 ,
SS_NOT_REUSABLE_AND_LOCAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 14
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 14 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 14 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 14 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 14 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 14 , - 1 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 14 , 0 ) ; ssSetOutputPortOkToMerge
( S , 14 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 14 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 14 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetSimStateCompliance ( S ,
USE_CUSTOM_SIM_STATE ) ; mr_ins_anello_RegisterSimStateChecksum ( S ) ;
ssSetNumSampleTimes ( S , PORT_BASED_SAMPLE_TIMES ) ;
ssSetParameterTuningCompliance ( S , true ) ; ssSetNumRWork ( S , 0 ) ;
ssSetNumIWork ( S , 0 ) ; ssSetNumPWork ( S , 0 ) ; ssSetNumModes ( S , 0 ) ;
{ int_T zcsIdx = 0 ; } ssSetOutputPortIsNonContinuous ( S , 0 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 0 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 1 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 1 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 2 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 2 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 3 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 3 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 4 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 4 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 5 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 5 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 6 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 6 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 7 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 7 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 8 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 8 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 9 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 9 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 10 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 10 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 11 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 11 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 12 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 12 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 13 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 13 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 14 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 14 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 0 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 1 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 2 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 3 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 4 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 5 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 6 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 7 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 8 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 9 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 10 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 11 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 12 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 13 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 14 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 15 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 16 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 17 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 18 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 19 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 20 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 21 , 1 ) ;
ssSetModelReferenceSampleTimeInheritanceRule ( S ,
USE_DEFAULT_FOR_DISCRETE_INHERITANCE ) ; ssSetRuntimeThreadSafetyCompliance ( S
, RUNTIME_THREAD_SAFETY_COMPLIANCE_TRUE ) ; ssSetAcceptsFcnCallInputs ( S ) ;
ssSetModelReferenceNormalModeSupport ( S ,
MDL_START_AND_MDL_PROCESS_PARAMS_OK ) ; ssSupportsMultipleExecInstances ( S ,
true ) ; ssHasStateInsideForEachSS ( S , false ) ; ssSetOptions ( S ,
SS_OPTION_ALLOW_CONSTANT_PORT_SAMPLE_TIME |
SS_OPTION_ALLOW_PORT_SAMPLE_TIME_IN_TRIGSS |
SS_OPTION_SUPPORTS_ALIAS_DATA_TYPES | SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME
| SS_OPTION_EXCEPTION_FREE_CODE | SS_OPTION_WORKS_WITH_CODE_REUSE ) ;
#if SS_SFCN_FOR_SIM
if ( S -> mdlInfo -> genericFcn != ( NULL ) && ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { int_T retVal = 1 ; mr_ins_anello_MdlInfoRegFcn
( S , "ins_anello" , & retVal ) ; if ( ! retVal ) return ; }
#endif
#if SS_SFCN_FOR_SIM
if ( ssSetNumDWork ( S , 1 ) ) { int mdlrefDWTypeId ; ssRegMdlRefDWorkType ( S
, & mdlrefDWTypeId ) ; if ( mdlrefDWTypeId == INVALID_DTYPE_ID ) return ; if
( ! ssSetDataTypeSize ( S , mdlrefDWTypeId , sizeof ( doifm542cwv ) ) )
return ; ssSetDWorkDataType ( S , 0 , mdlrefDWTypeId ) ; ssSetDWorkWidth ( S
, 0 , 1 ) ; }
#else
if ( ! ssSetNumDWork ( S , 1 ) ) { return ; }
#endif
slmrRegisterSystemInitializeMethod ( S , mdlInitializeConditions ) ;
slmrRegisterSystemResetMethod ( S , mdlReset ) ;
ssSetSimulinkVersionGeneratedIn ( S , "26.1" ) ; } static void
mdlInitializeSampleTimes ( SimStruct * S ) { return ; }
#define MDL_SET_WORK_WIDTHS
static void mdlSetWorkWidths ( SimStruct * S ) { if ( S -> mdlInfo ->
genericFcn != ( NULL ) ) { ssSetSignalSizesComputeType ( S ,
SS_VARIABLE_SIZE_FROM_INPUT_VALUE_AND_SIZE ) ; } { static const char *
toFileNames [ ] = { "" } ; static const char * fromFileNames [ ] = { "" } ;
if ( ! ssSetModelRefFromFiles ( S , 0 , fromFileNames ) ) return ; if ( !
ssSetModelRefToFiles ( S , 0 , toFileNames ) ) return ; } }
#define MDL_SETUP_RUNTIME_RESOURCES
static void mdlSetupRuntimeResources ( SimStruct * S ) { doifm542cwv * dw = ( doifm542cwv * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_3_13 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; void * sysRanPtr = ( NULL ) ; int_T contextTid = 0 ; ssGetContextSysRanBCPtr ( S , & sysRanPtr ) ; ssGetContextSysTid ( S , & contextTid ) ; if ( contextTid == CONSTANT_TID ) { contextTid = 0 ; } simTgtAllocOPModelData ( S ) ; l55uemkp5l ( S , slmrGetTopTidFromMdlRefChildTid ( S , 0 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 1 , true ) , & ( dw -> rtm ) , & ( dw -> rtb ) , & ( dw -> rtdw ) , sysRanPtr , contextTid , ( ( NULL ) ) , ( ( NULL ) ) , 0 , - 1 ) ; ssSetModelMappingInfoPtr ( S , & ( dw -> rtm . DataMapInfo . mmi ) ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; } }
#define MDL_START
static void mdlStart ( SimStruct * S ) { doifm542cwv * dw = ( doifm542cwv * )
ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_B_3_13 = ( real_T * )
ssGetOutputPortSignal ( S , 0 ) ; mdlProcessParameters ( S ) ; njopuutrc4 ( o_o_o_o_B_3_13 ) ; } static void mdlOutputs ( SimStruct * S , int_T tid ) { doifm542cwv * dw = ( doifm542cwv * ) ssGetDWork ( S , 0 ) ; real_T const * i_kcpjrtzr2l = ( real_T * ) ssGetInputPortSignal ( S , 0 ) ; real_T const * i_k1gatg5qqg = ( real_T * ) ssGetInputPortSignal ( S , 1 ) ; real_T const * i_c0euqgiimv = ( real_T * ) ssGetInputPortSignal ( S , 3 ) ; real_T const * i_j4hktq1gja = ( real_T * ) ssGetInputPortSignal ( S , 16 ) ; real_T const * i_lfhzl551bc = ( real_T * ) ssGetInputPortSignal ( S , 17 ) ; real_T const * i_jjo5tqnbyv = ( real_T * ) ssGetInputPortSignal ( S , 19 ) ; real_T const * i_if4pwp3urg = ( real_T * ) ssGetInputPortSignal ( S , 20 ) ; real_T const * i_j0nohmobcx = ( real_T * ) ssGetInputPortSignal ( S , 21 ) ; real_T * o_B_3_1 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_3_2 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_3_3 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_B_3_4 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_B_3_5 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_B_3_6 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; real_T * o_B_3_7 = ( real_T * ) ssGetOutputPortSignal ( S , 8 ) ; real_T * o_B_3_8 = ( real_T * ) ssGetOutputPortSignal ( S , 9 ) ; real_T * o_B_3_9 = ( real_T * ) ssGetOutputPortSignal ( S , 11 ) ; real_T * o_B_3_10 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; real_T * o_B_3_11 = ( real_T * ) ssGetOutputPortSignal ( S , 13 ) ; real_T * o_B_3_12 = ( real_T * ) ssGetOutputPortSignal ( S , 14 ) ; real_T * o_o_o_o_B_3_13 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_3_14 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_B_3_15 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; if ( tid == PARAMETER_TUNING_TID ) { ins_anelloTID1 ( o_o_o_o_B_3_13 , o_B_3_14 , o_B_3_15 ) ; } if ( tid != CONSTANT_TID && tid != PARAMETER_TUNING_TID ) { ins_anello ( i_kcpjrtzr2l , i_k1gatg5qqg , i_c0euqgiimv , i_j4hktq1gja , i_lfhzl551bc , i_jjo5tqnbyv , i_if4pwp3urg , i_j0nohmobcx , o_B_3_1 , o_B_3_2 , o_B_3_3 , o_B_3_4 , o_B_3_5 , o_B_3_6 , o_B_3_7 , o_B_3_8 , o_B_3_9 , o_B_3_10 , o_B_3_11 , o_B_3_12 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; } } static void mdlTerminate ( SimStruct * S ) { doifm542cwv * dw = ( doifm542cwv * ) ssGetDWork ( S , 0 ) ; namfu1ihid ( & ( dw -> rtm ) ) ; simTgtFreeOPModelData ( S ) ; return ; }
#define MDL_CLEANUP_RUNTIME_RESOURCES
static void mdlCleanupRuntimeResources ( SimStruct * S ) { }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static mxArray * mdlGetSimState ( SimStruct * S ) { static const char *
simStateFieldNames [ 7 ] = { "localX" , "mdlrefDW" , "disallowedStateData" ,
"tNext" , "tNextTid" , "nonContDerivSigInfoPrevVal" , "voidMemoryData" , } ;
mxArray * ss = mxCreateStructMatrix ( 1 , 1 , 7 , simStateFieldNames ) ; {
mxArray * mdlrefDW = mr_ins_anello_GetDWork ( ssGetDWork ( S , 0 ) ) ;
mxSetFieldByNumber ( ss , 0 , 1 , mdlrefDW ) ; } { mxArray * data =
mr_ins_anello_GetSimStateDisallowedBlocks ( ) ; mxSetFieldByNumber ( ss , 0 ,
2 , data ) ; } ; mxSetFieldByNumber ( ss , 0 , 3 , mxCreateDoubleScalar ( ( double ) ssGetTNext ( S ) ) ) ; mxSetFieldByNumber ( ss , 0 , 4 , mxCreateDoubleScalar ( ( double ) ssGetTNextTid ( S ) ) ) ; { mxArray * voidMemData = simTgtSerializeVoidMemoryOP ( S ) ; mxSetFieldByNumber ( ss , 0 , 6 , voidMemData ) ; } return ss ; }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static void mdlSetSimState ( SimStruct * S , const mxArray * ss ) { { mxArray
* voidMemData = mxGetFieldByNumber ( ss , 0 , 6 ) ;
simTgtDeserializeVoidMemoryOP ( S , voidMemData ) ; } mr_ins_anello_SetDWork
( ssGetDWork ( S , 0 ) , mxGetFieldByNumber ( ss , 0 , 1 ) ) ; ssSetTNext ( S
, ( time_T ) mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 3 ) ) ) ;
ssSetTNextTid ( S , ( int_T ) mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 4 )
) ) ; }
#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#include "fixedpoint.c"
#else
#error Assertion failed: file must be compiled as a MEX-file
#endif
