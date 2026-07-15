#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "Air_capi_host.h"
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
#include "Air.h"
#include "Air_capi.h"
#include "Air_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif
static rtwCAPI_Signals rtBlockSignals [ ] = { { 0 , 0 , ( NULL ) , ( NULL ) ,
0 , 0 , 0 , 0 , 0 } } ; static rtwCAPI_States rtBlockStates [ ] = { { 0 , 21
, TARGET_STRING ( "Air/gust/Begin terminating gust after the gust duration/Distance into\ngust (x)/Distance into\nGust (x)\n(Limited to gust length d)" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 1 , 20 , TARGET_STRING ( "Air/gust/Begin terminating gust after the gust duration/Distance into\ngust (y)/Distance into\nGust (x)\n(Limited to gust length d)\n" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 2 , 19 , TARGET_STRING ( "Air/gust/Begin terminating gust after the gust duration/Distance into\ngust (z)/Distance into\nGust (x)\n(Limited to gust length d)\n" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 3 , 18 , TARGET_STRING ( "Air/gust/Initiate Gust/Distance into\ngust (x)/Distance into\nGust (x)\n(Limited to gust length d)" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 4 , 17 , TARGET_STRING ( "Air/gust/Initiate Gust/Distance into\ngust (y)/Distance into\nGust (x)\n(Limited to gust length d)\n" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 5 , 16 , TARGET_STRING ( "Air/gust/Initiate Gust/Distance into\ngust (z)/Distance into\nGust (x)\n(Limited to gust length d)\n" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 0 , 0 , 0 , 1 , - 1 , 0 } , { 6 , 14 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on angular rates/Hpgw/pgw_p" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 7 , 12 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on angular rates/Hqgw/qgw_p" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 8 , 10 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on angular rates/Hrgw/rgw_p" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 9 , 8 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on velocities/Hugw(s)/ug_p" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 10 , 4 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on velocities/Hvgw(s)/vg_p1" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 11 , 6 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on velocities/Hvgw(s)/vgw_p2" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 12 , 0 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on velocities/Hwgw(s)/wg_p1" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 13 , 2 , TARGET_STRING ( "Air/turbulence/Dryden Wind Turbulence Model \n(Continuous (+q +r))/Filters on velocities/Hwgw(s)/wg_p2" ) , TARGET_STRING ( "" ) , TARGET_STRING ( "" ) , 0 , 0 , 1 , 0 , 0 , 1 , - 1 , 0 } , { 0 , - 1 , ( NULL ) , ( NULL ) , ( NULL ) , 0 , 0 , 0 , 0 , 0 , 0 , - 1 , 0 } } ; static int_T rt_LoggedStateIdxList [ ] = { 13 , 12 , 11 , 10 , 9 , 8 , 7 , 6 , 5 , 4 , 2 , 3 , 0 , 1 } ;
#ifndef HOST_CAPI_BUILD
static void Air_InitializeDataAddr ( void * dataAddr [ ] , ia44hvjo5y *
localDW , pjetdmxrlz * localX ) { dataAddr [ 0 ] = ( void * ) ( & localX ->
hcrr45luqwm . lspp2304ya ) ; dataAddr [ 1 ] = ( void * ) ( & localX ->
irpvfptwfro . kschmsqcob ) ; dataAddr [ 2 ] = ( void * ) ( & localX ->
cp345krnyk . kschmsqcob ) ; dataAddr [ 3 ] = ( void * ) ( & localX ->
fj0c5tjfqn . lspp2304ya ) ; dataAddr [ 4 ] = ( void * ) ( & localX ->
kf3n24jn2o . kschmsqcob ) ; dataAddr [ 5 ] = ( void * ) ( & localX ->
gxezufcodw . kschmsqcob ) ; dataAddr [ 6 ] = ( void * ) ( & localX ->
etet3jaeye [ 0 ] ) ; dataAddr [ 7 ] = ( void * ) ( & localX -> jbqlhg13ze [ 0
] ) ; dataAddr [ 8 ] = ( void * ) ( & localX -> mfmpwdweqf [ 0 ] ) ; dataAddr
[ 9 ] = ( void * ) ( & localX -> avoxcbo1i4 [ 0 ] ) ; dataAddr [ 10 ] = ( void
* ) ( & localX -> coimpmfbov [ 0 ] ) ; dataAddr [ 11 ] = ( void * ) ( &
localX -> jqaupvvrr4 [ 0 ] ) ; dataAddr [ 12 ] = ( void * ) ( & localX ->
pjwr2w12wd [ 0 ] ) ; dataAddr [ 13 ] = ( void * ) ( & localX -> i2oncsl4wy [
0 ] ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void Air_InitializeVarDimsAddr ( int32_T * vardimsAddr [ ] ) {
vardimsAddr [ 0 ] = ( NULL ) ; }
#endif
#ifndef HOST_CAPI_BUILD
static void Air_InitializeLoggingFunctions ( RTWLoggingFcnPtr loggingPtrs [ ]
) { loggingPtrs [ 0 ] = ( NULL ) ; loggingPtrs [ 1 ] = ( NULL ) ; loggingPtrs
[ 2 ] = ( NULL ) ; loggingPtrs [ 3 ] = ( NULL ) ; loggingPtrs [ 4 ] = ( NULL
) ; loggingPtrs [ 5 ] = ( NULL ) ; loggingPtrs [ 6 ] = ( NULL ) ; loggingPtrs
[ 7 ] = ( NULL ) ; loggingPtrs [ 8 ] = ( NULL ) ; loggingPtrs [ 9 ] = ( NULL
) ; loggingPtrs [ 10 ] = ( NULL ) ; loggingPtrs [ 11 ] = ( NULL ) ;
loggingPtrs [ 12 ] = ( NULL ) ; loggingPtrs [ 13 ] = ( NULL ) ; }
#endif
static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap [ ] = { { "double" ,
"real_T" , 0 , 0 , sizeof ( real_T ) , ( uint8_T ) SS_DOUBLE , 0 , 0 , 0 } }
;
#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif
static TARGET_CONST rtwCAPI_ElementMap rtElementMap [ ] = { { ( NULL ) , 0 ,
0 , 0 , 0 } , } ; static rtwCAPI_DimensionMap rtDimensionMap [ ] = { {
rtwCAPI_SCALAR , 0 , 2 , 0 } , { rtwCAPI_VECTOR , 2 , 2 , 0 } } ; static
uint_T rtDimensionArray [ ] = { 1 , 1 , 2 , 1 } ; static const real_T
rtcapiStoredFloats [ ] = { 0.0 } ; static rtwCAPI_FixPtMap rtFixPtMap [ ] = {
{ ( NULL ) , ( NULL ) , rtwCAPI_FIX_RESERVED , 0 , 0 , ( boolean_T ) 0 } , }
; static rtwCAPI_SampleTimeMap rtSampleTimeMap [ ] = { { ( const void * ) &
rtcapiStoredFloats [ 0 ] , ( const void * ) & rtcapiStoredFloats [ 0 ] , ( int8_T ) 0 , ( uint8_T ) 0 } } ; static int_T rtContextSystems [ 23 ] ; static rtwCAPI_LoggingMetaInfo loggingMetaInfo [ ] = { { 0 , 0 , "" , 0 } } ; static rtwCAPI_ModelMapLoggingStaticInfo mmiStaticInfoLogging = { 23 , rtContextSystems , loggingMetaInfo , 0 , ( NULL ) , { 0 , ( NULL ) , ( NULL ) } , 0 , ( NULL ) } ; static rtwCAPI_ModelMappingStaticInfo mmiStatic = { { rtBlockSignals , 0 , ( NULL ) , 0 , ( NULL ) , 0 } , { ( NULL ) , 0 , ( NULL ) , 0 } , { rtBlockStates , 14 } , { rtDataTypeMap , rtDimensionMap , rtFixPtMap , rtElementMap , rtSampleTimeMap , rtDimensionArray } , "float" , { 4096184656U , 1329830706U , 4038942830U , 159971629U } , & mmiStaticInfoLogging , 0 , ( boolean_T ) 0 , rt_LoggedStateIdxList } ; const rtwCAPI_ModelMappingStaticInfo * Air_GetCAPIStaticMap ( void ) { return & mmiStatic ; }
#ifndef HOST_CAPI_BUILD
static void Air_InitializeSystemRan ( eyx3wv2dh2 * const nvfdhrszo5 ,
sysRanDType * systemRan [ ] , ia44hvjo5y * localDW , int_T systemTid [ ] ,
void * rootSysRanPtr , int rootTid ) { UNUSED_PARAMETER ( nvfdhrszo5 ) ;
UNUSED_PARAMETER ( localDW ) ; systemRan [ 0 ] = ( sysRanDType * )
rootSysRanPtr ; systemRan [ 1 ] = ( NULL ) ; systemRan [ 2 ] = ( sysRanDType
* ) & localDW -> hcrr45luqwm . oksyj0jv2v ; systemRan [ 3 ] = ( sysRanDType *
) & localDW -> irpvfptwfro . jh2h5ufsn0 ; systemRan [ 4 ] = ( sysRanDType * )
& localDW -> cp345krnyk . jh2h5ufsn0 ; systemRan [ 5 ] = ( sysRanDType * ) &
localDW -> fj0c5tjfqn . oksyj0jv2v ; systemRan [ 6 ] = ( sysRanDType * ) &
localDW -> kf3n24jn2o . jh2h5ufsn0 ; systemRan [ 7 ] = ( sysRanDType * ) &
localDW -> gxezufcodw . jh2h5ufsn0 ; systemRan [ 8 ] = ( sysRanDType * ) &
localDW -> fodof2x2t4 ; systemRan [ 9 ] = ( sysRanDType * ) & localDW ->
gedgrbiut4 ; systemRan [ 10 ] = ( sysRanDType * ) & localDW -> k1kdhj0lsd ;
systemRan [ 11 ] = ( sysRanDType * ) & localDW -> ic0ali3zew ; systemRan [ 12
] = ( sysRanDType * ) & localDW -> dkukdnfsuk ; systemRan [ 13 ] = ( sysRanDType
* ) & localDW -> pntrgqb3jy ; systemRan [ 14 ] = ( sysRanDType * ) & localDW
-> ls4kddknva ; systemRan [ 15 ] = ( sysRanDType * ) & localDW -> dj24cjnutv
; systemRan [ 16 ] = ( sysRanDType * ) & localDW -> gg0vkz2ya5 ; systemRan [
17 ] = ( sysRanDType * ) & localDW -> jcuathgxxh ; systemRan [ 18 ] = ( sysRanDType * ) & localDW -> bpivcbzlyi ; systemRan [ 19 ] = ( sysRanDType * ) & localDW -> isiljl3bu1 ; systemRan [ 20 ] = ( sysRanDType * ) & localDW -> obi4asdoze ; systemRan [ 21 ] = ( sysRanDType * ) & localDW -> irozwsyzkd ; systemRan [ 22 ] = ( NULL ) ; systemTid [ 1 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 2 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 5 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 3 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 4 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 6 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 7 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 8 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 9 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 10 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 11 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 12 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 13 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 14 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 16 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 17 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 15 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 19 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 20 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 18 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 21 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 22 ] = nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ; systemTid [ 0 ] = rootTid ; rtContextSystems [ 0 ] = 0 ; rtContextSystems [ 1 ] = 0 ; rtContextSystems [ 2 ] = 2 ; rtContextSystems [ 3 ] = 3 ; rtContextSystems [ 4 ] = 4 ; rtContextSystems [ 5 ] = 5 ; rtContextSystems [ 6 ] = 6 ; rtContextSystems [ 7 ] = 7 ; rtContextSystems [ 8 ] = 8 ; rtContextSystems [ 9 ] = 9 ; rtContextSystems [ 10 ] = 10 ; rtContextSystems [ 11 ] = 11 ; rtContextSystems [ 12 ] = 12 ; rtContextSystems [ 13 ] = 13 ; rtContextSystems [ 14 ] = 14 ; rtContextSystems [ 15 ] = 15 ; rtContextSystems [ 16 ] = 16 ; rtContextSystems [ 17 ] = 17 ; rtContextSystems [ 18 ] = 18 ; rtContextSystems [ 19 ] = 19 ; rtContextSystems [ 20 ] = 20 ; rtContextSystems [ 21 ] = 21 ; rtContextSystems [ 22 ] = 0 ; }
#endif
#ifndef HOST_CAPI_BUILD
void Air_InitializeDataMapInfo ( eyx3wv2dh2 * const nvfdhrszo5 , ia44hvjo5y *
localDW , pjetdmxrlz * localX , void * sysRanPtr , int_T contextTid ) {
rtwCAPI_SetVersion ( nvfdhrszo5 -> DataMapInfo . mmi , 1 ) ;
rtwCAPI_SetStaticMap ( nvfdhrszo5 -> DataMapInfo . mmi , & mmiStatic ) ;
rtwCAPI_SetLoggingStaticMap ( nvfdhrszo5 -> DataMapInfo . mmi , &
mmiStaticInfoLogging ) ; Air_InitializeDataAddr ( nvfdhrszo5 -> DataMapInfo .
dataAddress , localDW , localX ) ; rtwCAPI_SetDataAddressMap ( nvfdhrszo5 ->
DataMapInfo . mmi , nvfdhrszo5 -> DataMapInfo . dataAddress ) ;
Air_InitializeVarDimsAddr ( nvfdhrszo5 -> DataMapInfo . vardimsAddress ) ;
rtwCAPI_SetVarDimsAddressMap ( nvfdhrszo5 -> DataMapInfo . mmi , nvfdhrszo5
-> DataMapInfo . vardimsAddress ) ; rtwCAPI_SetPath ( nvfdhrszo5 ->
DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetFullPath ( nvfdhrszo5 ->
DataMapInfo . mmi , ( NULL ) ) ; Air_InitializeLoggingFunctions ( nvfdhrszo5
-> DataMapInfo . loggingPtrs ) ; rtwCAPI_SetLoggingPtrs ( nvfdhrszo5 ->
DataMapInfo . mmi , nvfdhrszo5 -> DataMapInfo . loggingPtrs ) ;
rtwCAPI_SetInstanceLoggingInfo ( nvfdhrszo5 -> DataMapInfo . mmi , &
nvfdhrszo5 -> DataMapInfo . mmiLogInstanceInfo ) ; rtwCAPI_SetChildMMIArray ( nvfdhrszo5 -> DataMapInfo . mmi , ( NULL ) ) ; rtwCAPI_SetChildMMIArrayLen ( nvfdhrszo5 -> DataMapInfo . mmi , 0 ) ; Air_InitializeSystemRan ( nvfdhrszo5 , nvfdhrszo5 -> DataMapInfo . systemRan , localDW , nvfdhrszo5 -> DataMapInfo . systemTid , sysRanPtr , contextTid ) ; rtwCAPI_SetSystemRan ( nvfdhrszo5 -> DataMapInfo . mmi , nvfdhrszo5 -> DataMapInfo . systemRan ) ; rtwCAPI_SetSystemTid ( nvfdhrszo5 -> DataMapInfo . mmi , nvfdhrszo5 -> DataMapInfo . systemTid ) ; rtwCAPI_SetGlobalTIDMap ( nvfdhrszo5 -> DataMapInfo . mmi , & nvfdhrszo5 -> Timing . mdlref_GlobalTID [ 0 ] ) ; }
#else
#ifdef __cplusplus
extern "C" {
#endif
void Air_host_InitializeDataMapInfo ( Air_host_DataMapInfo_T * dataMap ,
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
