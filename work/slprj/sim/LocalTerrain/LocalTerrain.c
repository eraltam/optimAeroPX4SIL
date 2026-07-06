#include "LocalTerrain.h"
#include "rtwtypes.h"
#include "LocalTerrain_capi.h"
#include "LocalTerrain_private.h"
static RegMdlInfo rtMdlInfo_LocalTerrain [ 41 ] = { { "a3tgdgrc1z2" ,
MDL_INFO_NAME_MDLREF_DWORK , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"dxuzwujpv0" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "ngbhlxtmac" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "hkke2rnl4p" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"i1vlge3mvi" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "lqg5ifjn35" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "cnm5kizihb" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"jboirqvdre" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "pv2g2fycdh" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "pyfylrp02d" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"est5z4gclt" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "jayephs5b1" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "el0agsbyhj" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"am5jkbeyrs" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "ankyjgowdy" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "nktmhlhgc5" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"kcrhqktkmm" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "etquuaa4x0" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "n3llx4wcyn" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"eygje32hfp" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "LocalTerrain" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 ,
0 , ( NULL ) } , { "nzzkihu05a" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1
, ( void * ) "LocalTerrain" } , { "lsrc2rdqylv" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"lm33fj4cvi" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"LocalTerrain" } , { "ahmwprd4ow" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "LocalTerrain" } , { "TerrainEnvironmentBus" ,
MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , { "BodyStateBus" ,
MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"mr_LocalTerrain_GetSimStateDisallowedBlocks" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_restoreDataFromMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_cacheDataToMxArrayWithOffset" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_extractBitFieldFromMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0
, - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_cacheBitFieldToMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , -
1 , ( void * ) "LocalTerrain" } , { "mr_LocalTerrain_restoreDataFromMxArray"
, MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_cacheDataAsMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 ,
( void * ) "LocalTerrain" } , { "mr_LocalTerrain_RegisterSimStateChecksum" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"mr_LocalTerrain_SetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void *
) "LocalTerrain" } , { "mr_LocalTerrain_GetDWork" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "LocalTerrain" } , {
"LocalTerrain.h" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( NULL ) } , {
"LocalTerrain.c" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void * )
"LocalTerrain" } } ; void kcrhqktkmm ( real_T * oeqxxvbgut ) { * oeqxxvbgut =
rtP_terrainHeightNED_m ; } void n3llx4wcyn ( real_T * oeqxxvbgut ) { *
oeqxxvbgut = rtP_terrainHeightNED_m ; } void LocalTerrainTID1 ( real_T *
oeqxxvbgut ) { * oeqxxvbgut = rtP_terrainHeightNED_m ; } void nktmhlhgc5 ( ahmwprd4ow * const f0adfneflq ) { if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( f0adfneflq -> _mdlRefSfcnS , "LocalTerrain" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT" ) ; } } void etquuaa4x0 ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , ahmwprd4ow * const f0adfneflq , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset ( ( void * ) f0adfneflq , 0 , sizeof ( ahmwprd4ow ) ) ; f0adfneflq -> Timing . mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; f0adfneflq -> Timing . mdlref_GlobalTID [ 1 ] = mdlref_TID1 ; f0adfneflq -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( f0adfneflq -> _mdlRefSfcnS , "LocalTerrain" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } LocalTerrain_InitializeDataMapInfo ( f0adfneflq , sysRanPtr , contextTid ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( f0adfneflq -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( f0adfneflq -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( f0adfneflq -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_LocalTerrain_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_LocalTerrain , 41 ) ; * retVal = 1 ; } static void mr_LocalTerrain_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_LocalTerrain_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_LocalTerrain_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_LocalTerrain_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_LocalTerrain_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_LocalTerrain_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_LocalTerrain_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_LocalTerrain_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_LocalTerrain_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_LocalTerrain_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_LocalTerrain_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_LocalTerrain_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_LocalTerrain_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_LocalTerrain_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_LocalTerrain_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_LocalTerrain_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_LocalTerrain_GetDWork ( const a3tgdgrc1z2 * mdlrefDW ) { ( void ) mdlrefDW ; return ( NULL ) ; } void mr_LocalTerrain_SetDWork ( a3tgdgrc1z2 * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; } void mr_LocalTerrain_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 462519570U , 3375600534U , 3051300458U , 1464177414U , } ; slmrModelRefRegisterSimStateChecksum ( S , "LocalTerrain" , & chksum [ 0 ] ) ; } mxArray * mr_LocalTerrain_GetSimStateDisallowedBlocks ( ) { return ( NULL ) ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
