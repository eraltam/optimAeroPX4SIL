#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "VehiclePlant_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 58
#endif
#ifndef SS_INT64
#define SS_INT64 59
#endif
#else
#include "builtin_typeid_types.h"
#include "VehiclePlant.h"
#include "VehiclePlant_capi.h"
#include "VehiclePlant_private.h"
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
, ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ;
static int_T rt_LoggedStateIdxList [ ] = { - 1 } ; static TARGET_CONST
rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "" , "" , 0 , 0 , 0 , 0 , 0 , 0 ,
0 } } ;
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
rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL )
} , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { {
rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL
) , 0 } , { rtBlockStates , 0 } , { rtDataTypeMap , rtDimensionMap ,
rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" ,
{ 547508772U , 1492146221U , 2226483603U , 3783559543U } , &
mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const
rtwCAPI_ModelMappingStaticInfo * VehiclePlant_GetCAPIStaticMap ( void ) {
return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void VehiclePlant_InitializeSystemRan ( hl0stsvgru * const pyftoj5or2
, sysRanDType * systemRan [ ] , j0qkcyobx0 * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( pyftoj5or2 ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemTid [ 1 ] = pyftoj5or2 ->
Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ;
rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void VehiclePlant_InitializeDataMapInfo ( hl0stsvgru * const pyftoj5or2 ,
j0qkcyobx0 * localDW , void * sysRanPtr , int_T contextTid ) {
rtwCAPI_SetVersion ( pyftoj5or2 -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( pyftoj5or2 -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( pyftoj5or2 -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( pyftoj5or2 -> DataMapInfo . mmi ,
( NULL ) ) ; rtwCAPI_SetFullPath ( pyftoj5or2 -> DataMapInfo . mmi , ( NULL )
) ; rtwCAPI_SetInstanceLoggingInfo ( pyftoj5or2 -> DataMapInfo . mmi , &
pyftoj5or2 -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( pyftoj5or2 -> DataMapInfo . mmi , 3 ) ; VehiclePlant_InitializeSystemRan ( pyftoj5or2 , pyftoj5or2 -> DataMapInfo . systemRan , localDW , pyftoj5or2 -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( pyftoj5or2 -> DataMapInfo . mmi , & pyftoj5or2 -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void VehiclePlant_host_InitializeDataMapInfo ( VehiclePlant_host_DataMapInfo_T
* dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ;
rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ;
rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath
( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) )
; dataMap -> childMMI [ 0 ] = & ( dataMap -> child0 . mmi ) ;
environment_host_InitializeDataMapInfo ( & ( dataMap -> child0 ) ,
"VehiclePlant/environment" ) ; dataMap -> childMMI [ 1 ] = & ( dataMap ->
child1 . mmi ) ; hexarotor_host_InitializeDataMapInfo ( & ( dataMap -> child1
) , "VehiclePlant/plant/hexarotor" ) ; dataMap -> childMMI [ 2 ] = & ( dataMap
-> child2 . mmi ) ; sensors_host_InitializeDataMapInfo ( & ( dataMap ->
child2 ) , "VehiclePlant/sensors" ) ; rtwCAPI_SetChildMMIArray ( dataMap ->
mmi , dataMap -> childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi ,
3 ) ; }
#ifdef __cplusplus
}
#endif
#endif
