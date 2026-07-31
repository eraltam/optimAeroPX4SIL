#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "sensors_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 50
#endif
#ifndef SS_INT64
#define SS_INT64 51
#endif
#else
#include "builtin_typeid_types.h"
#include "sensors.h"
#include "sensors_capi.h"
#include "sensors_private.h"
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
{ 3317883028U , 848459731U , 4138890812U , 1451070328U } , &
mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const
rtwCAPI_ModelMappingStaticInfo * sensors_GetCAPIStaticMap ( void ) { return &
mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void sensors_InitializeSystemRan ( fcgqozqvuz * const nxtbamgl1k ,
sysRanDType * systemRan [ ] , e2ptrxcrjh * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( nxtbamgl1k ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemTid [ 1 ] = nxtbamgl1k ->
Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ;
rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void sensors_InitializeDataMapInfo ( fcgqozqvuz * const nxtbamgl1k ,
e2ptrxcrjh * localDW , void * sysRanPtr , int_T contextTid ) {
rtwCAPI_SetVersion ( nxtbamgl1k -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( nxtbamgl1k -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( nxtbamgl1k -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( nxtbamgl1k -> DataMapInfo . mmi ,
( NULL ) ) ; rtwCAPI_SetFullPath ( nxtbamgl1k -> DataMapInfo . mmi , ( NULL )
) ; rtwCAPI_SetInstanceLoggingInfo ( nxtbamgl1k -> DataMapInfo . mmi , &
nxtbamgl1k -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( nxtbamgl1k -> DataMapInfo . mmi , nxtbamgl1k -> DataMapInfo . childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( nxtbamgl1k -> DataMapInfo . mmi , 4 ) ; sensors_InitializeSystemRan ( nxtbamgl1k , nxtbamgl1k -> DataMapInfo . systemRan , localDW , nxtbamgl1k -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( nxtbamgl1k -> DataMapInfo . mmi , nxtbamgl1k -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( nxtbamgl1k -> DataMapInfo . mmi , nxtbamgl1k -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( nxtbamgl1k -> DataMapInfo . mmi , & nxtbamgl1k -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void sensors_host_InitializeDataMapInfo ( sensors_host_DataMapInfo_T *
dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ;
rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ;
rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath
( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) )
; dataMap -> childMMI [ 0 ] = & ( dataMap -> child0 . mmi ) ;
adc_host_InitializeDataMapInfo ( & ( dataMap -> child0 ) , "sensors/adc" ) ;
dataMap -> childMMI [ 1 ] = & ( dataMap -> child1 . mmi ) ;
gps_host_InitializeDataMapInfo ( & ( dataMap -> child1 ) , "sensors/gps" ) ;
dataMap -> childMMI [ 2 ] = & ( dataMap -> child2 . mmi ) ;
ins_anello_host_InitializeDataMapInfo ( & ( dataMap -> child2 ) ,
"sensors/ins/AnelloX3" ) ; dataMap -> childMMI [ 3 ] = & ( dataMap -> child3
. mmi ) ; ins_host_InitializeDataMapInfo ( & ( dataMap -> child3 ) ,
"sensors/ins/Generic" ) ; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , dataMap
-> childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 4 ) ; }
#ifdef __cplusplus
}
#endif
#endif
