#if !defined(S_FUNCTION_NAME)
#define S_FUNCTION_NAME VehiclePlant_msf
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
#include "VehiclePlant_types.h"
#include "VehiclePlant.h"
#include "VehiclePlant_private.h"
#include "environment.h"
#include "sensors.h"
#include "hexarotor.h"
struct_7BigaldNWGj33rb8sElmtH rtP_parameters ; real_T rtP_C_D [ 19 ] ; real_T
rtP_C_L [ 19 ] ; real_T rtP_INS_VARIANT ; real_T rtP_WIND_SOURCE ; real_T
rtP_actuatorDelay_s ; real_T rtP_airDensityAtStd_kgpm3 ; real_T
rtP_aircraftInitialBodyRates_radps [ 3 ] ; real_T
rtP_aircraftInitialEuler_rad [ 3 ] ; real_T rtP_aircraftInitialPosInNED_m [ 3
] ; real_T rtP_aircraftInitialVelInBody_mps [ 3 ] ; real_T rtP_alphaBkpts_rad
[ 19 ] ; real_T rtP_gustAmplitude_mps [ 3 ] ; real_T rtP_gustDuration_sec ;
real_T rtP_gustLength_m [ 3 ] ; real_T rtP_gustStartTime_sec ; real_T
rtP_referenceAltitude_m ; real_T rtP_referenceDecimalYear_years ; real_T
rtP_rpm2radps ; real_T rtP_terrainHeightNED_m ; real_T
rtP_turbulenceEndTime_sec ; real_T rtP_turbulenceMinAirspeed_mps ; real_T
rtP_turbulenceStartTime_sec ; real_T rtP_turbulenceWingspan_m ; real_T
rtP_xWindInNED_mps ; real_T rtP_yWindInNED_mps ; real_T rtP_zWindInNED_mps ;
boolean_T rtP_gustEnabled ; boolean_T rtP_turbulenceEnabled ; const char *
rt_GetMatSignalLoggingFileName ( void ) { return NULL ; } const char *
rt_GetMatSigLogSelectorFileName ( void ) { return NULL ; } void *
rt_GetOSigstreamManager ( void ) { return NULL ; } void * rt_slioCatalogue ( void ) { return NULL ; } void * rtwGetPointerFromUniquePtr ( void * uniquePtr ) { return NULL ; } void * CreateDiagnosticAsVoidPtr ( const char * id , int nargs , ... ) { void * voidPtrDiagnostic = NULL ; va_list args ; va_start ( args , nargs ) ; slmrCreateDiagnostic ( id , nargs , args , & voidPtrDiagnostic ) ; va_end ( args ) ; return voidPtrDiagnostic ; } void rt_ssSet_slErrMsg ( void * S , void * diag ) { SimStruct * simStrcut = ( SimStruct * ) S ; if ( ! _ssIsErrorStatusAslErrMsg ( simStrcut ) ) { _ssSet_slLocalErrMsg ( simStrcut , diag ) ; } else { _ssDiscardDiagnostic ( simStrcut , diag ) ; } } void rt_ssReportDiagnosticAsWarning ( void * S , void * diag ) { _ssReportDiagnosticAsWarning ( ( SimStruct * ) S , diag ) ; } void rt_ssReportDiagnosticAsInfo ( void * S , void * diag ) { _ssReportDiagnosticAsInfo ( ( SimStruct * ) S , diag ) ; } const char * rt_CreateFullPathToTop ( const char * toppath , const char * subpath ) { char * fullpath = NULL ; slmrCreateFullPathToTop ( toppath , subpath , & fullpath ) ; return fullpath ; } boolean_T slIsRapidAcceleratorSimulating ( void ) { return false ; } void rt_RAccelReplaceFromFilename ( SimStruct * S , const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void rt_RAccelReplaceToFilename ( SimStruct * S , const char * blockpath , char * fileName ) { ( void ) blockpath ; ( void ) fileName ; } void * slsa_malloc ( size_t s ) { return malloc ( s ) ; } void slsa_free ( void * ptr ) { free ( ptr ) ; }
#define MDL_PROCESS_PARAMETERS
#if defined(MATLAB_MEX_FILE)
static void mdlProcessParameters ( SimStruct * S ) {
struct_7BigaldNWGj33rb8sElmtH * GlobalPrm_0 = ( struct_7BigaldNWGj33rb8sElmtH
* ) NULL ; real_T * GlobalPrm_1 = ( real_T * ) NULL ; real_T * GlobalPrm_2 =
( real_T * ) NULL ; real_T * GlobalPrm_3 = ( real_T * ) NULL ; real_T *
GlobalPrm_4 = ( real_T * ) NULL ; real_T * GlobalPrm_5 = ( real_T * ) NULL ;
real_T * GlobalPrm_6 = ( real_T * ) NULL ; real_T * GlobalPrm_7 = ( real_T *
) NULL ; real_T * GlobalPrm_8 = ( real_T * ) NULL ; real_T * GlobalPrm_9 = ( real_T * ) NULL ; real_T * GlobalPrm_10 = ( real_T * ) NULL ; real_T * GlobalPrm_11 = ( real_T * ) NULL ; real_T * GlobalPrm_12 = ( real_T * ) NULL ; real_T * GlobalPrm_13 = ( real_T * ) NULL ; real_T * GlobalPrm_14 = ( real_T * ) NULL ; real_T * GlobalPrm_15 = ( real_T * ) NULL ; real_T * GlobalPrm_16 = ( real_T * ) NULL ; real_T * GlobalPrm_17 = ( real_T * ) NULL ; real_T * GlobalPrm_18 = ( real_T * ) NULL ; real_T * GlobalPrm_19 = ( real_T * ) NULL ; real_T * GlobalPrm_20 = ( real_T * ) NULL ; real_T * GlobalPrm_21 = ( real_T * ) NULL ; real_T * GlobalPrm_22 = ( real_T * ) NULL ; real_T * GlobalPrm_23 = ( real_T * ) NULL ; real_T * GlobalPrm_24 = ( real_T * ) NULL ; real_T * GlobalPrm_25 = ( real_T * ) NULL ; real_T * GlobalPrm_26 = ( real_T * ) NULL ; boolean_T * GlobalPrm_27 = ( boolean_T * ) NULL ; boolean_T * GlobalPrm_28 = ( boolean_T * ) NULL ; if ( ! ssGetModelRefGlobalParamData ( S , 0 , ( void * * ) ( & GlobalPrm_0 ) ) ) return ; if ( GlobalPrm_0 != NULL ) { ( void ) memcpy ( & ( rtP_parameters ) , GlobalPrm_0 , sizeof ( struct_7BigaldNWGj33rb8sElmtH ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 1 , ( void * * ) ( & GlobalPrm_1 ) ) ) return ; if ( GlobalPrm_1 != NULL ) { ( void ) memcpy ( & ( rtP_C_D [ 0 ] ) , GlobalPrm_1 , sizeof ( real_T ) * 19 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 2 , ( void * * ) ( & GlobalPrm_2 ) ) ) return ; if ( GlobalPrm_2 != NULL ) { ( void ) memcpy ( & ( rtP_C_L [ 0 ] ) , GlobalPrm_2 , sizeof ( real_T ) * 19 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 3 , ( void * * ) ( & GlobalPrm_3 ) ) ) return ; if ( GlobalPrm_3 != NULL ) { ( void ) memcpy ( & ( rtP_INS_VARIANT ) , GlobalPrm_3 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 4 , ( void * * ) ( & GlobalPrm_4 ) ) ) return ; if ( GlobalPrm_4 != NULL ) { ( void ) memcpy ( & ( rtP_WIND_SOURCE ) , GlobalPrm_4 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 5 , ( void * * ) ( & GlobalPrm_5 ) ) ) return ; if ( GlobalPrm_5 != NULL ) { ( void ) memcpy ( & ( rtP_actuatorDelay_s ) , GlobalPrm_5 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 6 , ( void * * ) ( & GlobalPrm_6 ) ) ) return ; if ( GlobalPrm_6 != NULL ) { ( void ) memcpy ( & ( rtP_airDensityAtStd_kgpm3 ) , GlobalPrm_6 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 7 , ( void * * ) ( & GlobalPrm_7 ) ) ) return ; if ( GlobalPrm_7 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialBodyRates_radps [ 0 ] ) , GlobalPrm_7 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 8 , ( void * * ) ( & GlobalPrm_8 ) ) ) return ; if ( GlobalPrm_8 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialEuler_rad [ 0 ] ) , GlobalPrm_8 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 9 , ( void * * ) ( & GlobalPrm_9 ) ) ) return ; if ( GlobalPrm_9 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialPosInNED_m [ 0 ] ) , GlobalPrm_9 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 10 , ( void * * ) ( & GlobalPrm_10 ) ) ) return ; if ( GlobalPrm_10 != NULL ) { ( void ) memcpy ( & ( rtP_aircraftInitialVelInBody_mps [ 0 ] ) , GlobalPrm_10 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 11 , ( void * * ) ( & GlobalPrm_11 ) ) ) return ; if ( GlobalPrm_11 != NULL ) { ( void ) memcpy ( & ( rtP_alphaBkpts_rad [ 0 ] ) , GlobalPrm_11 , sizeof ( real_T ) * 19 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 12 , ( void * * ) ( & GlobalPrm_12 ) ) ) return ; if ( GlobalPrm_12 != NULL ) { ( void ) memcpy ( & ( rtP_gustAmplitude_mps [ 0 ] ) , GlobalPrm_12 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 13 , ( void * * ) ( & GlobalPrm_13 ) ) ) return ; if ( GlobalPrm_13 != NULL ) { ( void ) memcpy ( & ( rtP_gustDuration_sec ) , GlobalPrm_13 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 14 , ( void * * ) ( & GlobalPrm_14 ) ) ) return ; if ( GlobalPrm_14 != NULL ) { ( void ) memcpy ( & ( rtP_gustLength_m [ 0 ] ) , GlobalPrm_14 , sizeof ( real_T ) * 3 ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 15 , ( void * * ) ( & GlobalPrm_15 ) ) ) return ; if ( GlobalPrm_15 != NULL ) { ( void ) memcpy ( & ( rtP_gustStartTime_sec ) , GlobalPrm_15 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 16 , ( void * * ) ( & GlobalPrm_16 ) ) ) return ; if ( GlobalPrm_16 != NULL ) { ( void ) memcpy ( & ( rtP_referenceAltitude_m ) , GlobalPrm_16 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 17 , ( void * * ) ( & GlobalPrm_17 ) ) ) return ; if ( GlobalPrm_17 != NULL ) { ( void ) memcpy ( & ( rtP_referenceDecimalYear_years ) , GlobalPrm_17 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 18 , ( void * * ) ( & GlobalPrm_18 ) ) ) return ; if ( GlobalPrm_18 != NULL ) { ( void ) memcpy ( & ( rtP_rpm2radps ) , GlobalPrm_18 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 19 , ( void * * ) ( & GlobalPrm_19 ) ) ) return ; if ( GlobalPrm_19 != NULL ) { ( void ) memcpy ( & ( rtP_terrainHeightNED_m ) , GlobalPrm_19 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 20 , ( void * * ) ( & GlobalPrm_20 ) ) ) return ; if ( GlobalPrm_20 != NULL ) { ( void ) memcpy ( & ( rtP_turbulenceEndTime_sec ) , GlobalPrm_20 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 21 , ( void * * ) ( & GlobalPrm_21 ) ) ) return ; if ( GlobalPrm_21 != NULL ) { ( void ) memcpy ( & ( rtP_turbulenceMinAirspeed_mps ) , GlobalPrm_21 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 22 , ( void * * ) ( & GlobalPrm_22 ) ) ) return ; if ( GlobalPrm_22 != NULL ) { ( void ) memcpy ( & ( rtP_turbulenceStartTime_sec ) , GlobalPrm_22 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 23 , ( void * * ) ( & GlobalPrm_23 ) ) ) return ; if ( GlobalPrm_23 != NULL ) { ( void ) memcpy ( & ( rtP_turbulenceWingspan_m ) , GlobalPrm_23 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 24 , ( void * * ) ( & GlobalPrm_24 ) ) ) return ; if ( GlobalPrm_24 != NULL ) { ( void ) memcpy ( & ( rtP_xWindInNED_mps ) , GlobalPrm_24 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 25 , ( void * * ) ( & GlobalPrm_25 ) ) ) return ; if ( GlobalPrm_25 != NULL ) { ( void ) memcpy ( & ( rtP_yWindInNED_mps ) , GlobalPrm_25 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 26 , ( void * * ) ( & GlobalPrm_26 ) ) ) return ; if ( GlobalPrm_26 != NULL ) { ( void ) memcpy ( & ( rtP_zWindInNED_mps ) , GlobalPrm_26 , sizeof ( real_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 27 , ( void * * ) ( & GlobalPrm_27 ) ) ) return ; if ( GlobalPrm_27 != NULL ) { ( void ) memcpy ( & ( rtP_gustEnabled ) , GlobalPrm_27 , sizeof ( boolean_T ) ) ; } if ( ! ssGetModelRefGlobalParamData ( S , 28 , ( void * * ) ( & GlobalPrm_28 ) ) ) return ; if ( GlobalPrm_28 != NULL ) { ( void ) memcpy ( & ( rtP_turbulenceEnabled ) , GlobalPrm_28 , sizeof ( boolean_T ) ) ; } }
#endif
static void mdlInitializeConditions ( SimStruct * S ) { owiw5mmjy2f * dw = ( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_o_B_1_77 = ( real_T * ) ssGetOutputPortSignal ( S , 44 ) ; real_T * o_o_o_o_B_1_78 = ( real_T * ) ssGetOutputPortSignal ( S , 45 ) ; real_T * o_o_o_o_o_B_1_79 = ( real_T * ) ssGetOutputPortSignal ( S , 46 ) ; real_T * o_o_B_1_66 = ( real_T * ) ssGetOutputPortSignal ( S , 70 ) ; real_T * o_o_B_1_67 = ( real_T * ) ssGetOutputPortSignal ( S , 71 ) ; real_T * o_o_o_o_B_1_81 = ( real_T * ) ssGetOutputPortSignal ( S , 72 ) ; hdulqcnnpv * localX = ( hdulqcnnpv * ) ssGetContStates ( S ) ; cjxjkcm4ra ( o_o_o_o_o_B_1_77 , o_o_o_o_B_1_78 , o_o_o_o_o_B_1_79 , o_o_B_1_66 , o_o_B_1_67 , o_o_o_o_B_1_81 , & ( dw -> rtdw ) , localX ) ; } static void mdlReset ( SimStruct * S ) { owiw5mmjy2f * dw = ( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; hdulqcnnpv * localX = ( hdulqcnnpv * ) ssGetContStates ( S ) ; dapgvgjvnf ( & ( dw -> rtdw ) , localX ) ; } static void mdlPeriodicOutputUpdate ( SimStruct * S , int_T tid ) { owiw5mmjy2f * dw = ( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; boolean_T const * i_aqbxopp4od = ( boolean_T * ) ssGetInputPortSignal ( S , 0 ) ; boolean_T const * i_gom2amlqr1 = ( boolean_T * ) ssGetInputPortSignal ( S , 1 ) ; boolean_T const * i_dvujmtstvi = ( boolean_T * ) ssGetInputPortSignal ( S , 2 ) ; boolean_T const * i_ar2tixbyms = ( boolean_T * ) ssGetInputPortSignal ( S , 3 ) ; boolean_T const * i_pks5fmem4t = ( boolean_T * ) ssGetInputPortSignal ( S , 4 ) ; boolean_T const * i_iuwyb4fljm = ( boolean_T * ) ssGetInputPortSignal ( S , 5 ) ; real_T const * i_domdentxv3 = ( real_T * ) ssGetInputPortSignal ( S , 6 ) ; real_T const * i_mvdqaplnnt = ( real_T * ) ssGetInputPortSignal ( S , 7 ) ; real_T const * i_g45kr4jwpf = ( real_T * ) ssGetInputPortSignal ( S , 8 ) ; real_T const * i_aibdphnll3 = ( real_T * ) ssGetInputPortSignal ( S , 9 ) ; real_T const * i_k3dmq3ayz4 = ( real_T * ) ssGetInputPortSignal ( S , 10 ) ; real_T const * i_evn3anml3h = ( real_T * ) ssGetInputPortSignal ( S , 11 ) ; real_T const * i_nhq1rhohzj = ( real_T * ) ssGetInputPortSignal ( S , 12 ) ; real_T const * i_bg0p24qife = ( real_T * ) ssGetInputPortSignal ( S , 13 ) ; real_T * o_B_1_1 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_1_2 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_1_3 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_1_4 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_B_1_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_B_1_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_B_1_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_B_1_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; real_T * o_B_1_9 = ( real_T * ) ssGetOutputPortSignal ( S , 8 ) ; real_T * o_B_1_10 = ( real_T * ) ssGetOutputPortSignal ( S , 9 ) ; real_T * o_B_1_11 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; real_T * o_B_1_12 = ( real_T * ) ssGetOutputPortSignal ( S , 11 ) ; real_T * o_B_1_13 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; real_T * o_B_1_14 = ( real_T * ) ssGetOutputPortSignal ( S , 13 ) ; real_T * o_B_1_15 = ( real_T * ) ssGetOutputPortSignal ( S , 14 ) ; real_T * o_B_1_16 = ( real_T * ) ssGetOutputPortSignal ( S , 15 ) ; real_T * o_B_1_17 = ( real_T * ) ssGetOutputPortSignal ( S , 16 ) ; real_T * o_B_1_18 = ( real_T * ) ssGetOutputPortSignal ( S , 17 ) ; real_T * o_B_1_19 = ( real_T * ) ssGetOutputPortSignal ( S , 18 ) ; real_T * o_B_1_20 = ( real_T * ) ssGetOutputPortSignal ( S , 19 ) ; real_T * o_B_1_21 = ( real_T * ) ssGetOutputPortSignal ( S , 20 ) ; real_T * o_B_1_22 = ( real_T * ) ssGetOutputPortSignal ( S , 21 ) ; real_T * o_B_1_23 = ( real_T * ) ssGetOutputPortSignal ( S , 22 ) ; real_T * o_B_1_24 = ( real_T * ) ssGetOutputPortSignal ( S , 23 ) ; real_T * o_B_1_25 = ( real_T * ) ssGetOutputPortSignal ( S , 24 ) ; real_T * o_B_1_26 = ( real_T * ) ssGetOutputPortSignal ( S , 25 ) ; real_T * o_B_1_27 = ( real_T * ) ssGetOutputPortSignal ( S , 26 ) ; real_T * o_B_1_28 = ( real_T * ) ssGetOutputPortSignal ( S , 27 ) ; real_T * o_B_1_29 = ( real_T * ) ssGetOutputPortSignal ( S , 28 ) ; real_T * o_B_1_30 = ( real_T * ) ssGetOutputPortSignal ( S , 29 ) ; real_T * o_B_1_31 = ( real_T * ) ssGetOutputPortSignal ( S , 30 ) ; real_T * o_B_1_32 = ( real_T * ) ssGetOutputPortSignal ( S , 31 ) ; real_T * o_B_1_33 = ( real_T * ) ssGetOutputPortSignal ( S , 32 ) ; real_T * o_B_1_34 = ( real_T * ) ssGetOutputPortSignal ( S , 33 ) ; boolean_T * o_o_B_1_35 = ( boolean_T * ) ssGetOutputPortSignal ( S , 34 ) ; real_T * o_o_B_1_36 = ( real_T * ) ssGetOutputPortSignal ( S , 35 ) ; real_T * o_o_B_1_37 = ( real_T * ) ssGetOutputPortSignal ( S , 36 ) ; real_T * o_o_B_1_38 = ( real_T * ) ssGetOutputPortSignal ( S , 37 ) ; real_T * o_o_B_1_39 = ( real_T * ) ssGetOutputPortSignal ( S , 38 ) ; real_T * o_o_B_1_40 = ( real_T * ) ssGetOutputPortSignal ( S , 39 ) ; real_T * o_o_o_B_1_41 = ( real_T * ) ssGetOutputPortSignal ( S , 40 ) ; real_T * o_o_o_B_1_42 = ( real_T * ) ssGetOutputPortSignal ( S , 41 ) ; real_T * o_o_B_1_43 = ( real_T * ) ssGetOutputPortSignal ( S , 42 ) ; real_T * o_o_B_1_44 = ( real_T * ) ssGetOutputPortSignal ( S , 43 ) ; real_T * o_o_o_o_o_B_1_77 = ( real_T * ) ssGetOutputPortSignal ( S , 44 ) ; real_T * o_o_o_o_o_B_1_79 = ( real_T * ) ssGetOutputPortSignal ( S , 46 ) ; real_T * o_o_B_1_45 = ( real_T * ) ssGetOutputPortSignal ( S , 48 ) ; real_T * o_o_B_1_46 = ( real_T * ) ssGetOutputPortSignal ( S , 49 ) ; real_T * o_o_B_1_47 = ( real_T * ) ssGetOutputPortSignal ( S , 50 ) ; real_T * o_o_B_1_48 = ( real_T * ) ssGetOutputPortSignal ( S , 51 ) ; real_T * o_o_B_1_49 = ( real_T * ) ssGetOutputPortSignal ( S , 52 ) ; real_T * o_o_B_1_50 = ( real_T * ) ssGetOutputPortSignal ( S , 53 ) ; real_T * o_o_B_1_51 = ( real_T * ) ssGetOutputPortSignal ( S , 54 ) ; real_T * o_o_B_1_52 = ( real_T * ) ssGetOutputPortSignal ( S , 55 ) ; real_T * o_o_B_1_53 = ( real_T * ) ssGetOutputPortSignal ( S , 56 ) ; real_T * o_o_B_1_54 = ( real_T * ) ssGetOutputPortSignal ( S , 57 ) ; real_T * o_o_B_1_55 = ( real_T * ) ssGetOutputPortSignal ( S , 58 ) ; real_T * o_o_B_1_56 = ( real_T * ) ssGetOutputPortSignal ( S , 59 ) ; real_T * o_o_B_1_57 = ( real_T * ) ssGetOutputPortSignal ( S , 60 ) ; real_T * o_B_1_58 = ( real_T * ) ssGetOutputPortSignal ( S , 61 ) ; real_T * o_B_1_59 = ( real_T * ) ssGetOutputPortSignal ( S , 62 ) ; real_T * o_B_1_60 = ( real_T * ) ssGetOutputPortSignal ( S , 63 ) ; real_T * o_B_1_61 = ( real_T * ) ssGetOutputPortSignal ( S , 64 ) ; real_T * o_B_1_62 = ( real_T * ) ssGetOutputPortSignal ( S , 65 ) ; real_T * o_B_1_63 = ( real_T * ) ssGetOutputPortSignal ( S , 66 ) ; real_T * o_B_1_64 = ( real_T * ) ssGetOutputPortSignal ( S , 67 ) ; real_T * o_B_1_65 = ( real_T * ) ssGetOutputPortSignal ( S , 68 ) ; real_T * o_o_B_1_66 = ( real_T * ) ssGetOutputPortSignal ( S , 70 ) ; real_T * o_o_B_1_67 = ( real_T * ) ssGetOutputPortSignal ( S , 71 ) ; real_T * o_o_o_o_B_1_81 = ( real_T * ) ssGetOutputPortSignal ( S , 72 ) ; real_T * o_B_1_68 = ( real_T * ) ssGetOutputPortSignal ( S , 73 ) ; real_T * o_B_1_69 = ( real_T * ) ssGetOutputPortSignal ( S , 74 ) ; real_T * o_B_1_70 = ( real_T * ) ssGetOutputPortSignal ( S , 75 ) ; real_T * o_B_1_71 = ( real_T * ) ssGetOutputPortSignal ( S , 76 ) ; real_T * o_B_1_72 = ( real_T * ) ssGetOutputPortSignal ( S , 77 ) ; real_T * o_B_1_73 = ( real_T * ) ssGetOutputPortSignal ( S , 78 ) ; real_T * o_B_1_74 = ( real_T * ) ssGetOutputPortSignal ( S , 79 ) ; real_T * o_o_B_1_75 = ( real_T * ) ssGetOutputPortSignal ( S , 80 ) ; real_T * o_o_B_1_76 = ( real_T * ) ssGetOutputPortSignal ( S , 81 ) ; hdulqcnnpv * localX = ( hdulqcnnpv * ) ssGetContStates ( S ) ; o2ljf02bvi * localXdis = ( o2ljf02bvi * ) ssGetContStateDisabled ( S ) ; real_T * o_o_o_o_B_1_78 = ( real_T * ) ssGetOutputPortSignal ( S , 45 ) ; if ( tid == 0 ) { VehiclePlant ( & ( dw -> rtm ) , i_aqbxopp4od , i_gom2amlqr1 , i_dvujmtstvi , i_ar2tixbyms , i_pks5fmem4t , i_iuwyb4fljm , i_domdentxv3 , i_mvdqaplnnt , i_g45kr4jwpf , i_aibdphnll3 , i_k3dmq3ayz4 , i_evn3anml3h , i_nhq1rhohzj , i_bg0p24qife , o_B_1_1 , o_B_1_2 , o_B_1_3 , o_B_1_4 , o_B_1_5 , o_B_1_6 , o_B_1_7 , o_B_1_8 , o_B_1_9 , o_B_1_10 , o_B_1_11 , o_B_1_12 , o_B_1_13 , o_B_1_14 , o_B_1_15 , o_B_1_16 , o_B_1_17 , o_B_1_18 , o_B_1_19 , o_B_1_20 , o_B_1_21 , o_B_1_22 , o_B_1_23 , o_B_1_24 , o_B_1_25 , o_B_1_26 , o_B_1_27 , o_B_1_28 , o_B_1_29 , o_B_1_30 , o_B_1_31 , o_B_1_32 , o_B_1_33 , o_B_1_34 , o_o_B_1_35 , o_o_B_1_36 , o_o_B_1_37 , o_o_B_1_38 , o_o_B_1_39 , o_o_B_1_40 , o_o_o_B_1_41 , o_o_o_B_1_42 , o_o_B_1_43 , o_o_B_1_44 , o_o_o_o_o_B_1_77 , o_o_o_o_o_B_1_79 , o_o_B_1_45 , o_o_B_1_46 , o_o_B_1_47 , o_o_B_1_48 , o_o_B_1_49 , o_o_B_1_50 , o_o_B_1_51 , o_o_B_1_52 , o_o_B_1_53 , o_o_B_1_54 , o_o_B_1_55 , o_o_B_1_56 , o_o_B_1_57 , o_B_1_58 , o_B_1_59 , o_B_1_60 , o_B_1_61 , o_B_1_62 , o_B_1_63 , o_B_1_64 , o_B_1_65 , o_o_B_1_66 , o_o_B_1_67 , o_o_o_o_B_1_81 , o_B_1_68 , o_B_1_69 , o_B_1_70 , o_B_1_71 , o_B_1_72 , o_B_1_73 , o_B_1_74 , o_o_B_1_75 , o_o_B_1_76 , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX , localXdis ) ; bdatrumzy4 ( & ( dw -> rtm ) , o_o_B_1_35 , o_o_B_1_36 , o_o_B_1_37 , o_o_B_1_38 , o_o_B_1_39 , o_o_B_1_40 , o_o_o_B_1_41 , o_o_o_B_1_42 , o_o_B_1_43 , o_o_B_1_44 , o_o_o_o_o_B_1_77 , o_o_o_o_B_1_78 , o_o_o_o_o_B_1_79 , o_o_B_1_45 , o_o_B_1_46 , o_o_B_1_47 , o_o_B_1_48 , o_o_B_1_49 , o_o_B_1_50 , o_o_B_1_51 , o_o_B_1_52 , o_o_B_1_53 , o_o_B_1_54 , o_o_B_1_55 , o_o_B_1_56 , o_o_B_1_57 , o_o_B_1_75 , o_o_B_1_76 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; } } static void mdlInitializeSizes ( SimStruct * S ) { if ( ( S -> mdlInfo -> genericFcn != ( NULL ) ) && ( ! ( S -> mdlInfo -> genericFcn ) ( S , GEN_FCN_CHK_MODELREF_SFUN_HAS_MODEL_BLOCK , - 1 , ( NULL ) ) ) ) { return ; } ssSetNumSFcnParams ( S , 0 ) ; ssFxpSetU32BitRegionCompliant ( S , 1 ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; } ssSetRTWGeneratedSFcn ( S , 2 ) ; ssSetNumContStates ( S , 47 ) ; ssSetNumDiscStates ( S , 0 ) ; ssSetNumPeriodicContStates ( S , 0 ) ; ssSetSymbolicDimsSupport ( S , true ) ; slmrInitializeIOPortDataVectors ( S , 14 , 82 ) ; if ( ! ssSetNumInputPorts ( S , 14 ) ) return ; if ( ! ssSetInputPortVectorDimension ( S , 0 , 1 ) ) return ; ssSetInputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ; ssSetInputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 0 , SS_BOOLEAN ) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
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
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 1 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
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
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 2 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
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
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 3 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
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
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 4 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
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
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 5 , SS_BOOLEAN )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
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
( S , 8 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 9 , 1 ) ) return
; ssSetInputPortDimensionsMode ( S , 9 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 9 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 9 , SS_DOUBLE ) ;
} if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 9 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 9 , 1 ) ;
ssSetInputPortRequiredContiguous ( S , 9 , 1 ) ; ssSetInputPortOptimOpts ( S
, 9 , SS_NOT_REUSABLE_AND_GLOBAL ) ; ssSetInputPortOverWritable ( S , 9 ,
false ) ; ssSetInputPortSampleTime ( S , 9 , 0.0 ) ; ssSetInputPortOffsetTime
( S , 9 , 0.0 ) ; if ( ! ssSetInputPortVectorDimension ( S , 10 , 1 ) )
return ; ssSetInputPortDimensionsMode ( S , 10 , FIXED_DIMS_MODE ) ;
ssSetInputPortFrameData ( S , 10 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetInputPortDataType ( S , 10 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetInputPortUnit ( S , 10 , unitIdReg ) ;
#endif
} ssSetInputPortDirectFeedThrough ( S , 10 , 1 ) ;
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
} ssSetInputPortDirectFeedThrough ( S , 11 , 1 ) ;
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
} ssSetInputPortDirectFeedThrough ( S , 12 , 1 ) ;
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
ssSetInputPortOffsetTime ( S , 13 , 0.0 ) ; if ( ! ssSetNumOutputPorts ( S ,
82 ) ) return ; if ( ! ssSetOutputPortVectorDimension ( S , 0 , 1 ) ) return
; ssSetOutputPortDimensionsMode ( S , 0 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 0 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 0 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 0 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 0 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 0 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 0 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 0 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 0 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 0 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 1 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 1 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 1 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 1 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 1 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 1 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 1 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 1 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 1 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 1 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 1 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 2 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 2 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 2 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 2 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 2 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 2 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 2 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 2 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 2 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 2 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 2 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 3 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 3 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 3 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 3 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 3 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 3 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 3 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 3 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 3 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 3 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 3 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 4 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 4 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 4 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 4 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 4 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 4 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 4 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 4 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 4 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 4 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 4 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 5 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 5 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 5 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 5 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 5 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 5 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 5 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 5 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 5 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 5 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 5 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 6 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 6 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 6 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 6 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 6 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 6 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 6 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 6 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 6 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 6 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 6 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 7 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 7 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 7 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 7 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 7 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 7 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 7 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 7 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 7 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 7 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 7 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 8 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 8 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 8 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 8 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 8 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 8 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 8 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 8 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 8 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 8 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 8 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 9 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 9 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 9 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 9 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 9 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 9 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 9 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 9 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 9 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 9 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 9 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 10 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 10 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 10 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 10 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 10 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 10 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 10 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 10 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 10 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 10 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 10 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 11 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 11 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 11 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 11 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 11 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 11 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 11 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 11 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 11 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 11 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 11 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 12 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 12 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 12 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 12 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 12 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 12 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 12 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 12 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 12 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 12 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 12 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 13 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 13 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 13 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 13 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 13 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 13 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 13 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 13 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 13 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 13 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 13 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 14 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 14 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 14 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 14 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 14 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 14 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 14 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 14 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 14 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 14 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 14 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 15 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 15 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 15 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 15 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 15 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 15 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 15 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 15 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 15 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 15 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 15 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 16 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 16 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 16 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 16 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 16 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 16 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 16 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 16 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 16 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 16 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 16 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 17 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 17 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 17 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 17 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 17 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 17 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 17 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 17 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 17 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 17 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 17 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 18 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 18 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 18 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 18 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 18 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 18 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 18 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 18 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 18 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 18 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 18 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 19 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 19 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 19 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 19 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 19 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 19 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 19 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 19 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 19 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 19 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 19 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 20 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 20 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 20 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 20 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 20 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 20 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 20 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 20 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 20 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 20 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 20 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 21 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 21 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 21 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 21 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 21 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 21 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 21 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 21 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 21 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 21 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 21 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 22 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 22 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 22 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 22 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 22 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 22 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 22 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 22 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 22 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 22 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 22 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 23 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 23 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 23 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 23 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 23 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 23 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 23 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 23 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 23 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 23 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 23 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 24 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 24 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 24 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 24 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 24 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 24 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 24 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 24 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 24 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 24 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 24 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
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
ssSetOutputPortOkToMerge ( S , 25 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 25 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 25 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
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
ssSetOutputPortOkToMerge ( S , 26 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 26 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 26 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 27 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 27 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 27 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 27 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 27 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 27 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 27 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 27 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 27 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 27 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 27 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 28 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 28 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 28 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 28 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 28 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 28 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 28 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 28 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 28 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 28 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 28 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 29 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 29 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 29 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 29 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 29 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 29 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 29 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 29 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 29 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 29 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 29 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 30 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 30 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 30 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 30 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 30 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 30 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 30 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 30 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 30 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 30 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 30 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 31 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 31 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 31 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 31 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 31 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 31 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 31 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 31 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 31 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 31 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 31 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 32 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 32 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 32 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 32 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 32 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 32 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 32 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 32 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 32 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 32 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 32 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 33 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 33 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 33 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 33 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 33 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 33 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 33 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 33 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 33 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 33 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 33 , SS_NOT_REUSABLE_AND_LOCAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 34 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 34 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 34 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 34 , SS_BOOLEAN
) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 34 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 34 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 34 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 34 , 1 ) ;
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
ssSetOutputPortOptimOpts ( S , 35 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 36 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 36 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 36 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 36 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 36 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 36 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 36 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 36 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 36 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 36 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 36 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 37 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 37 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 37 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 37 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 37 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 37 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 37 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 37 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 37 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 37 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 37 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 38 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 38 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 38 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 38 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 38 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 38 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 38 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 38 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 38 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 38 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 38 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 39 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 39 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 39 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 39 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 39 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 39 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 39 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 39 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 39 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 39 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 39 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 40 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 40 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 40 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 40 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 40 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 40 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 40 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 40 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 40 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 40 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 40 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 41 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 41 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 41 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 41 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 41 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 41 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 41 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 41 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 41 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 41 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 41 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortMatrixDimensions ( S , 42 , 3 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 42 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 42 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 42 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 42 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 42 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 42 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 42 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 42 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 42 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 42 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 43 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 43 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 43 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 43 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 43 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 43 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 43 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 43 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 43 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 43 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 43 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 44 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 44 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 44 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 44 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 44 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 44 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 44 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 44 , 0 ) ; ssSetOutputPortOkToMerge
( S , 44 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 44 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 44 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 45
, 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 45 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 45 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 45 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 45 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 45 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 45 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 45 , 0 ) ; ssSetOutputPortOkToMerge
( S , 45 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 45 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 45 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortMatrixDimensions ( S ,
46 , 3 , 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 46 ,
FIXED_DIMS_MODE ) ; ssSetOutputPortFrameData ( S , 46 , FRAME_NO ) ; if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 46 , SS_DOUBLE ) ; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 46 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 46 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 46 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 46 , 0 ) ; ssSetOutputPortOkToMerge
( S , 46 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 46 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 46 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 47
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 47 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 47 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 47 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 47 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 47 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 47 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 47 , 0 ) ; ssSetOutputPortOkToMerge
( S , 47 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 47 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 47 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 48
, 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 48 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 48 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 48 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 48 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 48 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 48 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 48 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 48 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 48 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 48 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 49 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 49 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 49 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 49 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 49 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 49 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 49 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 49 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 49 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 49 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 49 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 50 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 50 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 50 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 50 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 50 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 50 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 50 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 50 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 50 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 50 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 50 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 51 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 51 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 51 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 51 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 51 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 51 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 51 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 51 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 51 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 51 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 51 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 52 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 52 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 52 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 52 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 52 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 52 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 52 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 52 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 52 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 52 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 52 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 53 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 53 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 53 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 53 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 53 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 53 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 53 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 53 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 53 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 53 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 53 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 54 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 54 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 54 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 54 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 54 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 54 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 54 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 54 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 54 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 54 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 54 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 55 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 55 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 55 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 55 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 55 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 55 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 55 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 55 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 55 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 55 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 55 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 56 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 56 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 56 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 56 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 56 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 56 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 56 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 56 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 56 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 56 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 56 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 57 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 57 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 57 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 57 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 57 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 57 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 57 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 57 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 57 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 57 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 57 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 58 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 58 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 58 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 58 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 58 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 58 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 58 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 58 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 58 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 58 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 58 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 59 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 59 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 59 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 59 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 59 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 59 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 59 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 59 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 59 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 59 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 59 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 60 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 60 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 60 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 60 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 60 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 60 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 60 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 60 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 60 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 60 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 60 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 61 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 61 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 61 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 61 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 61 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 61 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 61 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 61 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 61 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 61 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 61 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 62 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 62 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 62 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 62 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 62 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 62 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 62 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 62 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 62 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 62 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 62 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 63 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 63 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 63 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 63 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 63 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 63 , 0.004 ) ; ssSetOutputPortOffsetTime ( S
, 63 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 63 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 63 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 63 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 63 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 64 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 64 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 64 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 64 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 64 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 64 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 64 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 64 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 64 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 64 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 64 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 65 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 65 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 65 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 65 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 65 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 65 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 65 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 65 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 65 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 65 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 65 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 66 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 66 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 66 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 66 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 66 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 66 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 66 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 66 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 66 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 66 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 66 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 67 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 67 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 67 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 67 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 67 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 67 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 67 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 67 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 67 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 67 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 67 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 68 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 68 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 68 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 68 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 68 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 68 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 68 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 68 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 68 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 68 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 68 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 69 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 69 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 69 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 69 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 69 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 69 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 69 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 69 , 0 ) ; ssSetOutputPortOkToMerge
( S , 69 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 69 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 69 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 70
, 3 ) ) return ; ssSetOutputPortDimensionsMode ( S , 70 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 70 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 70 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 70 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 70 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 70 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 70 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 70 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 70 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 70 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 71 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 71 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 71 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 71 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 71 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 71 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 71 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 71 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 71 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 71 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 71 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 72 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 72 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 72 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 72 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 72 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 72 , mxGetInf ( ) ) ;
ssSetOutputPortOffsetTime ( S , 72 , 0 ) ;
ssSetOutputPortDiscreteValuedOutput ( S , 72 , 0 ) ; ssSetOutputPortOkToMerge
( S , 72 , SS_NOT_OK_TO_MERGE ) ; ssSetOutputPortICAttributes ( S , 72 ,
false , false , false ) ; ssSetOutputPortOptimOpts ( S , 72 ,
SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( ! ssSetOutputPortVectorDimension ( S , 73
, 1 ) ) return ; ssSetOutputPortDimensionsMode ( S , 73 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 73 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 73 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 73 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 73 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 73 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 73 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 73 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 73 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 73 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 74 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 74 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 74 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 74 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 74 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 74 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 74 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 74 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 74 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 74 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 74 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 75 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 75 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 75 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 75 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 75 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 75 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 75 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 75 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 75 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 75 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 75 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 76 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 76 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 76 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 76 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 76 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 76 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 76 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 76 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 76 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 76 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 76 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 77 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 77 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 77 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 77 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 77 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 77 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 77 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 77 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 77 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 77 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 77 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 78 , 1 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 78 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 78 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 78 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 78 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 78 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 78 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 78 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 78 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 78 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 78 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 79 , 6 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 79 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 79 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 79 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 79 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 79 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 79 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 79 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 79 , SS_OK_TO_MERGE_CONDITIONAL ) ;
ssSetOutputPortICAttributes ( S , 79 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 79 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 80 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 80 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 80 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 80 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 80 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 80 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 80 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 80 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 80 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 80 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 80 , SS_NOT_REUSABLE_AND_GLOBAL ) ; if ( !
ssSetOutputPortVectorDimension ( S , 81 , 3 ) ) return ;
ssSetOutputPortDimensionsMode ( S , 81 , FIXED_DIMS_MODE ) ;
ssSetOutputPortFrameData ( S , 81 , FRAME_NO ) ; if ( ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { ssSetOutputPortDataType ( S , 81 , SS_DOUBLE )
; } if ( ssGetSimMode ( S ) != SS_SIMMODE_SIZES_CALL_ONLY ) {
#if defined (MATLAB_MEX_FILE)
UnitId unitIdReg ; ssRegisterUnitFromExpr ( S , "" , & unitIdReg ) ; if ( unitIdReg == INVALID_UNIT_ID ) return ; ssSetOutputPortUnit ( S , 81 , unitIdReg ) ;
#endif
} ssSetOutputPortSampleTime ( S , 81 , 0.0 ) ; ssSetOutputPortOffsetTime ( S
, 81 , 0.0 ) ; ssSetOutputPortDiscreteValuedOutput ( S , 81 , 0 ) ;
ssSetOutputPortOkToMerge ( S , 81 , SS_NOT_OK_TO_MERGE ) ;
ssSetOutputPortICAttributes ( S , 81 , false , false , false ) ;
ssSetOutputPortOptimOpts ( S , 81 , SS_NOT_REUSABLE_AND_GLOBAL ) ;
ssSetSimStateCompliance ( S , USE_CUSTOM_SIM_STATE ) ;
mr_VehiclePlant_RegisterSimStateChecksum ( S ) ; ssSetNumSampleTimes ( S , 5
) ; ssSetParameterTuningCompliance ( S , true ) ; ssSetNumRWork ( S , 0 ) ;
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
ssSetOutputPortIsNonContinuous ( S , 15 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 15 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 16 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 16 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 17 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 17 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 18 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 18 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 19 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 19 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 20 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 20 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 21 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 21 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 22 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 22 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 23 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 23 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 24 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 24 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 25 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 25 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 26 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 26 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 27 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 27 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 28 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 28 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 29 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 29 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 30 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 30 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 31 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 31 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 32 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 32 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 33 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 33 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 34 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 34 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 35 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 35 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 36 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 36 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 37 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 37 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 38 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 38 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 39 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 39 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 40 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 40 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 41 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 41 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 42 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 42 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 43 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 43 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 44 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 44 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 45 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 45 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 46 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 46 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 47 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 47 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 48 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 48 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 49 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 49 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 50 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 50 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 51 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 51 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 52 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 52 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 53 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 53 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 54 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 54 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 55 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 55 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 56 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 56 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 57 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 57 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 58 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 58 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 59 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 59 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 60 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 60 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 61 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 61 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 62 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 62 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 63 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 63 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 64 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 64 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 65 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 65 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 66 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 66 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 67 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 67 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 68 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 68 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 69 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 69 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 70 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 70 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 71 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 71 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 72 , 0 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 72 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 73 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 73 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 74 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 74 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 75 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 75 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 76 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 76 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 77 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 77 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 78 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 78 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 79 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 79 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 80 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 80 , 0 ) ;
ssSetOutputPortIsNonContinuous ( S , 81 , 1 ) ;
ssSetOutputPortIsFedByBlockWithModesNoZCs ( S , 81 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 0 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 1 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 2 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 3 , 1 ) ; ssSetInputPortIsNotDerivPort ( S
, 4 , 1 ) ; ssSetInputPortIsNotDerivPort ( S , 5 , 1 ) ;
ssSetInputPortIsNotDerivPort ( S , 6 , 0 ) ; ssSetInputPortIsNotDerivPort ( S
, 7 , 0 ) ; ssSetInputPortIsNotDerivPort ( S , 8 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 9 , 0 ) ; ssSetInputPortIsNotDerivPort ( S
, 10 , 0 ) ; ssSetInputPortIsNotDerivPort ( S , 11 , 0 ) ;
ssSetInputPortIsNotDerivPort ( S , 12 , 0 ) ; ssSetInputPortIsNotDerivPort ( S
, 13 , 1 ) ; ssSetModelReferenceSampleTimeInheritanceRule ( S ,
DISALLOW_SAMPLE_TIME_INHERITANCE ) ; ssSetAcceptsFcnCallInputs ( S ) ;
ssSetModelReferenceNormalModeSupport ( S ,
MDL_START_AND_MDL_PROCESS_PARAMS_OK ) ; ssSupportsMultipleExecInstances ( S ,
false ) ; ssRegisterMsgForNotSupportingMultiExecInst ( S ,
 "<diag_root><diag id=\"Simulink:blocks:BlockDoesNotSupportMultiExecInstancesWithCustomMsg\" pr=\"d\"><arguments><arg type=\"encoded\">VgBlAGgAaQBjAGwAZQBQAGwAYQBuAHQALwBlAG4AdgBpAHIAbwBuAG0AZQBuAHQAAAA=</arg><arg type=\"encoded\">PABfAF8AaQBpAFMAUwBfAF8APgA8AC8AXwBfAGkAaQBTAFMAXwBfAD4AAAA=</arg><arg type=\"encoded\">PABfAF8AaQB0AGUAcgBCAGwAawBfAF8APgA8AC8AXwBfAGkAdABlAHIAQgBsAGsAXwBfAD4AAAA=</arg></arguments><hs><h>AAAABgAp30A=</h></hs><causes><diag id=\"Simulink:blocks:BlockDoesNotSupportMultiExecInstancesWithCustomMsg\" pr=\"d\"><arguments><arg type=\"encoded\">ZQBuAHYAaQByAG8AbgBtAGUAbgB0AC8AZQBhAHIAdABoAAAA</arg><arg type=\"encoded\">PABfAF8AaQBpAFMAUwBfAF8APgA8AC8AXwBfAGkAaQBTAFMAXwBfAD4AAAA=</arg><arg type=\"encoded\">PABfAF8AaQB0AGUAcgBCAGwAawBfAF8APgA8AC8AXwBfAGkAdABlAHIAQgBsAGsAXwBfAD4AAAA=</arg></arguments><hs><h>AAAAHACczEA=</h></hs><causes><diag id=\"Simulink:blocks:BlockDoesNotSupportMultiExecInstances\" pr=\"d\"><arguments><arg type=\"encoded\">RQBhAHIAdABoAC8AVwBHAFMAOAA0ACAARwByAGEAdgBpAHQAeQAgAE0AbwBkAGUAbAAgACAAAAA=</arg><arg type=\"encoded\">PABfAF8AaQBpAFMAUwBfAF8APgA8AC8AXwBfAGkAaQBTAFMAXwBfAD4AAAA=</arg><arg type=\"encoded\">PABfAF8AaQB0AGUAcgBCAGwAawBfAF8APgA8AC8AXwBfAGkAdABlAHIAQgBsAGsAXwBfAD4AAAA=</arg></arguments><hs><h>AAAAHACczEA=</h></hs></diag></causes></diag></causes></diag></diag_root>"
) ; ssHasStateInsideForEachSS ( S , true ) ; ssSetOptions ( S ,
SS_OPTION_ALLOW_CONSTANT_PORT_SAMPLE_TIME |
SS_OPTION_PORT_SAMPLE_TIMES_ASSIGNED | SS_OPTION_SUPPORTS_ALIAS_DATA_TYPES |
SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME | SS_OPTION_EXCEPTION_FREE_CODE |
SS_OPTION_WORKS_WITH_CODE_REUSE ) ;
#if SS_SFCN_FOR_SIM
if ( S -> mdlInfo -> genericFcn != ( NULL ) && ssGetSimMode ( S ) !=
SS_SIMMODE_SIZES_CALL_ONLY ) { int_T retVal = 1 ;
mr_VehiclePlant_MdlInfoRegFcn ( S , "VehiclePlant" , & retVal ) ; if ( !
retVal ) return ; }
#endif
#if SS_SFCN_FOR_SIM
if ( ssSetNumDWork ( S , 1 ) ) { int mdlrefDWTypeId ; ssRegMdlRefDWorkType ( S
, & mdlrefDWTypeId ) ; if ( mdlrefDWTypeId == INVALID_DTYPE_ID ) return ; if
( ! ssSetDataTypeSize ( S , mdlrefDWTypeId , sizeof ( owiw5mmjy2f ) ) )
return ; ssSetDWorkDataType ( S , 0 , mdlrefDWTypeId ) ; ssSetDWorkWidth ( S
, 0 , 1 ) ; }
#else
if ( ! ssSetNumDWork ( S , 1 ) ) { return ; }
#endif
slmrRegisterSystemInitializeMethod ( S , mdlInitializeConditions ) ;
slmrRegisterSystemResetMethod ( S , mdlReset ) ;
slmrRegisterPeriodicOutputUpdateMethod ( S , mdlPeriodicOutputUpdate ) ;
ssSetSimulinkVersionGeneratedIn ( S , "26.1" ) ; ssSetNeedAbsoluteTime ( S ,
1 ) ; } static void mdlInitializeSampleTimes ( SimStruct * S ) {
ssSetSampleTime ( S , 0 , 0 ) ; ssSetOffsetTime ( S , 0 , 0 ) ;
ssSetSampleTime ( S , 1 , 0.004 ) ; ssSetOffsetTime ( S , 1 , 0 ) ;
ssSetSampleTime ( S , 2 , 0.1 ) ; ssSetOffsetTime ( S , 2 , 0 ) ;
ssSetSampleTime ( S , 3 , mxGetInf ( ) ) ; ssSetOffsetTime ( S , 3 , 0 ) ;
ssSetSampleTime ( S , 4 , rtInf ) ; ssSetOffsetTime ( S , 4 , rtInf ) ;
return ; }
#define MDL_SET_WORK_WIDTHS
static void mdlSetWorkWidths ( SimStruct * S ) { if ( S -> mdlInfo ->
genericFcn != ( NULL ) ) { ssSetSignalSizesComputeType ( S ,
SS_VARIABLE_SIZE_FROM_INPUT_VALUE_AND_SIZE ) ; } { static const char *
toFileNames [ ] = { "" } ; static const char * fromFileNames [ ] = { "" } ;
if ( ! ssSetModelRefFromFiles ( S , 0 , fromFileNames ) ) return ; if ( !
ssSetModelRefToFiles ( S , 0 , toFileNames ) ) return ; } }
#define MDL_SETUP_RUNTIME_RESOURCES
static void mdlSetupRuntimeResources ( SimStruct * S ) { owiw5mmjy2f * dw = ( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_o_B_1_77 = ( real_T * ) ssGetOutputPortSignal ( S , 44 ) ; real_T * o_o_o_o_B_1_78 = ( real_T * ) ssGetOutputPortSignal ( S , 45 ) ; real_T * o_o_o_o_o_B_1_79 = ( real_T * ) ssGetOutputPortSignal ( S , 46 ) ; real_T * o_B_1_58 = ( real_T * ) ssGetOutputPortSignal ( S , 61 ) ; real_T * o_B_1_59 = ( real_T * ) ssGetOutputPortSignal ( S , 62 ) ; real_T * o_B_1_60 = ( real_T * ) ssGetOutputPortSignal ( S , 63 ) ; real_T * o_B_1_80 = ( real_T * ) ssGetOutputPortSignal ( S , 69 ) ; real_T * o_o_o_o_B_1_81 = ( real_T * ) ssGetOutputPortSignal ( S , 72 ) ; hdulqcnnpv * localX = ( hdulqcnnpv * ) ssGetContStates ( S ) ; ssNonContDerivSigFeedingOutports mr_nonContOutput35 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput36 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput37 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput38 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput39 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput40 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput41 [ 18 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput42 [ 17 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput43 [ 18 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput48 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput49 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput50 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput51 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput52 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput53 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput54 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput55 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput56 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput57 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput64 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput65 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput66 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput67 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput68 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput70 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput71 [ 25 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput73 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput74 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput75 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput76 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput77 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput78 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput79 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput80 [ 13 ] ; ssNonContDerivSigFeedingOutports mr_nonContOutput81 [ 13 ] ; ssNonContDerivSigFeedingOutports * mr_nonContOutputArray [ 82 ] ; void * sysRanPtr = ( NULL ) ; int_T contextTid = 0 ; ssGetContextSysRanBCPtr ( S , & sysRanPtr ) ; ssGetContextSysTid ( S , & contextTid ) ; if ( contextTid == CONSTANT_TID ) { contextTid = 0 ; } mr_nonContOutputArray [ 0 ] = ( NULL ) ; mr_nonContOutputArray [ 1 ] = ( NULL ) ; mr_nonContOutputArray [ 2 ] = ( NULL ) ; mr_nonContOutputArray [ 3 ] = ( NULL ) ; mr_nonContOutputArray [ 4 ] = ( NULL ) ; mr_nonContOutputArray [ 5 ] = ( NULL ) ; mr_nonContOutputArray [ 6 ] = ( NULL ) ; mr_nonContOutputArray [ 7 ] = ( NULL ) ; mr_nonContOutputArray [ 8 ] = ( NULL ) ; mr_nonContOutputArray [ 9 ] = ( NULL ) ; mr_nonContOutputArray [ 10 ] = ( NULL ) ; mr_nonContOutputArray [ 11 ] = ( NULL ) ; mr_nonContOutputArray [ 12 ] = ( NULL ) ; mr_nonContOutputArray [ 13 ] = ( NULL ) ; mr_nonContOutputArray [ 14 ] = ( NULL ) ; mr_nonContOutputArray [ 15 ] = ( NULL ) ; mr_nonContOutputArray [ 16 ] = ( NULL ) ; mr_nonContOutputArray [ 17 ] = ( NULL ) ; mr_nonContOutputArray [ 18 ] = ( NULL ) ; mr_nonContOutputArray [ 19 ] = ( NULL ) ; mr_nonContOutputArray [ 20 ] = ( NULL ) ; mr_nonContOutputArray [ 21 ] = ( NULL ) ; mr_nonContOutputArray [ 22 ] = ( NULL ) ; mr_nonContOutputArray [ 23 ] = ( NULL ) ; mr_nonContOutputArray [ 24 ] = ( NULL ) ; mr_nonContOutputArray [ 25 ] = ( NULL ) ; mr_nonContOutputArray [ 26 ] = ( NULL ) ; mr_nonContOutputArray [ 27 ] = ( NULL ) ; mr_nonContOutputArray [ 28 ] = ( NULL ) ; mr_nonContOutputArray [ 29 ] = ( NULL ) ; mr_nonContOutputArray [ 30 ] = ( NULL ) ; mr_nonContOutputArray [ 31 ] = ( NULL ) ; mr_nonContOutputArray [ 32 ] = ( NULL ) ; mr_nonContOutputArray [ 33 ] = ( NULL ) ; mr_nonContOutputArray [ 34 ] = ( NULL ) ; mr_nonContOutputArray [ 35 ] = mr_nonContOutput35 ; mr_nonContOutputArray [ 36 ] = mr_nonContOutput36 ; mr_nonContOutputArray [ 37 ] = mr_nonContOutput37 ; mr_nonContOutputArray [ 38 ] = mr_nonContOutput38 ; mr_nonContOutputArray [ 39 ] = mr_nonContOutput39 ; mr_nonContOutputArray [ 40 ] = mr_nonContOutput40 ; mr_nonContOutputArray [ 41 ] = mr_nonContOutput41 ; mr_nonContOutputArray [ 42 ] = mr_nonContOutput42 ; mr_nonContOutputArray [ 43 ] = mr_nonContOutput43 ; mr_nonContOutputArray [ 44 ] = ( NULL ) ; mr_nonContOutputArray [ 45 ] = ( NULL ) ; mr_nonContOutputArray [ 46 ] = ( NULL ) ; mr_nonContOutputArray [ 47 ] = ( NULL ) ; mr_nonContOutputArray [ 48 ] = mr_nonContOutput48 ; mr_nonContOutputArray [ 49 ] = mr_nonContOutput49 ; mr_nonContOutputArray [ 50 ] = mr_nonContOutput50 ; mr_nonContOutputArray [ 51 ] = mr_nonContOutput51 ; mr_nonContOutputArray [ 52 ] = mr_nonContOutput52 ; mr_nonContOutputArray [ 53 ] = mr_nonContOutput53 ; mr_nonContOutputArray [ 54 ] = mr_nonContOutput54 ; mr_nonContOutputArray [ 55 ] = mr_nonContOutput55 ; mr_nonContOutputArray [ 56 ] = mr_nonContOutput56 ; mr_nonContOutputArray [ 57 ] = mr_nonContOutput57 ; mr_nonContOutputArray [ 58 ] = ( NULL ) ; mr_nonContOutputArray [ 59 ] = ( NULL ) ; mr_nonContOutputArray [ 60 ] = ( NULL ) ; mr_nonContOutputArray [ 61 ] = ( NULL ) ; mr_nonContOutputArray [ 62 ] = ( NULL ) ; mr_nonContOutputArray [ 63 ] = ( NULL ) ; mr_nonContOutputArray [ 64 ] = mr_nonContOutput64 ; mr_nonContOutputArray [ 65 ] = mr_nonContOutput65 ; mr_nonContOutputArray [ 66 ] = mr_nonContOutput66 ; mr_nonContOutputArray [ 67 ] = mr_nonContOutput67 ; mr_nonContOutputArray [ 68 ] = mr_nonContOutput68 ; mr_nonContOutputArray [ 69 ] = ( NULL ) ; mr_nonContOutputArray [ 70 ] = mr_nonContOutput70 ; mr_nonContOutputArray [ 71 ] = mr_nonContOutput71 ; mr_nonContOutputArray [ 72 ] = ( NULL ) ; mr_nonContOutputArray [ 73 ] = mr_nonContOutput73 ; mr_nonContOutputArray [ 74 ] = mr_nonContOutput74 ; mr_nonContOutputArray [ 75 ] = mr_nonContOutput75 ; mr_nonContOutputArray [ 76 ] = mr_nonContOutput76 ; mr_nonContOutputArray [ 77 ] = mr_nonContOutput77 ; mr_nonContOutputArray [ 78 ] = mr_nonContOutput78 ; mr_nonContOutputArray [ 79 ] = mr_nonContOutput79 ; mr_nonContOutputArray [ 80 ] = mr_nonContOutput80 ; mr_nonContOutputArray [ 81 ] = mr_nonContOutput81 ; simTgtAllocOPModelData ( S ) ; dwjsblzuar ( o_o_o_o_o_B_1_77 , o_o_o_o_B_1_78 , o_o_o_o_o_B_1_79 , o_B_1_58 , o_B_1_59 , o_B_1_60 , o_B_1_80 , o_o_o_o_B_1_81 , S , mr_nonContOutputArray , slmrGetTopTidFromMdlRefChildTid ( S , 0 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 1 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 2 , false ) , slmrGetTopTidFromMdlRefChildTid ( S , 3 , true ) , & ( dw -> rtm ) , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX , sysRanPtr , contextTid , ( ( NULL ) ) , ( ( NULL ) ) , 0 , - 1 ) ; ssSetModelMappingInfoPtr ( S , & ( dw -> rtm . DataMapInfo . mmi ) ) ; if ( S -> mdlInfo -> genericFcn != ( NULL ) ) { _GenericFcn fcn = S -> mdlInfo -> genericFcn ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 35 , mr_nonContOutput35 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 36 , mr_nonContOutput36 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 37 , mr_nonContOutput37 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 38 , mr_nonContOutput38 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 39 , mr_nonContOutput39 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 40 , mr_nonContOutput40 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 41 , mr_nonContOutput41 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 42 , mr_nonContOutput42 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 43 , mr_nonContOutput43 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 48 , mr_nonContOutput48 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 49 , mr_nonContOutput49 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 50 , mr_nonContOutput50 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 51 , mr_nonContOutput51 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 52 , mr_nonContOutput52 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 53 , mr_nonContOutput53 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 54 , mr_nonContOutput54 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 55 , mr_nonContOutput55 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 56 , mr_nonContOutput56 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 57 , mr_nonContOutput57 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 64 , mr_nonContOutput64 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 65 , mr_nonContOutput65 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 66 , mr_nonContOutput66 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 67 , mr_nonContOutput67 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 68 , mr_nonContOutput68 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 70 , mr_nonContOutput70 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 71 , mr_nonContOutput71 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 73 , mr_nonContOutput73 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 74 , mr_nonContOutput74 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 75 , mr_nonContOutput75 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 76 , mr_nonContOutput76 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 77 , mr_nonContOutput77 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 78 , mr_nonContOutput78 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 79 , mr_nonContOutput79 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 80 , mr_nonContOutput80 ) ) return ; if ( ! ( fcn ) ( S , GEN_FCN_REG_MODELREF_NONCONTSIGS , 81 , mr_nonContOutput81 ) ) return ; } gyzb3lqody ( & ( dw -> rtm ) , & ( dw -> rtdw ) ) ; }
#define MDL_START
static void mdlStart ( SimStruct * S ) { owiw5mmjy2f * dw = ( owiw5mmjy2f * )
ssGetDWork ( S , 0 ) ; real_T * o_o_o_o_o_B_1_77 = ( real_T * )
ssGetOutputPortSignal ( S , 44 ) ; real_T * o_o_o_o_B_1_78 = ( real_T * )
ssGetOutputPortSignal ( S , 45 ) ; real_T * o_o_o_o_o_B_1_79 = ( real_T * )
ssGetOutputPortSignal ( S , 46 ) ; real_T * o_o_o_o_B_1_81 = ( real_T * )
ssGetOutputPortSignal ( S , 72 ) ; mdlProcessParameters ( S ) ; hqrg4vgws5 ( o_o_o_o_o_B_1_77 , o_o_o_o_B_1_78 , o_o_o_o_o_B_1_79 , o_o_o_o_B_1_81 , & ( dw -> rtdw ) ) ; }
#define RTW_GENERATED_DISABLE
static void mdlDisable ( SimStruct * S ) { owiw5mmjy2f * dw = ( owiw5mmjy2f *
) ssGetDWork ( S , 0 ) ; iv0fdu4uqg ( & ( dw -> rtdw ) ) ; return ; } static
void mdlOutputs ( SimStruct * S , int_T tid ) { owiw5mmjy2f * dw = ( owiw5mmjy2f
* ) ssGetDWork ( S , 0 ) ; boolean_T const * i_aqbxopp4od = ( boolean_T * )
ssGetInputPortSignal ( S , 0 ) ; boolean_T const * i_gom2amlqr1 = ( boolean_T
* ) ssGetInputPortSignal ( S , 1 ) ; boolean_T const * i_dvujmtstvi = ( boolean_T * ) ssGetInputPortSignal ( S , 2 ) ; boolean_T const * i_ar2tixbyms = ( boolean_T * ) ssGetInputPortSignal ( S , 3 ) ; boolean_T const * i_pks5fmem4t = ( boolean_T * ) ssGetInputPortSignal ( S , 4 ) ; boolean_T const * i_iuwyb4fljm = ( boolean_T * ) ssGetInputPortSignal ( S , 5 ) ; real_T const * i_domdentxv3 = ( real_T * ) ssGetInputPortSignal ( S , 6 ) ; real_T const * i_mvdqaplnnt = ( real_T * ) ssGetInputPortSignal ( S , 7 ) ; real_T const * i_g45kr4jwpf = ( real_T * ) ssGetInputPortSignal ( S , 8 ) ; real_T const * i_aibdphnll3 = ( real_T * ) ssGetInputPortSignal ( S , 9 ) ; real_T const * i_k3dmq3ayz4 = ( real_T * ) ssGetInputPortSignal ( S , 10 ) ; real_T const * i_evn3anml3h = ( real_T * ) ssGetInputPortSignal ( S , 11 ) ; real_T const * i_nhq1rhohzj = ( real_T * ) ssGetInputPortSignal ( S , 12 ) ; real_T const * i_bg0p24qife = ( real_T * ) ssGetInputPortSignal ( S , 13 ) ; real_T * o_B_1_1 = ( real_T * ) ssGetOutputPortSignal ( S , 0 ) ; real_T * o_B_1_2 = ( real_T * ) ssGetOutputPortSignal ( S , 1 ) ; real_T * o_B_1_3 = ( real_T * ) ssGetOutputPortSignal ( S , 2 ) ; real_T * o_B_1_4 = ( real_T * ) ssGetOutputPortSignal ( S , 3 ) ; real_T * o_B_1_5 = ( real_T * ) ssGetOutputPortSignal ( S , 4 ) ; real_T * o_B_1_6 = ( real_T * ) ssGetOutputPortSignal ( S , 5 ) ; real_T * o_B_1_7 = ( real_T * ) ssGetOutputPortSignal ( S , 6 ) ; real_T * o_B_1_8 = ( real_T * ) ssGetOutputPortSignal ( S , 7 ) ; real_T * o_B_1_9 = ( real_T * ) ssGetOutputPortSignal ( S , 8 ) ; real_T * o_B_1_10 = ( real_T * ) ssGetOutputPortSignal ( S , 9 ) ; real_T * o_B_1_11 = ( real_T * ) ssGetOutputPortSignal ( S , 10 ) ; real_T * o_B_1_12 = ( real_T * ) ssGetOutputPortSignal ( S , 11 ) ; real_T * o_B_1_13 = ( real_T * ) ssGetOutputPortSignal ( S , 12 ) ; real_T * o_B_1_14 = ( real_T * ) ssGetOutputPortSignal ( S , 13 ) ; real_T * o_B_1_15 = ( real_T * ) ssGetOutputPortSignal ( S , 14 ) ; real_T * o_B_1_16 = ( real_T * ) ssGetOutputPortSignal ( S , 15 ) ; real_T * o_B_1_17 = ( real_T * ) ssGetOutputPortSignal ( S , 16 ) ; real_T * o_B_1_18 = ( real_T * ) ssGetOutputPortSignal ( S , 17 ) ; real_T * o_B_1_19 = ( real_T * ) ssGetOutputPortSignal ( S , 18 ) ; real_T * o_B_1_20 = ( real_T * ) ssGetOutputPortSignal ( S , 19 ) ; real_T * o_B_1_21 = ( real_T * ) ssGetOutputPortSignal ( S , 20 ) ; real_T * o_B_1_22 = ( real_T * ) ssGetOutputPortSignal ( S , 21 ) ; real_T * o_B_1_23 = ( real_T * ) ssGetOutputPortSignal ( S , 22 ) ; real_T * o_B_1_24 = ( real_T * ) ssGetOutputPortSignal ( S , 23 ) ; real_T * o_B_1_25 = ( real_T * ) ssGetOutputPortSignal ( S , 24 ) ; real_T * o_B_1_26 = ( real_T * ) ssGetOutputPortSignal ( S , 25 ) ; real_T * o_B_1_27 = ( real_T * ) ssGetOutputPortSignal ( S , 26 ) ; real_T * o_B_1_28 = ( real_T * ) ssGetOutputPortSignal ( S , 27 ) ; real_T * o_B_1_29 = ( real_T * ) ssGetOutputPortSignal ( S , 28 ) ; real_T * o_B_1_30 = ( real_T * ) ssGetOutputPortSignal ( S , 29 ) ; real_T * o_B_1_31 = ( real_T * ) ssGetOutputPortSignal ( S , 30 ) ; real_T * o_B_1_32 = ( real_T * ) ssGetOutputPortSignal ( S , 31 ) ; real_T * o_B_1_33 = ( real_T * ) ssGetOutputPortSignal ( S , 32 ) ; real_T * o_B_1_34 = ( real_T * ) ssGetOutputPortSignal ( S , 33 ) ; boolean_T * o_o_B_1_35 = ( boolean_T * ) ssGetOutputPortSignal ( S , 34 ) ; real_T * o_o_B_1_36 = ( real_T * ) ssGetOutputPortSignal ( S , 35 ) ; real_T * o_o_B_1_37 = ( real_T * ) ssGetOutputPortSignal ( S , 36 ) ; real_T * o_o_B_1_38 = ( real_T * ) ssGetOutputPortSignal ( S , 37 ) ; real_T * o_o_B_1_39 = ( real_T * ) ssGetOutputPortSignal ( S , 38 ) ; real_T * o_o_B_1_40 = ( real_T * ) ssGetOutputPortSignal ( S , 39 ) ; real_T * o_o_o_B_1_41 = ( real_T * ) ssGetOutputPortSignal ( S , 40 ) ; real_T * o_o_o_B_1_42 = ( real_T * ) ssGetOutputPortSignal ( S , 41 ) ; real_T * o_o_B_1_43 = ( real_T * ) ssGetOutputPortSignal ( S , 42 ) ; real_T * o_o_B_1_44 = ( real_T * ) ssGetOutputPortSignal ( S , 43 ) ; real_T * o_o_o_o_o_B_1_77 = ( real_T * ) ssGetOutputPortSignal ( S , 44 ) ; real_T * o_o_o_o_o_B_1_79 = ( real_T * ) ssGetOutputPortSignal ( S , 46 ) ; real_T * o_o_B_1_45 = ( real_T * ) ssGetOutputPortSignal ( S , 48 ) ; real_T * o_o_B_1_46 = ( real_T * ) ssGetOutputPortSignal ( S , 49 ) ; real_T * o_o_B_1_47 = ( real_T * ) ssGetOutputPortSignal ( S , 50 ) ; real_T * o_o_B_1_48 = ( real_T * ) ssGetOutputPortSignal ( S , 51 ) ; real_T * o_o_B_1_49 = ( real_T * ) ssGetOutputPortSignal ( S , 52 ) ; real_T * o_o_B_1_50 = ( real_T * ) ssGetOutputPortSignal ( S , 53 ) ; real_T * o_o_B_1_51 = ( real_T * ) ssGetOutputPortSignal ( S , 54 ) ; real_T * o_o_B_1_52 = ( real_T * ) ssGetOutputPortSignal ( S , 55 ) ; real_T * o_o_B_1_53 = ( real_T * ) ssGetOutputPortSignal ( S , 56 ) ; real_T * o_o_B_1_54 = ( real_T * ) ssGetOutputPortSignal ( S , 57 ) ; real_T * o_o_B_1_55 = ( real_T * ) ssGetOutputPortSignal ( S , 58 ) ; real_T * o_o_B_1_56 = ( real_T * ) ssGetOutputPortSignal ( S , 59 ) ; real_T * o_o_B_1_57 = ( real_T * ) ssGetOutputPortSignal ( S , 60 ) ; real_T * o_B_1_58 = ( real_T * ) ssGetOutputPortSignal ( S , 61 ) ; real_T * o_B_1_59 = ( real_T * ) ssGetOutputPortSignal ( S , 62 ) ; real_T * o_B_1_60 = ( real_T * ) ssGetOutputPortSignal ( S , 63 ) ; real_T * o_B_1_61 = ( real_T * ) ssGetOutputPortSignal ( S , 64 ) ; real_T * o_B_1_62 = ( real_T * ) ssGetOutputPortSignal ( S , 65 ) ; real_T * o_B_1_63 = ( real_T * ) ssGetOutputPortSignal ( S , 66 ) ; real_T * o_B_1_64 = ( real_T * ) ssGetOutputPortSignal ( S , 67 ) ; real_T * o_B_1_65 = ( real_T * ) ssGetOutputPortSignal ( S , 68 ) ; real_T * o_o_B_1_66 = ( real_T * ) ssGetOutputPortSignal ( S , 70 ) ; real_T * o_o_B_1_67 = ( real_T * ) ssGetOutputPortSignal ( S , 71 ) ; real_T * o_o_o_o_B_1_81 = ( real_T * ) ssGetOutputPortSignal ( S , 72 ) ; real_T * o_B_1_68 = ( real_T * ) ssGetOutputPortSignal ( S , 73 ) ; real_T * o_B_1_69 = ( real_T * ) ssGetOutputPortSignal ( S , 74 ) ; real_T * o_B_1_70 = ( real_T * ) ssGetOutputPortSignal ( S , 75 ) ; real_T * o_B_1_71 = ( real_T * ) ssGetOutputPortSignal ( S , 76 ) ; real_T * o_B_1_72 = ( real_T * ) ssGetOutputPortSignal ( S , 77 ) ; real_T * o_B_1_73 = ( real_T * ) ssGetOutputPortSignal ( S , 78 ) ; real_T * o_B_1_74 = ( real_T * ) ssGetOutputPortSignal ( S , 79 ) ; real_T * o_o_B_1_75 = ( real_T * ) ssGetOutputPortSignal ( S , 80 ) ; real_T * o_o_B_1_76 = ( real_T * ) ssGetOutputPortSignal ( S , 81 ) ; hdulqcnnpv * localX = ( hdulqcnnpv * ) ssGetContStates ( S ) ; o2ljf02bvi * localXdis = ( o2ljf02bvi * ) ssGetContStateDisabled ( S ) ; real_T * o_o_o_o_B_1_78 = ( real_T * ) ssGetOutputPortSignal ( S , 45 ) ; real_T * o_B_1_80 = ( real_T * ) ssGetOutputPortSignal ( S , 69 ) ; if ( tid == PARAMETER_TUNING_TID ) { VehiclePlantTID3 ( o_o_o_o_o_B_1_77 , o_o_o_o_B_1_78 , o_o_o_o_o_B_1_79 , o_B_1_80 , o_o_o_o_B_1_81 , & ( dw -> rtdw ) ) ; } if ( tid != CONSTANT_TID && tid != PARAMETER_TUNING_TID ) { if ( ssIsSampleHit ( S , 0 , tid ) || ssIsMinorTimeStep ( S ) ) { VehiclePlant ( & ( dw -> rtm ) , i_aqbxopp4od , i_gom2amlqr1 , i_dvujmtstvi , i_ar2tixbyms , i_pks5fmem4t , i_iuwyb4fljm , i_domdentxv3 , i_mvdqaplnnt , i_g45kr4jwpf , i_aibdphnll3 , i_k3dmq3ayz4 , i_evn3anml3h , i_nhq1rhohzj , i_bg0p24qife , o_B_1_1 , o_B_1_2 , o_B_1_3 , o_B_1_4 , o_B_1_5 , o_B_1_6 , o_B_1_7 , o_B_1_8 , o_B_1_9 , o_B_1_10 , o_B_1_11 , o_B_1_12 , o_B_1_13 , o_B_1_14 , o_B_1_15 , o_B_1_16 , o_B_1_17 , o_B_1_18 , o_B_1_19 , o_B_1_20 , o_B_1_21 , o_B_1_22 , o_B_1_23 , o_B_1_24 , o_B_1_25 , o_B_1_26 , o_B_1_27 , o_B_1_28 , o_B_1_29 , o_B_1_30 , o_B_1_31 , o_B_1_32 , o_B_1_33 , o_B_1_34 , o_o_B_1_35 , o_o_B_1_36 , o_o_B_1_37 , o_o_B_1_38 , o_o_B_1_39 , o_o_B_1_40 , o_o_o_B_1_41 , o_o_o_B_1_42 , o_o_B_1_43 , o_o_B_1_44 , o_o_o_o_o_B_1_77 , o_o_o_o_o_B_1_79 , o_o_B_1_45 , o_o_B_1_46 , o_o_B_1_47 , o_o_B_1_48 , o_o_B_1_49 , o_o_B_1_50 , o_o_B_1_51 , o_o_B_1_52 , o_o_B_1_53 , o_o_B_1_54 , o_o_B_1_55 , o_o_B_1_56 , o_o_B_1_57 , o_B_1_58 , o_B_1_59 , o_B_1_60 , o_B_1_61 , o_B_1_62 , o_B_1_63 , o_B_1_64 , o_B_1_65 , o_o_B_1_66 , o_o_B_1_67 , o_o_o_o_B_1_81 , o_B_1_68 , o_B_1_69 , o_B_1_70 , o_B_1_71 , o_B_1_72 , o_B_1_73 , o_B_1_74 , o_o_B_1_75 , o_o_B_1_76 , & ( dw -> rtb ) , & ( dw -> rtdw ) , localX , localXdis ) ; } } }
#define MDL_UPDATE
static void mdlUpdate ( SimStruct * S , int_T tid ) { owiw5mmjy2f * dw = ( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; boolean_T * o_o_B_1_35 = ( boolean_T * ) ssGetOutputPortSignal ( S , 34 ) ; real_T * o_o_B_1_36 = ( real_T * ) ssGetOutputPortSignal ( S , 35 ) ; real_T * o_o_B_1_37 = ( real_T * ) ssGetOutputPortSignal ( S , 36 ) ; real_T * o_o_B_1_38 = ( real_T * ) ssGetOutputPortSignal ( S , 37 ) ; real_T * o_o_B_1_39 = ( real_T * ) ssGetOutputPortSignal ( S , 38 ) ; real_T * o_o_B_1_40 = ( real_T * ) ssGetOutputPortSignal ( S , 39 ) ; real_T * o_o_o_B_1_41 = ( real_T * ) ssGetOutputPortSignal ( S , 40 ) ; real_T * o_o_o_B_1_42 = ( real_T * ) ssGetOutputPortSignal ( S , 41 ) ; real_T * o_o_B_1_43 = ( real_T * ) ssGetOutputPortSignal ( S , 42 ) ; real_T * o_o_B_1_44 = ( real_T * ) ssGetOutputPortSignal ( S , 43 ) ; real_T * o_o_o_o_o_B_1_77 = ( real_T * ) ssGetOutputPortSignal ( S , 44 ) ; real_T * o_o_o_o_B_1_78 = ( real_T * ) ssGetOutputPortSignal ( S , 45 ) ; real_T * o_o_o_o_o_B_1_79 = ( real_T * ) ssGetOutputPortSignal ( S , 46 ) ; real_T * o_o_B_1_45 = ( real_T * ) ssGetOutputPortSignal ( S , 48 ) ; real_T * o_o_B_1_46 = ( real_T * ) ssGetOutputPortSignal ( S , 49 ) ; real_T * o_o_B_1_47 = ( real_T * ) ssGetOutputPortSignal ( S , 50 ) ; real_T * o_o_B_1_48 = ( real_T * ) ssGetOutputPortSignal ( S , 51 ) ; real_T * o_o_B_1_49 = ( real_T * ) ssGetOutputPortSignal ( S , 52 ) ; real_T * o_o_B_1_50 = ( real_T * ) ssGetOutputPortSignal ( S , 53 ) ; real_T * o_o_B_1_51 = ( real_T * ) ssGetOutputPortSignal ( S , 54 ) ; real_T * o_o_B_1_52 = ( real_T * ) ssGetOutputPortSignal ( S , 55 ) ; real_T * o_o_B_1_53 = ( real_T * ) ssGetOutputPortSignal ( S , 56 ) ; real_T * o_o_B_1_54 = ( real_T * ) ssGetOutputPortSignal ( S , 57 ) ; real_T * o_o_B_1_55 = ( real_T * ) ssGetOutputPortSignal ( S , 58 ) ; real_T * o_o_B_1_56 = ( real_T * ) ssGetOutputPortSignal ( S , 59 ) ; real_T * o_o_B_1_57 = ( real_T * ) ssGetOutputPortSignal ( S , 60 ) ; real_T * o_o_B_1_75 = ( real_T * ) ssGetOutputPortSignal ( S , 80 ) ; real_T * o_o_B_1_76 = ( real_T * ) ssGetOutputPortSignal ( S , 81 ) ; bdatrumzy4 ( & ( dw -> rtm ) , o_o_B_1_35 , o_o_B_1_36 , o_o_B_1_37 , o_o_B_1_38 , o_o_B_1_39 , o_o_B_1_40 , o_o_o_B_1_41 , o_o_o_B_1_42 , o_o_B_1_43 , o_o_B_1_44 , o_o_o_o_o_B_1_77 , o_o_o_o_B_1_78 , o_o_o_o_o_B_1_79 , o_o_B_1_45 , o_o_B_1_46 , o_o_B_1_47 , o_o_B_1_48 , o_o_B_1_49 , o_o_B_1_50 , o_o_B_1_51 , o_o_B_1_52 , o_o_B_1_53 , o_o_B_1_54 , o_o_B_1_55 , o_o_B_1_56 , o_o_B_1_57 , o_o_B_1_75 , o_o_B_1_76 , & ( dw -> rtb ) , & ( dw -> rtdw ) ) ; return ; }
#define MDL_DERIVATIVES
static void mdlDerivatives ( SimStruct * S ) { owiw5mmjy2f * dw = ( owiw5mmjy2f
* ) ssGetDWork ( S , 0 ) ; real_T * o_o_o_B_1_41 = ( real_T * )
ssGetOutputPortSignal ( S , 40 ) ; real_T * o_o_o_B_1_42 = ( real_T * )
ssGetOutputPortSignal ( S , 41 ) ; hdulqcnnpv * localX = ( hdulqcnnpv * )
ssGetContStates ( S ) ; p1wcsoehle * localXdot = ( p1wcsoehle * ) ssGetdX ( S
) ; m5u2c0ldal ( o_o_o_B_1_41 , o_o_o_B_1_42 , & ( dw -> rtdw ) , localX ,
localXdot ) ; } static void mdlTerminate ( SimStruct * S ) { owiw5mmjy2f * dw
= ( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; omtgknbblj ( & ( dw -> rtdw ) , &
( dw -> rtm ) ) ; simTgtFreeOPModelData ( S ) ; return ; }
#define MDL_CLEANUP_RUNTIME_RESOURCES
static void mdlCleanupRuntimeResources ( SimStruct * S ) { owiw5mmjy2f * dw =
( owiw5mmjy2f * ) ssGetDWork ( S , 0 ) ; dgmiwxg00f ( & ( dw -> rtdw ) ) ; }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static mxArray * mdlGetSimState ( SimStruct * S ) { static const char *
simStateFieldNames [ 7 ] = { "localX" , "mdlrefDW" , "disallowedStateData" ,
"tNext" , "tNextTid" , "nonContDerivSigInfoPrevVal" , "voidMemoryData" , } ;
mxArray * ss = mxCreateStructMatrix ( 1 , 1 , 7 , simStateFieldNames ) ; {
const hdulqcnnpv * localX = ( const hdulqcnnpv * ) ssGetContStates ( S ) ;
const size_t numBytes = sizeof ( hdulqcnnpv ) ; mxArray * storedX =
mxCreateNumericMatrix ( 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; UINT8_T *
rawData = ( UINT8_T * ) mxGetData ( storedX ) ; memcpy ( & rawData [ 0 ] ,
localX , numBytes ) ; mxSetFieldByNumber ( ss , 0 , 0 , storedX ) ; } {
mxArray * mdlrefDW = mr_VehiclePlant_GetDWork ( ssGetDWork ( S , 0 ) ) ;
mxSetFieldByNumber ( ss , 0 , 1 , mdlrefDW ) ; } { mxArray * data =
mr_VehiclePlant_GetSimStateDisallowedBlocks ( ) ; mxSetFieldByNumber ( ss , 0
, 2 , data ) ; } ; mxSetFieldByNumber ( ss , 0 , 3 , mxCreateDoubleScalar ( ( double ) ssGetTNext ( S ) ) ) ; mxSetFieldByNumber ( ss , 0 , 4 , mxCreateDoubleScalar ( ( double ) ssGetTNextTid ( S ) ) ) ; { mxArray * voidMemData = simTgtSerializeVoidMemoryOP ( S ) ; mxSetFieldByNumber ( ss , 0 , 6 , voidMemData ) ; } return ss ; }
#if !defined(MDL_SIM_STATE)
#define MDL_SIM_STATE
#endif
static void mdlSetSimState ( SimStruct * S , const mxArray * ss ) { { mxArray
* voidMemData = mxGetFieldByNumber ( ss , 0 , 6 ) ;
simTgtDeserializeVoidMemoryOP ( S , voidMemData ) ; } { hdulqcnnpv * localX =
( hdulqcnnpv * ) ssGetContStates ( S ) ; const size_t numBytes = sizeof ( hdulqcnnpv ) ; const mxArray * storedX = mxGetFieldByNumber ( ss , 0 , 0 ) ; const UINT8_T * rawData = ( const UINT8_T * ) mxGetData ( storedX ) ; memcpy ( localX , & rawData [ 0 ] , numBytes ) ; } mr_VehiclePlant_SetDWork ( ssGetDWork ( S , 0 ) , mxGetFieldByNumber ( ss , 0 , 1 ) ) ; ssSetTNext ( S , ( time_T ) mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 3 ) ) ) ; ssSetTNextTid ( S , ( int_T ) mxGetScalar ( mxGetFieldByNumber ( ss , 0 , 4 ) ) ) ; }
#ifdef MATLAB_MEX_FILE 
#include "simulink.c"
#include "fixedpoint.c"
#else
#error Assertion failed: file must be compiled as a MEX-file
#endif
