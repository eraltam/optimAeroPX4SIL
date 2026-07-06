#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "inertialDynamics_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 21
#endif
#ifndef SS_INT64
#define SS_INT64 22
#endif
#else
#include "builtin_typeid_types.h"
#include "inertialDynamics.h"
#include "inertialDynamics_capi.h"
#include "inertialDynamics_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , ( NULL ) , ( NULL ) ,
0 , 0 , 0 , 0 , 0 } } ; static rtwCAPI_States rtBlockStates [ ] = { { 0 , 3 ,
TARGET_STRING ( "inertialDynamics/variableMass6DOF/p,q,r " ) , TARGET_STRING
( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 1 , 0 ,
TARGET_STRING ( "inertialDynamics/variableMass6DOF/ub,vb,wb" ) ,
TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0
} , { 2 , 10 , TARGET_STRING ( "inertialDynamics/variableMass6DOF/xe,ye,ze" )
, TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 ,
0 } , { 3 , 6 , TARGET_STRING ( "inertialDynamics/variableMass6DOF/Calculate DCM &\nEuler Angles/q0\nq1\nq2\nq3" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 0 , - 1 , ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ; static int_T rt_LoggedStateIdxList [ ] = { 1 , 0 , 3 , 2 } ;
#ifndef HOST_CAPI_BUILD
static void inertialDynamics_InitializeDataAddr ( void * dataAddr [ ] ,
eme3h2xopw * localDW , bsfwj4wmdv * localX ) { dataAddr [ 0 ] = ( void * ) ( &
localX -> fcdsavymb5 [ 0 ] ) ; dataAddr [ 1 ] = ( void * ) ( & localX ->
f31o2xbo0p [ 0 ] ) ; dataAddr [ 2 ] = ( void * ) ( & localX -> imy3vblrz0 [ 0
] ) ; dataAddr [ 3 ] = ( void * ) ( & localX -> ewqcbelfa0 [ 0 ] ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void inertialDynamics_InitializeVarDimsAddr ( int32_T * vardimsAddr [
] ) { vardimsAddr [ 0 ] = ( NULL ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void inertialDynamics_InitializeLoggingFunctions ( RTWLoggingFcnPtr
loggingPtrs [ ] ) { loggingPtrs [ 0 ] = ( NULL ) ; loggingPtrs [ 1 ] = ( NULL
) ; loggingPtrs [ 2 ] = ( NULL ) ; loggingPtrs [ 3 ] = ( NULL ) ; }
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , ( uint8_T ) SS_DOUBLE , 0 , 0 , 0 } }
;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_VECTOR , 0 , 2 , 0 } , { rtwCAPI_VECTOR , 2 , 2 , 0 } } ; static
uint_T rtDimensionArray [ ] = { 3 , 1 , 4 , 1 } ; static const real_T
rtcapiStoredFloats [ ] = { 0.0 } ; static rtwCAPI_FixPtMap rtFixPtMap [ ] = {
{ ( NULL ) , ( NULL ) , rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , }
; static rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( const void * ) &
rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 0 ] , ( int8_T ) 0 , ( uint8_T ) 0 } } ; static int_T rtContextSystems [ 22 ] ; static rtwCAPI_LoggingMetaInfo loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 22 , rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL ) } , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 4 } , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" , { 556096540U , 3089543035U , 756040539U , 1028856250U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo * inertialDynamics_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void inertialDynamics_InitializeSystemRan ( knpvazrmc0 * const
ag5wddik2h , sysRanDType * systemRan [ ] , eme3h2xopw * localDW , int_T
systemTid [ ] , void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( ag5wddik2h ) ; UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * ) rootSysRanPtr ; systemRan [ 1 ] = ( sysRanDType * ) & localDW -> bgzclzzhfw ; systemRan [ 2 ] = ( sysRanDType * ) & localDW -> c14vd4y1qk ; systemRan [ 3 ] = ( sysRanDType * ) & localDW -> fmz4dmdvz4 ; systemRan [ 4 ] = ( sysRanDType * ) & localDW -> izb5qunafm ; systemRan [ 5 ] = ( sysRanDType * ) & localDW -> fmrnddkmir ; systemRan [ 6 ] = ( sysRanDType * ) & localDW -> gk43f4mbi3 ; systemRan [ 7 ] = ( sysRanDType * ) & localDW -> hgp5oylal5 ; systemRan [ 8 ] = ( sysRanDType * ) & localDW -> d5jvwpkuiu ; systemRan [ 9 ] = ( sysRanDType * ) & localDW -> cn011ea52d ; systemRan [ 10 ] = ( sysRanDType * ) & localDW -> p0o04ghnpb ; systemRan [ 11 ] = ( sysRanDType * ) & localDW -> nhiluv4y4e ; systemRan [ 12 ] = ( sysRanDType * ) & localDW -> ewx5cx1amh ; systemRan [ 13 ] = ( sysRanDType * ) & localDW -> a2qel35vnh ; systemRan [ 14 ] = ( sysRanDType * ) & localDW -> bxo2dasr0u ; systemRan [ 15 ] = ( sysRanDType * ) & localDW -> bq2bf2jl1a ; systemRan [ 16 ] = ( sysRanDType * ) & localDW -> dkklgbpgan ; systemRan [ 17 ] = ( sysRanDType * ) & localDW -> niwv0zpcul ; systemRan [ 18 ] = ( sysRanDType * ) & localDW -> pou2pibjcr ; systemRan [ 19 ] = ( NULL ) ; systemRan [ 20 ] = ( NULL ) ; systemRan [ 21 ] = ( NULL ) ; systemTid [ 5 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 6 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 7 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 8 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 1 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 2 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 3 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 4 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 16 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 14 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 15 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 17 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 11 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 9 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 10 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 12 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 13 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 18 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 19 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 20 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 1 ] ; systemTid [ 21 ] = ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 1 ; rtContextSystems [ 2 ] = 2 ; rtContextSystems [ 3 ] = 3 ; rtContextSystems [ 4 ] = 4 ; rtContextSystems [ 5 ] = 5 ; rtContextSystems [ 6 ] = 6 ; rtContextSystems [ 7 ] = 7 ; rtContextSystems [ 8 ] = 8 ; rtContextSystems [ 9 ] = 9 ; rtContextSystems [ 10 ] = 10 ; rtContextSystems [ 11 ] = 11 ; rtContextSystems [ 12 ] = 12 ; rtContextSystems [ 13 ] = 13 ; rtContextSystems [ 14 ] = 14 ; rtContextSystems [ 15 ] = 15 ; rtContextSystems [ 16 ] = 16 ; rtContextSystems [ 17 ] = 17 ; rtContextSystems [ 18 ] = 18 ; rtContextSystems [ 19 ] = 0 ; rtContextSystems [ 20 ] = 0 ; rtContextSystems [ 21 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void inertialDynamics_InitializeDataMapInfo ( knpvazrmc0 * const ag5wddik2h ,
eme3h2xopw * localDW , bsfwj4wmdv * localX , void * sysRanPtr , int_T
contextTid ) { rtwCAPI_SetVersion ( ag5wddik2h -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( ag5wddik2h -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( ag5wddik2h -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; inertialDynamics_InitializeDataAddr ( ag5wddik2h ->
DataMapInfo . dataAddress , localDW , localX ) ; rtwCAPI_SetDataAddressMap ( ag5wddik2h -> DataMapInfo . mmi , ag5wddik2h -> DataMapInfo . dataAddress ) ; inertialDynamics_InitializeVarDimsAddr ( ag5wddik2h -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetVarDimsAddressMap ( ag5wddik2h -> DataMapInfo . mmi , ag5wddik2h -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetPath ( ag5wddik2h -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetFullPath ( ag5wddik2h -> DataMapInfo . mmi , ( NULL ) ) ; inertialDynamics_InitializeLoggingFunctions ( ag5wddik2h -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetLoggingPtrs ( ag5wddik2h -> DataMapInfo . mmi , ag5wddik2h -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetInstanceLoggingInfo ( ag5wddik2h -> DataMapInfo . mmi , & ag5wddik2h -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( ag5wddik2h -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( ag5wddik2h -> DataMapInfo . mmi , 0 ) ; inertialDynamics_InitializeSystemRan ( ag5wddik2h , ag5wddik2h -> DataMapInfo . systemRan , localDW , ag5wddik2h -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( ag5wddik2h -> DataMapInfo . mmi , ag5wddik2h -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( ag5wddik2h -> DataMapInfo . mmi , ag5wddik2h -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( ag5wddik2h -> DataMapInfo . mmi , & ag5wddik2h -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void inertialDynamics_host_InitializeDataMapInfo ( inertialDynamics_host_DataMapInfo_T * dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ; rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ; rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath ( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 0 ) ; }
#ifdef __cplusplus
}
#endif
#endif
