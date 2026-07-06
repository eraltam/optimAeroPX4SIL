#include "cgMassInertia.h"
#include "rtwtypes.h"
#include "cgMassInertia_private.h"
#include <string.h>
#include "cgMassInertia_capi.h"
static RegMdlInfo rtMdlInfo_cgMassInertia [ 41 ] = { { "jdfkzpcqy5j" ,
MDL_INFO_NAME_MDLREF_DWORK , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"jpp02lcdmy" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "apzbyof0t2" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "hton2voage" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"f512ygfiiq" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "nnfat1ie50" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "nxghrxms3l" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"cyeuk2as1i" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "hi3jboxi3a" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "adhjepae4y" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"btac0vxe4a" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "m1uiucu3uq" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "bhnotg0ptt" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"gmlvbb32fa" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "i0rk4aztuz" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "pkpeacqtcd" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"hldptrix4x" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "hpxy3sxbqz" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "jjipi5h0uc" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mwy1oi4fia" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "cgMassInertia" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0
, 0 , ( NULL ) } , { "o3whg3gfbu" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "cci5zeak1tu" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"fr0f4ak1sz" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"cgMassInertia" } , { "lh5ctkt1vy" , MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , -
1 , ( void * ) "cgMassInertia" } , { "k5crodhdd0" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"MassPropertiesBus" , MDL_INFO_ID_DATA_TYPE , 0 , - 1 , ( NULL ) } , {
"mr_cgMassInertia_GetSimStateDisallowedBlocks" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_restoreDataFromMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_cacheDataToMxArrayWithOffset" , MDL_INFO_ID_MODEL_FCN_NAME
, 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_extractBitFieldFromMxArray" , MDL_INFO_ID_MODEL_FCN_NAME ,
0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_cacheBitFieldToMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 ,
- 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_restoreDataFromMxArray" , MDL_INFO_ID_MODEL_FCN_NAME , 0 ,
- 1 , ( void * ) "cgMassInertia" } , { "mr_cgMassInertia_cacheDataAsMxArray"
, MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_RegisterSimStateChecksum" , MDL_INFO_ID_MODEL_FCN_NAME , 0
, - 1 , ( void * ) "cgMassInertia" } , { "mr_cgMassInertia_SetDWork" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "cgMassInertia" } , {
"mr_cgMassInertia_GetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void *
) "cgMassInertia" } , { "cgMassInertia.h" , MDL_INFO_MODEL_FILENAME , 0 , - 1
, ( NULL ) } , { "cgMassInertia.c" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void
* ) "cgMassInertia" } } ; cci5zeak1tu cci5zeak1t = { 81.64656 , { 0.0 , 0.0 ,
0.0 } , { 11.8 , 0.0 , 0.0 , 0.0 , 11.8 , 0.0 , 0.0 , 0.0 , 23.5 } } ; void
hldptrix4x ( real_T * kve0m1jdyq , real_T mytftut522 [ 3 ] , real_T
ndlmpi30oe [ 9 ] ) { * kve0m1jdyq = cci5zeak1t . P_0 ; mytftut522 [ 0 ] =
cci5zeak1t . P_1 [ 0 ] ; mytftut522 [ 1 ] = cci5zeak1t . P_1 [ 1 ] ;
mytftut522 [ 2 ] = cci5zeak1t . P_1 [ 2 ] ; memcpy ( & ndlmpi30oe [ 0 ] , &
cci5zeak1t . P_2 [ 0 ] , 9U * sizeof ( real_T ) ) ; } void jjipi5h0uc ( real_T
* kve0m1jdyq , real_T mytftut522 [ 3 ] , real_T ndlmpi30oe [ 9 ] ) { *
kve0m1jdyq = cci5zeak1t . P_0 ; mytftut522 [ 0 ] = cci5zeak1t . P_1 [ 0 ] ;
mytftut522 [ 1 ] = cci5zeak1t . P_1 [ 1 ] ; mytftut522 [ 2 ] = cci5zeak1t .
P_1 [ 2 ] ; memcpy ( & ndlmpi30oe [ 0 ] , & cci5zeak1t . P_2 [ 0 ] , 9U *
sizeof ( real_T ) ) ; } void cgMassInertiaTID1 ( real_T * kve0m1jdyq , real_T
mytftut522 [ 3 ] , real_T ndlmpi30oe [ 9 ] ) { * kve0m1jdyq = cci5zeak1t .
P_0 ; mytftut522 [ 0 ] = cci5zeak1t . P_1 [ 0 ] ; mytftut522 [ 1 ] =
cci5zeak1t . P_1 [ 1 ] ; mytftut522 [ 2 ] = cci5zeak1t . P_1 [ 2 ] ; memcpy ( &
ndlmpi30oe [ 0 ] , & cci5zeak1t . P_2 [ 0 ] , 9U * sizeof ( real_T ) ) ; }
void pkpeacqtcd ( lh5ctkt1vy * const jp2nr14ddq ) { if ( !
slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( jp2nr14ddq ->
_mdlRefSfcnS , "cgMassInertia" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT"
) ; } } void hpxy3sxbqz ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 ,
int_T mdlref_TID1 , lh5ctkt1vy * const jp2nr14ddq , void * sysRanPtr , int_T
contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T *
rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset
( ( void * ) jp2nr14ddq , 0 , sizeof ( lh5ctkt1vy ) ) ; jp2nr14ddq -> Timing
. mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; jp2nr14ddq -> Timing .
mdlref_GlobalTID [ 1 ] = mdlref_TID1 ; jp2nr14ddq -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( jp2nr14ddq -> _mdlRefSfcnS , "cgMassInertia" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } cgMassInertia_InitializeDataMapInfo ( jp2nr14ddq , sysRanPtr , contextTid ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( jp2nr14ddq -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( jp2nr14ddq -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( jp2nr14ddq -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_cgMassInertia_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_cgMassInertia , 41 ) ; * retVal = 1 ; } static void mr_cgMassInertia_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_cgMassInertia_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_cgMassInertia_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_cgMassInertia_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_cgMassInertia_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_cgMassInertia_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_cgMassInertia_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_cgMassInertia_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_cgMassInertia_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_cgMassInertia_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_cgMassInertia_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_cgMassInertia_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_cgMassInertia_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_cgMassInertia_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_cgMassInertia_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_cgMassInertia_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_cgMassInertia_GetDWork ( const jdfkzpcqy5j * mdlrefDW ) { ( void ) mdlrefDW ; return ( NULL ) ; } void mr_cgMassInertia_SetDWork ( jdfkzpcqy5j * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; } void mr_cgMassInertia_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 677062624U , 1833198686U , 2488929927U , 3547987112U , } ; slmrModelRefRegisterSimStateChecksum ( S , "cgMassInertia" , & chksum [ 0 ] ) ; } mxArray * mr_cgMassInertia_GetSimStateDisallowedBlocks ( ) { return ( NULL ) ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
