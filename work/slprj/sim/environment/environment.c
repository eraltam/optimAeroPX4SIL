#include "environment.h"
#include "rtwtypes.h"
#include "environment_capi.h"
#include "environment_private.h"
#include <string.h>
#include "Air.h"
#include "Earth.h"
#include "LocalTerrain.h"
static RegMdlInfo rtMdlInfo_environment [ 58 ] = { { "lj3yonrq1cm" ,
MDL_INFO_NAME_MDLREF_DWORK , 0 , - 1 , ( void * ) "environment" } , {
"aeypz01neu" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "mbpk5tldh3" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "by5ve2ox1z" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"fawde2v4ql" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "djcyjwlyeb" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "fgdgb2ruqd" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"ar45jsrhtq" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "isjgmujhyd" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "grcefi3vak" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"dsukku0ybc" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "krumeuy3jd" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "otxhehhi2w" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"ehxtqyeepi" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "kjqozb5mog" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "mzncbscp1i" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"h04mg41egj" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "modkwf3jsj" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "eur0hdmr0t" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"jcjsocq2aa" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "cvgtj0ljml" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "dqw2pebsxa" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"d2n3xqbspm" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "frbuztrvod" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "environment" } , { "btkehhwypc" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"environment" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , 0 , ( NULL ) } , {
"b5gqldnz3r" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "kzt5n3i0swp" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "environment" } , { "cgjzqe2tvy" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "environment" } , {
"mojmojnvtk" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"environment" } , { "EnvironmentBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL
) } , { "VehicleBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"AirDataBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"AircraftForcesMomentsBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"ComponentForcesMomentsBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } ,
{ "MassPropertiesBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"VehicleConfigurationBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"TerrainEnvironmentBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"a3tgdgrc1z2" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"EarthEnvironmentBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"kzo2mbsde2x" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"BodyStateBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"AirEnvironmentBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"pp2a15jtu4d" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"mr_environment_GetSimStateDisallowedBlocks" , MDL_INFO_ID_MODEL_FCN_NAME , 0
, - 1 , ( void * ) "environment" } , {
"mr_environment_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "environment" } , {
"mr_environment_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "environment" } , {
"mr_environment_restoreDataFromMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "environment" } , {
"mr_environment_cacheDataToMxArrayWithOffset" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "environment" } , {
"mr_environment_extractBitFieldFromMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0
, - 1 , ( void * ) "environment" } , {
"mr_environment_cacheBitFieldToMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , -
1 , ( void * ) "environment" } , { "mr_environment_restoreDataFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "environment" } , {
"mr_environment_cacheDataAsMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 ,
( void * ) "environment" } , { "mr_environment_RegisterSimStateChecksum" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "environment" } , {
"mr_environment_SetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * )
"environment" } , { "mr_environment_GetDWork" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "environment" } , { "environment.h" ,
MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( NULL ) } , { "environment.c" ,
MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void * ) "environment" } } ; void
btkehhwypc ( mojmojnvtk * const chs3edang5 , otxhehhi2w * localDW ) {
simTgtPushModelBlockPath ( chs3edang5 -> _mdlRefSfcnS , "environment/earth" )
; fgj5iijlyx ( & ( localDW -> onovmkdvpt . rtm ) , & ( localDW -> onovmkdvpt
. rtdw ) ) ; simTgtPopModelBlockPath ( chs3edang5 -> _mdlRefSfcnS ,
"environment/earth" ) ; } void dqw2pebsxa ( real_T k0jizrqbpm [ 3 ] , real_T
htdjgzerpb [ 3 ] , real_T * jvxwvnqbed , otxhehhi2w * localDW , ar45jsrhtq *
localX ) { eml2bfmjzs ( & k0jizrqbpm [ 0 ] , & htdjgzerpb [ 0 ] , & ( localDW
-> b44lmhfkkz . rtb ) , & ( localDW -> b44lmhfkkz . rtdw ) , & ( localX ->
fimefkpimo ) ) ; h0xamgxegy ( & ( localDW -> onovmkdvpt . rtb ) , & ( localDW
-> onovmkdvpt . rtdw ) ) ; kcrhqktkmm ( jvxwvnqbed ) ; } void cvgtj0ljml ( otxhehhi2w * localDW ) { hhmbei1r1d ( & ( localDW -> onovmkdvpt . rtdw ) ) ; } void modkwf3jsj ( otxhehhi2w * localDW ) { lpjxqqjfle ( & ( localDW -> b44lmhfkkz . rtm ) , & ( localDW -> b44lmhfkkz . rtb ) , & ( localDW -> b44lmhfkkz . rtdw ) ) ; jvoajx2hic ( & ( localDW -> onovmkdvpt . rtb ) , & ( localDW -> onovmkdvpt . rtdw ) ) ; } void frbuztrvod ( real_T * jvxwvnqbed , otxhehhi2w * localDW ) { jgoqiwhsgd ( & ( localDW -> b44lmhfkkz . rtdw ) ) ; n3llx4wcyn ( jvxwvnqbed ) ; } void environment ( mojmojnvtk * const chs3edang5 , const real_T bdoxnlmvb5 [ 3 ] , const real_T nrbkpcwykv [ 3 ] , const real_T hdal4gcyr4 [ 9 ] , real_T * lc2l1ifrdc , real_T jnjk2hkwbs [ 3 ] , real_T iiowwd1ole [ 3 ] , real_T * eh5pp5enrj , real_T * eqzen13jm1 , real_T * o35xut5vvt , real_T nvlkzczvdw [ 3 ] , real_T * fwxuqwezyk , real_T k0jizrqbpm [ 3 ] , real_T htdjgzerpb [ 3 ] , otxhehhi2w * localDW , ar45jsrhtq * localX , djcyjwlyeb * localXdis ) { Air ( & ( localDW -> b44lmhfkkz . rtm ) , & bdoxnlmvb5 [ 0 ] , & nrbkpcwykv [ 0 ] , & hdal4gcyr4 [ 0 ] , eh5pp5enrj , eqzen13jm1 , o35xut5vvt , & nvlkzczvdw [ 0 ] , fwxuqwezyk , & k0jizrqbpm [ 0 ] , & htdjgzerpb [ 0 ] , & ( localDW -> b44lmhfkkz . rtb ) , & ( localDW -> b44lmhfkkz . rtdw ) , & ( localX -> fimefkpimo ) , & ( localXdis -> fimefkpimo ) ) ; if ( rtmIsMajorTimeStep ( chs3edang5 ) && rtmIsSampleHit ( chs3edang5 , 1 , 0 ) ) { Earth ( & ( localDW -> onovmkdvpt . rtm ) , & bdoxnlmvb5 [ 0 ] , lc2l1ifrdc , & jnjk2hkwbs [ 0 ] , & iiowwd1ole [ 0 ] , & ( localDW -> onovmkdvpt . rtb ) , & ( localDW -> onovmkdvpt . rtdw ) ) ; } } void environmentTID3 ( real_T d2bzr133u2 [ 3 ] , real_T * jvxwvnqbed , otxhehhi2w * localDW ) { AirTID3 ( & ( localDW -> b44lmhfkkz . rtm ) , & d2bzr133u2 [ 0 ] , & ( localDW -> b44lmhfkkz . rtb ) , & ( localDW -> b44lmhfkkz . rtdw ) ) ; EarthTID1 ( & ( localDW -> onovmkdvpt . rtb ) ) ; LocalTerrainTID1 ( jvxwvnqbed ) ; } void jcjsocq2aa ( mojmojnvtk * const chs3edang5 , otxhehhi2w * localDW ) { hqhjziniu5 ( & ( localDW -> b44lmhfkkz . rtm ) , & ( localDW -> b44lmhfkkz . rtdw ) ) ; if ( rtmIsMajorTimeStep ( chs3edang5 ) && rtmIsSampleHit ( chs3edang5 , 1 , 0 ) ) { cnzfydxze0 ( & ( localDW -> onovmkdvpt . rtb ) , & ( localDW -> onovmkdvpt . rtdw ) ) ; } } void eur0hdmr0t ( otxhehhi2w * localDW , ar45jsrhtq * localX , fgdgb2ruqd * localXdot ) { nbxqyvr5xm ( & ( localDW -> b44lmhfkkz . rtb ) , & ( localDW -> b44lmhfkkz . rtdw ) , & ( localX -> fimefkpimo ) , & ( localXdot -> fimefkpimo ) ) ; } void h04mg41egj ( otxhehhi2w * localDW ) { a0favbk2uu ( & ( localDW -> onovmkdvpt . rtm ) , & ( localDW -> onovmkdvpt . rtdw ) ) ; } void mzncbscp1i ( otxhehhi2w * localDW , mojmojnvtk * const chs3edang5 ) { f0nfjjh25d ( & ( localDW -> b44lmhfkkz . rtm ) ) ; gyaf1bhjum ( & ( localDW -> onovmkdvpt . rtm ) ) ; nktmhlhgc5 ( & ( localDW -> lbkegtmmtx . rtm ) ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( chs3edang5 -> _mdlRefSfcnS , "environment" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT" ) ; } } void d2n3xqbspm ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , int_T mdlref_TID3 , mojmojnvtk * const chs3edang5 , otxhehhi2w * localDW , ar45jsrhtq * localX , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset ( ( void * ) chs3edang5 , 0 , sizeof ( mojmojnvtk ) ) ; chs3edang5 -> Timing . mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; chs3edang5 -> Timing . mdlref_GlobalTID [ 1 ] = mdlref_TID1 ; chs3edang5 -> Timing . mdlref_GlobalTID [ 2 ] = mdlref_TID2 ; chs3edang5 -> Timing . mdlref_GlobalTID [ 3 ] = mdlref_TID3 ; chs3edang5 -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( chs3edang5 -> _mdlRefSfcnS , "environment" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } ( void ) memset ( ( void * ) localDW , 0 , sizeof ( otxhehhi2w ) ) ; environment_InitializeDataMapInfo ( chs3edang5 , localDW , sysRanPtr , contextTid ) ; d10mathqwe ( _mdlRefSfcnS , mdlref_TID0 , mdlref_TID1 , mdlref_TID2 , mdlref_TID3 , & ( localDW -> b44lmhfkkz . rtm ) , & ( localDW -> b44lmhfkkz . rtb ) , & ( localDW -> b44lmhfkkz . rtdw ) , & ( localX -> fimefkpimo ) , chs3edang5 -> DataMapInfo . systemRan [ 0 ] , chs3edang5 -> DataMapInfo . systemTid [ 0 ] , & ( chs3edang5 -> DataMapInfo . mmi ) , "environment/air" , 0 , 0 ) ; o1evou4ho2 ( _mdlRefSfcnS , mdlref_TID1 , mdlref_TID3 , & ( localDW -> onovmkdvpt . rtm ) , & ( localDW -> onovmkdvpt . rtb ) , & ( localDW -> onovmkdvpt . rtdw ) , chs3edang5 -> DataMapInfo . systemRan [ 0 ] , chs3edang5 -> DataMapInfo . systemTid [ 0 ] , & ( chs3edang5 -> DataMapInfo . mmi ) , "environment/earth" , 1 , - 1 ) ; etquuaa4x0 ( _mdlRefSfcnS , mdlref_TID1 , mdlref_TID3 , & ( localDW -> lbkegtmmtx . rtm ) , chs3edang5 -> DataMapInfo . systemRan [ 0 ] , chs3edang5 -> DataMapInfo . systemTid [ 0 ] , & ( chs3edang5 -> DataMapInfo . mmi ) , "environment/terrain" , 2 , - 1 ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( chs3edang5 -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( chs3edang5 -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( chs3edang5 -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_environment_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { mr_Air_MdlInfoRegFcn ( mdlRefSfcnS , "Air" , retVal ) ; if ( * retVal == 0 ) return ; * retVal = 0 ; mr_Earth_MdlInfoRegFcn ( mdlRefSfcnS , "Earth" , retVal ) ; if ( * retVal == 0 ) return ; * retVal = 0 ; mr_LocalTerrain_MdlInfoRegFcn ( mdlRefSfcnS , "LocalTerrain" , retVal ) ; if ( * retVal == 0 ) return ; * retVal = 0 ; } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_environment , 58 ) ; * retVal = 1 ; } static void mr_environment_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_environment_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_environment_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_environment_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_environment_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_environment_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_environment_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_environment_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_environment_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_environment_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_environment_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_environment_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_environment_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_environment_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_environment_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_environment_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_environment_GetDWork ( const lj3yonrq1cm * mdlrefDW ) { static const char_T * ssDWFieldNames [ 3 ] = { "NULL_rtb" , "rtdw" , "NULL_rtzce" } ; mxArray * ssDW = mxCreateStructMatrix ( 1 , 1 , 3 , ssDWFieldNames ) ; { static const char_T * rtdwDataFieldNames [ 3 ] = { "mdlrefDW_rtdw_b44lmhfkkz" , "mdlrefDW_rtdw_onovmkdvpt" , "mdlrefDW_rtdw_lbkegtmmtx" , } ; mxArray * rtdwData = mxCreateStructMatrix ( 1 , 1 , 3 , rtdwDataFieldNames ) ; { mxArray * varData = mr_Air_GetDWork ( & ( mdlrefDW -> rtdw . b44lmhfkkz ) ) ; mxSetFieldByNumber ( rtdwData , 0 , 0 , varData ) ; } { mxArray * varData = mr_Earth_GetDWork ( & ( mdlrefDW -> rtdw . onovmkdvpt ) ) ; mxSetFieldByNumber ( rtdwData , 0 , 1 , varData ) ; } { mxArray * varData = mr_LocalTerrain_GetDWork ( & ( mdlrefDW -> rtdw . lbkegtmmtx ) ) ; mxSetFieldByNumber ( rtdwData , 0 , 2 , varData ) ; } mxSetFieldByNumber ( ssDW , 0 , 1 , rtdwData ) ; } ( void ) mdlrefDW ; return ssDW ; } void mr_environment_SetDWork ( lj3yonrq1cm * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; { const mxArray * rtdwData = mxGetFieldByNumber ( ssDW , 0 , 1 ) ; mr_Air_SetDWork ( & ( mdlrefDW -> rtdw . b44lmhfkkz ) , mxGetFieldByNumber ( rtdwData , 0 , 0 ) ) ; mr_Earth_SetDWork ( & ( mdlrefDW -> rtdw . onovmkdvpt ) , mxGetFieldByNumber ( rtdwData , 0 , 1 ) ) ; mr_LocalTerrain_SetDWork ( & ( mdlrefDW -> rtdw . lbkegtmmtx ) , mxGetFieldByNumber ( rtdwData , 0 , 2 ) ) ; } } void mr_environment_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 326047323U , 3849478604U , 1232221651U , 3151765326U , } ; slmrModelRefRegisterSimStateChecksum ( S , "environment" , & chksum [ 0 ] ) ; mr_Air_RegisterSimStateChecksum ( S ) ; mr_Earth_RegisterSimStateChecksum ( S ) ; mr_LocalTerrain_RegisterSimStateChecksum ( S ) ; } mxArray * mr_environment_GetSimStateDisallowedBlocks ( ) { mxArray * data = NULL ; size_t numChildrenWithDisallowedBlocks = 0 ; size_t numBlocks = 0 ; mxArray * disallowedBlocksInChild [ 3 ] ; disallowedBlocksInChild [ 0 ] = mr_Air_GetSimStateDisallowedBlocks ( ) ; disallowedBlocksInChild [ 1 ] = mr_Earth_GetSimStateDisallowedBlocks ( ) ; disallowedBlocksInChild [ 2 ] = mr_LocalTerrain_GetSimStateDisallowedBlocks ( ) ; { size_t i ; for ( i = 0 ; i < 3 ; ++ i ) { mxArray * data_i = disallowedBlocksInChild [ i ] ; if ( ( NULL ) != data_i ) { if ( 0 == numChildrenWithDisallowedBlocks ++ ) { data = data_i ; } numBlocks += mxGetM ( data_i ) ; } } } if ( numChildrenWithDisallowedBlocks > 1 ) { mwIndex subs [ 2 ] , offset ; data = mxCreateCellMatrix ( numBlocks , 3 ) ; subs [ 0 ] = 0 ; { size_t i ; for ( i = 0 ; i < 3 ; ++ i ) { mxArray * data_i = disallowedBlocksInChild [ i ] ; if ( ( NULL ) != data_i ) { mwIndex subs_i [ 2 ] , offset_i ; const mwIndex numRows_i = ( mwIndex ) mxGetM ( data_i ) ; for ( subs_i [ 0 ] = 0 ; subs_i [ 0 ] < numRows_i ; ++ ( subs_i [ 0 ] ) ) { mwIndex j ; for ( j = 0 ; j < 3 ; ++ j ) { mxArray * data_ij ; subs_i [ 1 ] = j ; offset_i = mxCalcSingleSubscript ( data_i , 2 , subs_i ) ; data_ij = mxGetCell ( data_i , offset_i ) ; data_ij = mxDuplicateArray ( data_ij ) ; subs [ 1 ] = j ; offset = mxCalcSingleSubscript ( data , 2 , subs ) ; mxSetCell ( data , offset , data_ij ) ; } ++ ( subs [ 0 ] ) ; } mxDestroyArray ( data_i ) ; } } } } return data ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
