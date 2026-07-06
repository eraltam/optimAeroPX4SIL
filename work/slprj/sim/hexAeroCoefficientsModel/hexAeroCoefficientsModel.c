#include "hexAeroCoefficientsModel.h"
#include "rtwtypes.h"
#include "hexAeroCoefficientsModel_private.h"
#include "mwmathutil.h"
#include "hexAeroCoefficientsModel_capi.h"
#include "look1_binlxpw.h"
static RegMdlInfo rtMdlInfo_hexAeroCoefficientsModel [ 40 ] = { {
"a442fa03xsq" , MDL_INFO_NAME_MDLREF_DWORK , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "nilscr45uh" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "fgwqzhh500" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "awgbv4kvdx" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "jhkou1tsin" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "jgwh0tbayr" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "da35fyne34" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "dofz55vll0" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "e1cgdyvybx" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "ikn2zwhwjq" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "n3zdnsll1r" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "liwoz2zuxx" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "ivq4dpbyem" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "peoo5t3mpu" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "jvcmrdjray" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "ifc1ugjrlf" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "mnsebcjqb5" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "l0mrvwrozf" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "jvaz4icrry" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "ixegvndvdo" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "cxqcbkve3m" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "hexAeroCoefficientsModel" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , 0 , ( NULL ) } , { "efztzh4vg1" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "omjaewjshtj" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "nsjwh01qyt" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , { "mrrbvppoau" ,
MDL_INFO_ID_GLOBAL_RTW_CONSTRUCT , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } , {
"mr_hexAeroCoefficientsModel_GetSimStateDisallowedBlocks" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_extractBitFieldFromCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_cacheBitFieldToCellArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_restoreDataFromMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_cacheDataToMxArrayWithOffset" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_extractBitFieldFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_cacheBitFieldToMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_restoreDataFromMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_cacheDataAsMxArray" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_RegisterSimStateChecksum" ,
MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1 , ( void * ) "hexAeroCoefficientsModel"
} , { "mr_hexAeroCoefficientsModel_SetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0
, - 1 , ( void * ) "hexAeroCoefficientsModel" } , {
"mr_hexAeroCoefficientsModel_GetDWork" , MDL_INFO_ID_MODEL_FCN_NAME , 0 , - 1
, ( void * ) "hexAeroCoefficientsModel" } , { "hexAeroCoefficientsModel.h" ,
MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( NULL ) } , {
"hexAeroCoefficientsModel.c" , MDL_INFO_MODEL_FILENAME , 0 , - 1 , ( void * )
"hexAeroCoefficientsModel" } } ; omjaewjshtj omjaewjsht = { - 1.0 , - 1.0 ,
0.0 , 0.0 , { 0.0 , 0.0 , 0.0 } } ; void l0mrvwrozf ( real_T ei0caupkdq [ 3 ]
) { ei0caupkdq [ 0 ] = omjaewjsht . P_7 [ 0 ] ; ei0caupkdq [ 1 ] = omjaewjsht
. P_7 [ 1 ] ; ei0caupkdq [ 2 ] = omjaewjsht . P_7 [ 2 ] ; } void ixegvndvdo ( real_T ei0caupkdq [ 3 ] ) { ei0caupkdq [ 0 ] = omjaewjsht . P_7 [ 0 ] ; ei0caupkdq [ 1 ] = omjaewjsht . P_7 [ 1 ] ; ei0caupkdq [ 2 ] = omjaewjsht . P_7 [ 2 ] ; } void hexAeroCoefficientsModel ( const real_T * ohothlrcxr , const real_T * csnsczifqr , real_T n4gptmxx0q [ 3 ] ) { real_T i1ohvs43tv [ 9 ] ; real_T jdf14jkkpr_idx_0 ; real_T jdf14jkkpr_idx_1 ; real_T ozj2cdyhxw_idx_0 ; real_T ozj2cdyhxw_idx_1 ; int32_T i ; muDoubleScalarSinCos ( * ohothlrcxr , & ozj2cdyhxw_idx_0 , & jdf14jkkpr_idx_0 ) ; muDoubleScalarSinCos ( * csnsczifqr , & ozj2cdyhxw_idx_1 , & jdf14jkkpr_idx_1 ) ; i1ohvs43tv [ 0 ] = jdf14jkkpr_idx_0 * jdf14jkkpr_idx_1 ; i1ohvs43tv [ 6 ] = ozj2cdyhxw_idx_0 * jdf14jkkpr_idx_1 ; i1ohvs43tv [ 1 ] = - ( jdf14jkkpr_idx_0 * ozj2cdyhxw_idx_1 ) ; i1ohvs43tv [ 7 ] = - ( ozj2cdyhxw_idx_0 * ozj2cdyhxw_idx_1 ) ; i1ohvs43tv [ 2 ] = - ozj2cdyhxw_idx_0 ; i1ohvs43tv [ 3 ] = ozj2cdyhxw_idx_1 ; i1ohvs43tv [ 4 ] = jdf14jkkpr_idx_1 ; i1ohvs43tv [ 5 ] = omjaewjsht . P_6 ; i1ohvs43tv [ 8 ] = jdf14jkkpr_idx_0 ; ozj2cdyhxw_idx_0 = ( look1_binlxpw ( * ohothlrcxr , rtP_alphaBkpts_rad , rtP_C_D , 18U ) + look1_binlxpw ( * ohothlrcxr , rtP_alphaBkpts_rad , rtP_C_D , 18U ) ) * omjaewjsht . P_3 ; jdf14jkkpr_idx_0 = omjaewjsht . P_4 * look1_binlxpw ( * ohothlrcxr , rtP_alphaBkpts_rad , rtP_C_L , 18U ) ; for ( i = 0 ; i < 3 ; i ++ ) { n4gptmxx0q [ i ] = ( i1ohvs43tv [ 3 * i + 1 ] * omjaewjsht . P_5 + i1ohvs43tv [ 3 * i ] * ozj2cdyhxw_idx_0 ) + i1ohvs43tv [ 3 * i + 2 ] * jdf14jkkpr_idx_0 ; } } void hexAeroCoefficientsModelTID1 ( real_T ei0caupkdq [ 3 ] ) { ei0caupkdq [ 0 ] = omjaewjsht . P_7 [ 0 ] ; ei0caupkdq [ 1 ] = omjaewjsht . P_7 [ 1 ] ; ei0caupkdq [ 2 ] = omjaewjsht . P_7 [ 2 ] ; } void ifc1ugjrlf ( mrrbvppoau * const imbcyjzm1j ) { if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( imbcyjzm1j -> _mdlRefSfcnS , "hexAeroCoefficientsModel" , "SIMSTATUS_TERMINATING_MODELREF_ACCEL_EVENT" ) ; } } void jvaz4icrry ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , mrrbvppoau * const imbcyjzm1j , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) { ( void ) memset ( ( void * ) imbcyjzm1j , 0 , sizeof ( mrrbvppoau ) ) ; imbcyjzm1j -> Timing . mdlref_GlobalTID [ 0 ] = mdlref_TID0 ; imbcyjzm1j -> Timing . mdlref_GlobalTID [ 1 ] = mdlref_TID1 ; imbcyjzm1j -> _mdlRefSfcnS = ( _mdlRefSfcnS ) ; if ( ! slIsRapidAcceleratorSimulating ( ) ) { slmrRunPluginEvent ( imbcyjzm1j -> _mdlRefSfcnS , "hexAeroCoefficientsModel" , "START_OF_SIM_MODEL_MODELREF_ACCEL_EVENT" ) ; } hexAeroCoefficientsModel_InitializeDataMapInfo ( imbcyjzm1j , sysRanPtr , contextTid ) ; if ( ( rt_ParentMMI != ( NULL ) ) && ( rt_ChildPath != ( NULL ) ) ) { rtwCAPI_SetChildMMI ( * rt_ParentMMI , rt_ChildMMIIdx , & ( imbcyjzm1j -> DataMapInfo . mmi ) ) ; rtwCAPI_SetPath ( imbcyjzm1j -> DataMapInfo . mmi , rt_ChildPath ) ; rtwCAPI_MMISetContStateStartIndex ( imbcyjzm1j -> DataMapInfo . mmi , rt_CSTATEIdx ) ; } } void mr_hexAeroCoefficientsModel_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) { * retVal = 0 ; { boolean_T regSubmodelsMdlinfo = false ; ssGetRegSubmodelsMdlinfo ( mdlRefSfcnS , & regSubmodelsMdlinfo ) ; if ( regSubmodelsMdlinfo ) { } } * retVal = 0 ; ssRegModelRefMdlInfo ( mdlRefSfcnS , modelName , rtMdlInfo_hexAeroCoefficientsModel , 40 ) ; * retVal = 1 ; } static void mr_hexAeroCoefficientsModel_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) ; static void mr_hexAeroCoefficientsModel_cacheDataAsMxArray ( mxArray * destArray , mwIndex i , int j , const void * srcData , size_t numBytes ) { mxArray * newArray = mxCreateUninitNumericMatrix ( ( size_t ) 1 , numBytes , mxUINT8_CLASS , mxREAL ) ; memcpy ( ( uint8_T * ) mxGetData ( newArray ) , ( const uint8_T * ) srcData , numBytes ) ; mxSetFieldByNumber ( destArray , i , j , newArray ) ; } static void mr_hexAeroCoefficientsModel_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) ; static void mr_hexAeroCoefficientsModel_restoreDataFromMxArray ( void * destData , const mxArray * srcArray , mwIndex i , int j , size_t numBytes ) { memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) , numBytes ) ; } static void mr_hexAeroCoefficientsModel_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) ; static void mr_hexAeroCoefficientsModel_cacheBitFieldToMxArray ( mxArray * destArray , mwIndex i , int j , uint_T bitVal ) { mxSetFieldByNumber ( destArray , i , j , mxCreateDoubleScalar ( ( real_T ) bitVal ) ) ; } static uint_T mr_hexAeroCoefficientsModel_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) ; static uint_T mr_hexAeroCoefficientsModel_extractBitFieldFromMxArray ( const mxArray * srcArray , mwIndex i , int j , uint_T numBits ) { const uint_T varVal = ( uint_T ) mxGetScalar ( mxGetFieldByNumber ( srcArray , i , j ) ) ; return varVal & ( ( 1u << numBits ) - 1u ) ; } static void mr_hexAeroCoefficientsModel_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) ; static void mr_hexAeroCoefficientsModel_cacheDataToMxArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , const void * srcData , size_t numBytes ) { uint8_T * varData = ( uint8_T * ) mxGetData ( mxGetFieldByNumber ( destArray , i , j ) ) ; memcpy ( ( uint8_T * ) & varData [ offset * numBytes ] , ( const uint8_T * ) srcData , numBytes ) ; } static void mr_hexAeroCoefficientsModel_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) ; static void mr_hexAeroCoefficientsModel_restoreDataFromMxArrayWithOffset ( void * destData , const mxArray * srcArray , mwIndex i , int j , mwIndex offset , size_t numBytes ) { const uint8_T * varData = ( const uint8_T * ) mxGetData ( mxGetFieldByNumber ( srcArray , i , j ) ) ; memcpy ( ( uint8_T * ) destData , ( const uint8_T * ) & varData [ offset * numBytes ] , numBytes ) ; } static void mr_hexAeroCoefficientsModel_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) ; static void mr_hexAeroCoefficientsModel_cacheBitFieldToCellArrayWithOffset ( mxArray * destArray , mwIndex i , int j , mwIndex offset , uint_T fieldVal ) { mxSetCell ( mxGetFieldByNumber ( destArray , i , j ) , offset , mxCreateDoubleScalar ( ( real_T ) fieldVal ) ) ; } static uint_T mr_hexAeroCoefficientsModel_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) ; static uint_T mr_hexAeroCoefficientsModel_extractBitFieldFromCellArrayWithOffset ( const mxArray * srcArray , mwIndex i , int j , mwIndex offset , uint_T numBits ) { const uint_T fieldVal = ( uint_T ) mxGetScalar ( mxGetCell ( mxGetFieldByNumber ( srcArray , i , j ) , offset ) ) ; return fieldVal & ( ( 1u << numBits ) - 1u ) ; } mxArray * mr_hexAeroCoefficientsModel_GetDWork ( const a442fa03xsq * mdlrefDW ) { ( void ) mdlrefDW ; return ( NULL ) ; } void mr_hexAeroCoefficientsModel_SetDWork ( a442fa03xsq * mdlrefDW , const mxArray * ssDW ) { ( void ) ssDW ; ( void ) mdlrefDW ; } void mr_hexAeroCoefficientsModel_RegisterSimStateChecksum ( SimStruct * S ) { const uint32_T chksum [ 4 ] = { 3513789990U , 563631155U , 3216916158U , 314739533U , } ; slmrModelRefRegisterSimStateChecksum ( S , "hexAeroCoefficientsModel" , & chksum [ 0 ] ) ; } mxArray * mr_hexAeroCoefficientsModel_GetSimStateDisallowedBlocks ( ) { return ( NULL ) ; }
#if defined(_MSC_VER)
#pragma warning(disable: 4505) //unreferenced local function has been removed
#endif
