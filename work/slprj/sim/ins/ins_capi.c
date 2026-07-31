#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "ins_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 34
#endif
#ifndef SS_INT64
#define SS_INT64 35
#endif
#else
#include "builtin_typeid_types.h"
#include "ins.h"
#include "ins_capi.h"
#include "ins_private.h"
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
, TARGET_STRING ( "ins/Three-axis Inertial\nMeasurement Unit/Three-axis Accelerometer/Dynamics/Second-order Dynamics/Discrete\nTransfer Fcn" ) , TARGET_STRING ( "states" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 1 , - 1 , TARGET_STRING ( "ins/Three-axis Inertial\nMeasurement Unit/Three-axis Gyroscope/Dynamics/Second-order Dynamics/Discrete\nTransfer Fcn" ) , TARGET_STRING ( "states" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } , { 0 , - 1 , ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ; static int_T rt_LoggedStateIdxList [ ] = { 1 , 0 } ;
#ifndef HOST_CAPI_BUILD
static void ins_InitializeDataAddr ( void * dataAddr [ ] , nh5zucdt3e *
localDW ) { dataAddr [ 0 ] = ( void * ) ( & localDW -> mp3p522fzj [ 0 ] ) ;
dataAddr [ 1 ] = ( void * ) ( & localDW -> brno0o4wuu [ 0 ] ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void ins_InitializeVarDimsAddr ( int32_T * vardimsAddr [ ] ) {
vardimsAddr [ 0 ] = ( NULL ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void ins_InitializeLoggingFunctions ( RTWLoggingFcnPtr loggingPtrs [ ]
) { loggingPtrs [ 0 ] = ( NULL ) ; loggingPtrs [ 1 ] = ( NULL ) ; }
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , ( uint8_T ) SS_DOUBLE , 0 , 0 , 0 } }
;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_VECTOR , 0 , 2 , 0 } } ; static uint_T rtDimensionArray [ ] = { 6 , 1
} ; static const real_T rtcapiStoredFloats [ ] = { 0.004 , 0.0 } ; static
rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) ,
rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , } ; static
rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( const void * ) &
rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 1 ] , ( int8_T ) 0 , ( uint8_T ) 0 } } ; static int_T rtContextSystems [ 2 ] ; static rtwCAPI_LoggingMetaInfo loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 2 , rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL ) } , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 2 } , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" , { 2721118992U , 1891799112U , 1806456272U , 2268003800U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo * ins_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void ins_InitializeSystemRan ( ntnem3iwts * const lsn0hi3qme ,
sysRanDType * systemRan [ ] , nh5zucdt3e * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( lsn0hi3qme ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemTid [ 1 ] = lsn0hi3qme ->
Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ;
rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void ins_InitializeDataMapInfo ( ntnem3iwts * const lsn0hi3qme , nh5zucdt3e *
localDW , void * sysRanPtr , int_T contextTid ) { rtwCAPI_SetVersion ( lsn0hi3qme -> DataMapInfo . mmi , 1 ) ; rtwCAPI_SetStaticMap ( lsn0hi3qme -> DataMapInfo . mmi , & mmiStatic ) ; rtwCAPI_SetLoggingStaticMap ( lsn0hi3qme -> DataMapInfo . mmi , & mmiStaticInfoLogging ) ; ins_InitializeDataAddr ( lsn0hi3qme -> DataMapInfo . dataAddress , localDW ) ; rtwCAPI_SetDataAddressMap ( lsn0hi3qme -> DataMapInfo . mmi , lsn0hi3qme -> DataMapInfo . dataAddress ) ; ins_InitializeVarDimsAddr ( lsn0hi3qme -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetVarDimsAddressMap ( lsn0hi3qme -> DataMapInfo . mmi , lsn0hi3qme -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetPath ( lsn0hi3qme -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetFullPath ( lsn0hi3qme -> DataMapInfo . mmi , ( NULL ) ) ; ins_InitializeLoggingFunctions ( lsn0hi3qme -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetLoggingPtrs ( lsn0hi3qme -> DataMapInfo . mmi , lsn0hi3qme -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetInstanceLoggingInfo ( lsn0hi3qme -> DataMapInfo . mmi , & lsn0hi3qme -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( lsn0hi3qme -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( lsn0hi3qme -> DataMapInfo . mmi , 0 ) ; ins_InitializeSystemRan ( lsn0hi3qme , lsn0hi3qme -> DataMapInfo . systemRan , localDW , lsn0hi3qme -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( lsn0hi3qme -> DataMapInfo . mmi , lsn0hi3qme -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( lsn0hi3qme -> DataMapInfo . mmi , lsn0hi3qme -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( lsn0hi3qme -> DataMapInfo . mmi , & lsn0hi3qme -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void ins_host_InitializeDataMapInfo ( ins_host_DataMapInfo_T * dataMap ,
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
