#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "Earth_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 19
#endif
#ifndef SS_INT64
#define SS_INT64 20
#endif
#else
#include "builtin_typeid_types.h"
#include "Earth.h"
#include "Earth_capi.h"
#include "Earth_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , ( NULL ) , ( NULL ) ,
0 , 0 , 0 , 0 , 0 } } ; static rtwCAPI_States rtBlockStates [ ] = { { 0 , - 1
, TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/Unit Delay" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 1 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/Unit Delay2" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 1 , 0 , 0 , 0 , - 1 , 0 } , { 2 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Convert from geodetic to\n spherical coordinates\n/For Iterator\nSubsystem/Unit Delay1" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 2 , 0 , 0 , 0 , - 1 , 0 } , { 3 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Accumulate terms of the \nspherical harmonic expansion/Unit Delay1" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 4 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Accumulate terms of the \nspherical harmonic expansion/Unit Delay2" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 5 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Accumulate terms of the \nspherical harmonic expansion/Unit Delay3" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 6 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Accumulate terms of the \nspherical harmonic expansion/Unit Delay4" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 7 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Compute unnormalized associated \nlegendre polynomials and \nderivatives via recursion relations/Unit Delay" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 3 , 0 , 0 , 0 , - 1 , 0 } , { 8 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Compute unnormalized associated \nlegendre polynomials and \nderivatives via recursion relations/Unit Delay1" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 3 , 0 , 0 , 0 , - 1 , 0 } , { 9 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Time adjust the gauss coefficients/Unit Delay" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 3 , 0 , 0 , 0 , - 1 , 0 } , { 10 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Accumulate terms of the \nspherical harmonic expansion/Special case - North//South Geographic Pole/Unit Delay1" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 4 , 0 , 0 , 0 , - 1 , 0 } , { 11 , - 1 , TARGET_STRING ( "Earth/World Magnetic Model/geomag/Compute magnetic vector in\nspherical coordinates/For Iterator\nSubsystem/Time adjust the gauss coefficients/if (m~=0)/Unit Delay" ) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 3 , 0 , 0 , 0 , - 1 , 0 } , { 0 , - 1 , ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ; static int_T rt_LoggedStateIdxList [ ] = { 1 , 2 , 0 , 3 , 5 , 4 , 6 , 9 , 10 , 7 , 11 , 8 } ;
#ifndef HOST_CAPI_BUILD
static void Earth_InitializeDataAddr ( void * dataAddr [ ] , micoo1q4de *
localDW ) { dataAddr [ 0 ] = ( void * ) ( & localDW -> nlhu1a3bnw ) ;
dataAddr [ 1 ] = ( void * ) ( & localDW -> gkws4s554s [ 0 ] ) ; dataAddr [ 2
] = ( void * ) ( & localDW -> aq4saffazm [ 0 ] ) ; dataAddr [ 3 ] = ( void *
) ( & localDW -> mfokmdqwwn ) ; dataAddr [ 4 ] = ( void * ) ( & localDW ->
ht50pmmim3 ) ; dataAddr [ 5 ] = ( void * ) ( & localDW -> plzcprkyqj ) ;
dataAddr [ 6 ] = ( void * ) ( & localDW -> iwkwllhrgj ) ; dataAddr [ 7 ] = ( void * ) ( & localDW -> kepstjkn2c [ 0 ] ) ; dataAddr [ 8 ] = ( void * ) ( & localDW -> o5zi5azywh [ 0 ] ) ; dataAddr [ 9 ] = ( void * ) ( & localDW -> o1ximcjf0n [ 0 ] ) ; dataAddr [ 10 ] = ( void * ) ( & localDW -> dt4rii0abk [ 0 ] ) ; dataAddr [ 11 ] = ( void * ) ( & localDW -> a1lw0ye4rx [ 0 ] ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void Earth_InitializeVarDimsAddr ( int32_T * vardimsAddr [ ] ) {
vardimsAddr [ 0 ] = ( NULL ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void Earth_InitializeLoggingFunctions ( RTWLoggingFcnPtr loggingPtrs [
] ) { loggingPtrs [ 0 ] = ( NULL ) ; loggingPtrs [ 1 ] = ( NULL ) ;
loggingPtrs [ 2 ] = ( NULL ) ; loggingPtrs [ 3 ] = ( NULL ) ; loggingPtrs [ 4
] = ( NULL ) ; loggingPtrs [ 5 ] = ( NULL ) ; loggingPtrs [ 6 ] = ( NULL ) ;
loggingPtrs [ 7 ] = ( NULL ) ; loggingPtrs [ 8 ] = ( NULL ) ; loggingPtrs [ 9
] = ( NULL ) ; loggingPtrs [ 10 ] = ( NULL ) ; loggingPtrs [ 11 ] = ( NULL )
; }
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , ( uint8_T ) SS_DOUBLE , 0 , 0 , 0 } }
;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 2 , 0 } , { rtwCAPI_VECTOR , 2 , 2 , 0 } , {
rtwCAPI_VECTOR , 4 , 2 , 0 } , { rtwCAPI_VECTOR , 6 , 2 , 0 } , {
rtwCAPI_VECTOR , 8 , 2 , 0 } } ; static uint_T rtDimensionArray [ ] = { 1 , 1
, 4 , 1 , 2 , 1 , 169 , 1 , 13 , 1 } ; static const real_T rtcapiStoredFloats
[ ] = { 0.004 , 0.0 } ; static rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL )
, ( NULL ) , rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , } ; static
rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( const void * ) &
rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 1 ] , ( int8_T ) 0 , ( uint8_T ) 0 } } ; static int_T rtContextSystems [ 27 ] ; static rtwCAPI_LoggingMetaInfo loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 27 , rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL ) } , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 12 } , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" , { 3167479844U , 1446033321U , 3553330557U , 3273377166U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo * Earth_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void Earth_InitializeSystemRan ( bb1vfhetht * const craevkrdm5 ,
sysRanDType * systemRan [ ] , micoo1q4de * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( craevkrdm5 ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( sysRanDType * ) & localDW -> f0e5t0qls2 ;
systemRan [ 2 ] = ( sysRanDType * ) & localDW -> an4cuvbvfz ; systemRan [ 3 ]
= ( NULL ) ; systemRan [ 4 ] = ( NULL ) ; systemRan [ 5 ] = ( NULL ) ;
systemRan [ 6 ] = ( sysRanDType * ) & localDW -> db43rtjmwy ; systemRan [ 7 ]
= ( sysRanDType * ) & localDW -> e1sy0aue55 ; systemRan [ 8 ] = ( sysRanDType
* ) & localDW -> cwaubofgz1 ; systemRan [ 9 ] = ( sysRanDType * ) & localDW
-> mopryukfaa ; systemRan [ 10 ] = ( sysRanDType * ) & localDW -> mvvyi2x3ss
; systemRan [ 11 ] = ( sysRanDType * ) & localDW -> iq40nm4sqy ; systemRan [
12 ] = ( sysRanDType * ) & localDW -> a1wbmrcgjp ; systemRan [ 13 ] = ( sysRanDType * ) & localDW -> cy2vzgbbze ; systemRan [ 14 ] = ( sysRanDType * ) & localDW -> ginf43jyeg ; systemRan [ 15 ] = ( NULL ) ; systemRan [ 16 ] = ( sysRanDType * ) & localDW -> d4yyi3btmt ; systemRan [ 17 ] = ( sysRanDType * ) & localDW -> pnvt13ahqb ; systemRan [ 18 ] = ( sysRanDType * ) & localDW -> pyj3gbvmey ; systemRan [ 19 ] = ( sysRanDType * ) & localDW -> pin4nicaxa ; systemRan [ 20 ] = ( NULL ) ; systemRan [ 21 ] = ( NULL ) ; systemRan [ 22 ] = ( sysRanDType * ) & localDW -> bv0fo1kf4j ; systemRan [ 23 ] = ( NULL ) ; systemRan [ 24 ] = ( sysRanDType * ) & localDW -> j3plydqzjg ; systemRan [ 25 ] = ( NULL ) ; systemRan [ 26 ] = ( NULL ) ; systemTid [ 1 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 2 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 3 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 4 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 5 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 6 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 7 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 8 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 9 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 10 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 11 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 12 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 13 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 14 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 15 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 16 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 17 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 18 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 19 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 20 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 21 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 22 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 23 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 24 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 25 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 26 ] = craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 1 ; rtContextSystems [ 2 ] = 2 ; rtContextSystems [ 3 ] = 0 ; rtContextSystems [ 4 ] = 0 ; rtContextSystems [ 5 ] = 0 ; rtContextSystems [ 6 ] = 6 ; rtContextSystems [ 7 ] = 7 ; rtContextSystems [ 8 ] = 8 ; rtContextSystems [ 9 ] = 9 ; rtContextSystems [ 10 ] = 10 ; rtContextSystems [ 11 ] = 11 ; rtContextSystems [ 12 ] = 12 ; rtContextSystems [ 13 ] = 13 ; rtContextSystems [ 14 ] = 14 ; rtContextSystems [ 15 ] = 18 ; rtContextSystems [ 16 ] = 16 ; rtContextSystems [ 17 ] = 17 ; rtContextSystems [ 18 ] = 18 ; rtContextSystems [ 19 ] = 19 ; rtContextSystems [ 20 ] = 0 ; rtContextSystems [ 21 ] = 0 ; rtContextSystems [ 22 ] = 22 ; rtContextSystems [ 23 ] = 24 ; rtContextSystems [ 24 ] = 24 ; rtContextSystems [ 25 ] = 0 ; rtContextSystems [ 26 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void Earth_InitializeDataMapInfo ( bb1vfhetht * const craevkrdm5 , micoo1q4de
* localDW , void * sysRanPtr , int_T contextTid ) { rtwCAPI_SetVersion ( craevkrdm5 -> DataMapInfo . mmi , 1 ) ; rtwCAPI_SetStaticMap ( craevkrdm5 -> DataMapInfo . mmi , & mmiStatic ) ; rtwCAPI_SetLoggingStaticMap ( craevkrdm5 -> DataMapInfo . mmi , & mmiStaticInfoLogging ) ; Earth_InitializeDataAddr ( craevkrdm5 -> DataMapInfo . dataAddress , localDW ) ; rtwCAPI_SetDataAddressMap ( craevkrdm5 -> DataMapInfo . mmi , craevkrdm5 -> DataMapInfo . dataAddress ) ; Earth_InitializeVarDimsAddr ( craevkrdm5 -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetVarDimsAddressMap ( craevkrdm5 -> DataMapInfo . mmi , craevkrdm5 -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetPath ( craevkrdm5 -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetFullPath ( craevkrdm5 -> DataMapInfo . mmi , ( NULL ) ) ; Earth_InitializeLoggingFunctions ( craevkrdm5 -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetLoggingPtrs ( craevkrdm5 -> DataMapInfo . mmi , craevkrdm5 -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetInstanceLoggingInfo ( craevkrdm5 -> DataMapInfo . mmi , & craevkrdm5 -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( craevkrdm5 -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( craevkrdm5 -> DataMapInfo . mmi , 0 ) ; Earth_InitializeSystemRan ( craevkrdm5 , craevkrdm5 -> DataMapInfo . systemRan , localDW , craevkrdm5 -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( craevkrdm5 -> DataMapInfo . mmi , craevkrdm5 -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( craevkrdm5 -> DataMapInfo . mmi , craevkrdm5 -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( craevkrdm5 -> DataMapInfo . mmi , & craevkrdm5 -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void Earth_host_InitializeDataMapInfo ( Earth_host_DataMapInfo_T * dataMap ,
const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ;
rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ;
rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath
( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) )
; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 0 ) ; }
#ifdef __cplusplus
}
#endif
#endif
