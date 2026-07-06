#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "ins_anello_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 27
#endif
#ifndef SS_INT64
#define SS_INT64 28
#endif
#else
#include "builtin_typeid_types.h"
#include "ins_anello.h"
#include "ins_anello_capi.h"
#include "ins_anello_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static const rtwCAPI_SignalHierLoggingInfo rtSigHierLoggingInfo [ ] = { { ""
, 5 , 0 } , { "signal1" , 0 , - 1 } , { "signal2" , 0 , - 1 } , { "signal3" ,
0 , - 1 } , { "signal4" , 0 , - 1 } , { "signal5" , 0 , - 1 } , { "" , 5 , 5
} , { "" , 5 , 10 } } ; static const uint_T rtSigHierLoggingChildIdxs [ ] = {
1 , 2 , 3 , 4 , 5 , 1 , 2 , 3 , 4 , 5 , 1 , 2 , 3 , 4 , 5 } ; static
rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , ( NULL ) , ( NULL ) , 0 , 0
, 0 , 0 , 0 } } ; static rtwCAPI_States rtBlockStates [ ] = { { 0 , - 1 , ( NULL
) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ; static int_T
rt_LoggedStateIdxList [ ] = { - 1 } ; static TARGET_CONST rtwCAPI_DataTypeMap
rtDataTypeMap [ ] = { { "" , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 } } ;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 0 , 0 } } ; static uint_T rtDimensionArray [ ] = { 0 } ;
static rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) ,
rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , } ; static
rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( NULL ) , ( NULL ) , 0 , 0 }
} ; static int_T rtContextSystems [ 3 ] ; static rtwCAPI_LoggingMetaInfo
loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static
rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 3 ,
rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 8 ,
rtSigHierLoggingInfo , rtSigHierLoggingChildIdxs } , 0 , ( NULL ) } ; static
rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL )
, 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 0
} , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap ,
rtSampleTimeMap , rtDimensionArray } , "float" , { 3078904131U , 1279732285U
, 2690870308U , 2687956626U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0
, rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo *
ins_anello_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void ins_anello_InitializeSystemRan ( psmsewigyz * const ckf43r0kru ,
sysRanDType * systemRan [ ] , a4mpwoagne * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( ckf43r0kru ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemRan [ 2 ] = ( NULL ) ;
systemTid [ 1 ] = rootTid ; systemTid [ 2 ] = rootTid ; systemTid [ 0 ] =
rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ;
rtContextSystems [ 2 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void ins_anello_InitializeDataMapInfo ( psmsewigyz * const ckf43r0kru ,
a4mpwoagne * localDW , void * sysRanPtr , int_T contextTid ) {
rtwCAPI_SetVersion ( ckf43r0kru -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( ckf43r0kru -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( ckf43r0kru -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( ckf43r0kru -> DataMapInfo . mmi ,
( NULL ) ) ; rtwCAPI_SetFullPath ( ckf43r0kru -> DataMapInfo . mmi , ( NULL )
) ; rtwCAPI_SetInstanceLoggingInfo ( ckf43r0kru -> DataMapInfo . mmi , &
ckf43r0kru -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( ckf43r0kru -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( ckf43r0kru -> DataMapInfo . mmi , 0 ) ; ins_anello_InitializeSystemRan ( ckf43r0kru , ckf43r0kru -> DataMapInfo . systemRan , localDW , ckf43r0kru -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( ckf43r0kru -> DataMapInfo . mmi , ckf43r0kru -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( ckf43r0kru -> DataMapInfo . mmi , ckf43r0kru -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( ckf43r0kru -> DataMapInfo . mmi , & ckf43r0kru -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void ins_anello_host_InitializeDataMapInfo ( ins_anello_host_DataMapInfo_T *
dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ;
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
