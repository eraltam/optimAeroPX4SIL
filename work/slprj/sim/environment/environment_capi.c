#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "environment_capi_host.h"
#define sizeof(...) ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el) ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s) (s)
#ifndef SS_UINT64
#define SS_UINT64 31
#endif
#ifndef SS_INT64
#define SS_INT64 32
#endif
#else
#include "builtin_typeid_types.h"
#include "environment.h"
#include "environment_capi.h"
#include "environment_private.h"
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
{ 821018902U , 555582222U , 3511052456U , 150356172U } , &
mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const
rtwCAPI_ModelMappingStaticInfo * environment_GetCAPIStaticMap ( void ) {
return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void environment_InitializeSystemRan ( mojmojnvtk * const chs3edang5 ,
sysRanDType * systemRan [ ] , otxhehhi2w * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( chs3edang5 ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemTid [ 1 ] = chs3edang5 ->
Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ;
rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void environment_InitializeDataMapInfo ( mojmojnvtk * const chs3edang5 ,
otxhehhi2w * localDW , void * sysRanPtr , int_T contextTid ) {
rtwCAPI_SetVersion ( chs3edang5 -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( chs3edang5 -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( chs3edang5 -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; rtwCAPI_SetPath ( chs3edang5 -> DataMapInfo . mmi ,
( NULL ) ) ; rtwCAPI_SetFullPath ( chs3edang5 -> DataMapInfo . mmi , ( NULL )
) ; rtwCAPI_SetInstanceLoggingInfo ( chs3edang5 -> DataMapInfo . mmi , &
chs3edang5 -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( chs3edang5 -> DataMapInfo . mmi , chs3edang5 -> DataMapInfo . childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( chs3edang5 -> DataMapInfo . mmi , 3 ) ; environment_InitializeSystemRan ( chs3edang5 , chs3edang5 -> DataMapInfo . systemRan , localDW , chs3edang5 -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( chs3edang5 -> DataMapInfo . mmi , chs3edang5 -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( chs3edang5 -> DataMapInfo . mmi , chs3edang5 -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( chs3edang5 -> DataMapInfo . mmi , & chs3edang5 -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void environment_host_InitializeDataMapInfo ( environment_host_DataMapInfo_T
* dataMap , const char * path ) { rtwCAPI_SetVersion ( dataMap -> mmi , 1 ) ;
rtwCAPI_SetStaticMap ( dataMap -> mmi , & mmiStatic ) ;
rtwCAPI_SetDataAddressMap ( dataMap -> mmi , ( NULL ) ) ;
rtwCAPI_SetVarDimsAddressMap ( dataMap -> mmi , ( NULL ) ) ; rtwCAPI_SetPath
( dataMap -> mmi , path ) ; rtwCAPI_SetFullPath ( dataMap -> mmi , ( NULL ) )
; dataMap -> childMMI [ 0 ] = & ( dataMap -> child0 . mmi ) ;
Air_host_InitializeDataMapInfo ( & ( dataMap -> child0 ) , "environment/air"
) ; dataMap -> childMMI [ 1 ] = & ( dataMap -> child1 . mmi ) ;
Earth_host_InitializeDataMapInfo ( & ( dataMap -> child1 ) ,
"environment/earth" ) ; dataMap -> childMMI [ 2 ] = & ( dataMap -> child2 .
mmi ) ; LocalTerrain_host_InitializeDataMapInfo ( & ( dataMap -> child2 ) ,
"environment/terrain" ) ; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , dataMap
-> childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 3 ) ; }
#ifdef __cplusplus
}
#endif
#endif
