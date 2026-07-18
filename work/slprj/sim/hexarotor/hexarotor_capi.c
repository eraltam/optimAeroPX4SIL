#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "hexarotor_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 44
#endif
#ifndef SS_INT64
#define SS_INT64 45
#endif
#else
#include "builtin_typeid_types.h"
#include "hexarotor.h"
#include "hexarotor_capi.h"
#include "hexarotor_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static const rtwCAPI_SignalHierLoggingInfo rtSigHierLoggingInfo [ ] = { { ""
, 3 , 0 } , { "controllerArmed" , 0 , - 1 } , { "ServosCommandBus" , 1 , 3 }
, { "MotorCommandHexBus" , 6 , 4 } , { "cmdRotor1_nd" , 0 , - 1 } , {
"cmdRotor2_nd" , 0 , - 1 } , { "cmdRotor3_nd" , 0 , - 1 } , { "cmdRotor4_nd"
, 0 , - 1 } , { "cmdRotor5_nd" , 0 , - 1 } , { "cmdRotor6_nd" , 0 , - 1 } , {
"EngineCommandBus" , 1 , 10 } , { "cmdThrottle_unit" , 0 , - 1 } , { "" , 2 ,
11 } , { "forcesInBody_N" , 0 , - 1 } , { "momentsInBody_Nm" , 0 , - 1 } } ;
static const uint_T rtSigHierLoggingChildIdxs [ ] = { 1 , 2 , 10 , 3 , 4 , 5
, 6 , 7 , 8 , 9 , 11 , 13 , 14 } ; static rtwCAPI_Signals rtBlockSignals [ ]
= { { 0 , 0 , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 } } ; static
rtwCAPI_States rtBlockStates [ ] = { { 0 , - 1 , ( NULL ) , ( NULL ) , ( NULL
) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ; static int_T rt_LoggedStateIdxList
[ ] = { - 1 } ; static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = {
{ "" , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 } } ;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 0 , 0 } } ; static uint_T rtDimensionArray [ ] = { 0 } ;
static rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) ,
rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , } ; static
rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( NULL ) , ( NULL ) , 0 , 0 }
} ; static int_T rtContextSystems [ 2 ] ; static rtwCAPI_LoggingMetaInfo
loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static
rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 2 ,
rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 15 ,
rtSigHierLoggingInfo , rtSigHierLoggingChildIdxs } , 0 , ( NULL ) } ; static
rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL )
, 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 0
} , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap ,
rtSampleTimeMap , rtDimensionArray } , "float" , { 2048735121U , 4163694928U
, 3508528143U , 3046437346U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0
, rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo *
hexarotor_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void hexarotor_InitializeSystemRan ( d3tk5bm4nu * const kq2a4ledd0 ,
sysRanDType * systemRan [ ] , pzfrokszxg * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( kq2a4ledd0 ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemTid [ 1 ] = kq2a4ledd0 ->
Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ;
rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void hexarotor_InitializeDataMapInfo ( d3tk5bm4nu * const kq2a4ledd0 ,
pzfrokszxg * localDW , void * sysRanPtr , int_T contextTid ) {
rtwCAPI_SetVersion ( kq2a4ledd0 -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( kq2a4ledd0 -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( kq2a4ledd0 -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( kq2a4ledd0 -> DataMapInfo . mmi ,
( NULL ) ) ; rtwCAPI_SetFullPath ( kq2a4ledd0 -> DataMapInfo . mmi , ( NULL )
) ; rtwCAPI_SetInstanceLoggingInfo ( kq2a4ledd0 -> DataMapInfo . mmi , &
kq2a4ledd0 -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( kq2a4ledd0 -> DataMapInfo . mmi , kq2a4ledd0 -> DataMapInfo . childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( kq2a4ledd0 -> DataMapInfo . mmi , 7 ) ; hexarotor_InitializeSystemRan ( kq2a4ledd0 , kq2a4ledd0 -> DataMapInfo . systemRan , localDW , kq2a4ledd0 -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( kq2a4ledd0 -> DataMapInfo . mmi , kq2a4ledd0 -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( kq2a4ledd0 -> DataMapInfo . mmi , kq2a4ledd0 -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( kq2a4ledd0 -> DataMapInfo . mmi , & kq2a4ledd0 -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void hexarotor_host_InitializeDataMapInfo ( hexarotor_host_DataMapInfo_T *
dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ;
rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ;
rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath
( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) )
; dataMap -> childMMI [ 0 ] = & ( dataMap -> child0 . mmi ) ;
hexActuator_host_InitializeDataMapInfo ( & ( dataMap -> child0 ) ,
"hexarotor/actuators" ) ; dataMap -> childMMI [ 1 ] = & ( dataMap -> child1 .
mmi ) ; hexAddFm_host_InitializeDataMapInfo ( & ( dataMap -> child1 ) ,
"hexarotor/addFm" ) ; dataMap -> childMMI [ 2 ] = & ( dataMap -> child2 . mmi
) ; hexAero_host_InitializeDataMapInfo ( & ( dataMap -> child2 ) ,
"hexarotor/aero" ) ; dataMap -> childMMI [ 3 ] = & ( dataMap -> child3 . mmi
) ; cgMassInertia_host_InitializeDataMapInfo ( & ( dataMap -> child3 ) ,
"hexarotor/cgMassInertia" ) ; dataMap -> childMMI [ 4 ] = & ( dataMap ->
child4 . mmi ) ; inertialDynamics_host_InitializeDataMapInfo ( & ( dataMap ->
child4 ) , "hexarotor/dynamics" ) ; dataMap -> childMMI [ 5 ] = & ( dataMap
-> child5 . mmi ) ; gravity_host_InitializeDataMapInfo ( & ( dataMap ->
child5 ) , "hexarotor/gravity" ) ; dataMap -> childMMI [ 6 ] = & ( dataMap ->
child6 . mmi ) ; hexGroundContact_host_InitializeDataMapInfo ( & ( dataMap ->
child6 ) , "hexarotor/groundModel" ) ; rtwCAPI_SetChildMMIArray ( dataMap ->
mmi , dataMap -> childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi ,
7 ) ; }
#ifdef __cplusplus
}
#endif
#endif
