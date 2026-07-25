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
#define SS_UINT64 63
#endif
#ifndef SS_INT64
#define SS_INT64 64
#endif
#else
#include "builtin_typeid_types.h"
#include "VehiclePlant.h"
#include "VehiclePlant_capi.h"
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
, { "ServosCommandFixedwingPlaneBus" , 3 , 4 } , { "cmdAileron_nd" , 0 , - 1
} , { "cmdElevator_nd" , 0 , - 1 } , { "cmdRudder_nd" , 0 , - 1 } , {
"EngineCommandBus" , 1 , 7 } , { "cmdThrottle_unit" , 0 , - 1 } } ; static
const uint_T rtSigHierLoggingChildIdxs [ ] = { 1 , 2 , 7 , 3 , 4 , 5 , 6 , 8
} ; static rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , ( NULL ) , ( NULL
) , 0 , 0 , 0 , 0 , 0 } } ; static rtwCAPI_States rtBlockStates [ ] = { { 0 ,
- 1 , TARGET_STRING ( "VehiclePlant/plant/c172pJSBSim/Variable Integer Delay"
) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 } , { 1 ,
- 1 , TARGET_STRING ( "VehiclePlant/plant/c172pJSBSim/Variable Integer Delay"
) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 } , { 2 ,
- 1 , TARGET_STRING ( "VehiclePlant/plant/c172pJSBSim/Variable Integer Delay"
) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 2 } , { 3 ,
- 1 , TARGET_STRING ( "VehiclePlant/plant/c172pJSBSim/Variable Integer Delay"
) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 3 } , { 4 ,
- 1 , TARGET_STRING ( "VehiclePlant/plant/c172pJSBSim/Variable Integer Delay"
) , TARGET_STRING ( "DSTATE" ) , "" , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 4 } , { 0 ,
- 1 , ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ;
static int_T rt_LoggedStateIdxList [ ] = { 2 , 3 , 4 , 5 , 6 } ;
#ifndef HOST_CAPI_BUILD
static void VehiclePlant_InitializeDataAddr ( void * dataAddr [ ] ) {
dataAddr [ 0 ] = ( void * ) ( & gqrn4faltve . dv0zw01afq [ 0 ] ) ; dataAddr [
1 ] = ( void * ) ( & gqrn4faltve . ko10qxkwp1 [ 0 ] ) ; dataAddr [ 2 ] = ( void
* ) ( & gqrn4faltve . jqwrh133to [ 0 ] ) ; dataAddr [ 3 ] = ( void * ) ( &
gqrn4faltve . n4cn0u2stm [ 0 ] ) ; dataAddr [ 4 ] = ( void * ) ( &
gqrn4faltve . gfshgcqqtd [ 0 ] ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void VehiclePlant_InitializeVarDimsAddr ( int32_T * vardimsAddr [ ] )
{ vardimsAddr [ 0 ] = ( NULL ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void VehiclePlant_InitializeLoggingFunctions ( RTWLoggingFcnPtr
loggingPtrs [ ] ) { loggingPtrs [ 0 ] = ( NULL ) ; loggingPtrs [ 1 ] = ( NULL
) ; loggingPtrs [ 2 ] = ( NULL ) ; loggingPtrs [ 3 ] = ( NULL ) ; loggingPtrs
[ 4 ] = ( NULL ) ; }
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , ( uint8_T ) SS_DOUBLE , 0 , 0 , 0 } }
;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_VECTOR , 0 , 2 , 0 } } ; static uint_T rtDimensionArray [ ] = { 100 ,
1 } ; static const real_T rtcapiStoredFloats [ ] = { 0.004 , 0.0 } ; static
rtwCAPI_FixPtMap rtFixPtMap [ ] = { { ( NULL ) , ( NULL ) ,
rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , } ; static
rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( const void * ) &
rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 1 ] , ( int8_T ) 1 , ( uint8_T ) 0 } } ; static int_T rtContextSystems [ 4 ] ; static rtwCAPI_LoggingMetaInfo loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 4 , rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 9 , rtSigHierLoggingInfo , rtSigHierLoggingChildIdxs } , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 5 } , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" , { 3225534471U , 3759379419U , 584515214U , 3126194833U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo * VehiclePlant_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void VehiclePlant_InitializeSystemRan ( hl0stsvgru * const pyftoj5or2
, sysRanDType * systemRan [ ] , int_T systemTid [ ] , void * rootSysRanPtr ,
int rootTid ) { UNUSED_PARAMETER ( pyftoj5or2 ) ; systemRan [ 0 ] = ( sysRanDType * ) rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemRan [ 2 ] = ( NULL ) ; systemRan [ 3 ] = ( NULL ) ; systemTid [ 1 ] = oga1twjs0f [ 0 ] ; systemTid [ 2 ] = oga1twjs0f [ 1 ] ; systemTid [ 3 ] = oga1twjs0f [ 0 ] ; systemTid [ 0 ] = rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; rtContextSystems [ 2 ] = 0 ; rtContextSystems [ 3 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void VehiclePlant_InitializeDataMapInfo ( hl0stsvgru * const pyftoj5or2 ,
void * sysRanPtr , int_T contextTid ) { rtwCAPI_SetVersion ( pyftoj5or2 ->
DataMapInfo . mmi , 1 ) ; rtwCAPI_SetStaticMap ( pyftoj5or2 -> DataMapInfo .
mmi , & mmiStatic ) ; rtwCAPI_SetLoggingStaticMap ( pyftoj5or2 -> DataMapInfo
. mmi , & mmiStaticInfoLogging ) ; VehiclePlant_InitializeDataAddr ( pyftoj5or2
-> DataMapInfo . dataAddress ) ; rtwCAPI_SetDataAddressMap ( pyftoj5or2 ->
DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . dataAddress ) ;
VehiclePlant_InitializeVarDimsAddr ( pyftoj5or2 -> DataMapInfo .
vardimsAddress ) ; rtwCAPI_SetVarDimsAddressMap ( pyftoj5or2 -> DataMapInfo .
mmi , pyftoj5or2 -> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetPath ( pyftoj5or2 -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetFullPath ( pyftoj5or2 -> DataMapInfo . mmi , ( NULL ) ) ; VehiclePlant_InitializeLoggingFunctions ( pyftoj5or2 -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetLoggingPtrs ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetInstanceLoggingInfo ( pyftoj5or2 -> DataMapInfo . mmi , & pyftoj5or2 -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( pyftoj5or2 -> DataMapInfo . mmi , 3 ) ; VehiclePlant_InitializeSystemRan ( pyftoj5or2 , pyftoj5or2 -> DataMapInfo . systemRan , pyftoj5or2 -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( pyftoj5or2 -> DataMapInfo . mmi , pyftoj5or2 -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( pyftoj5or2 -> DataMapInfo . mmi , & oga1twjs0f [ 0 ] ) ; }
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
child1 . mmi ) ; actuatorsC172p_host_InitializeDataMapInfo ( & ( dataMap ->
child1 ) , "VehiclePlant/plant/c172pJSBSim/actuators" ) ; dataMap -> childMMI
[ 2 ] = & ( dataMap -> child2 . mmi ) ; sensors_host_InitializeDataMapInfo ( &
( dataMap -> child2 ) , "VehiclePlant/sensors" ) ; rtwCAPI_SetChildMMIArray ( dataMap -> mmi , dataMap -> childMMI ) ; rtwCAPI_SetChildMMIArrayLen ( dataMap -> mmi , 3 ) ; }
#ifdef __cplusplus
}
#endif
#endif
