#ifndef failureInputReadHex_h_
#define failureInputReadHex_h_
#ifndef failureInputReadHex_COMMON_INCLUDES_
#define failureInputReadHex_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif
#include "failureInputReadHex_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include <string.h>
typedef struct { boolean_T fzgr5j1xnk ; } ftqj4vmsbp ; typedef struct {
struct { void * AQHandles ; } auk5los52d ; struct { void * AQHandles ; }
bnvgmuuqui ; struct { void * AQHandles ; } iruazx2lhk ; struct { void *
AQHandles ; } gdf0ieewnk ; struct { void * AQHandles ; } amlvfhkhdq ; struct
{ void * AQHandles ; } bwnr333m03 ; struct { void * AQHandles ; } iqrxr0qhje
; struct { void * AQHandles ; } dokwso4aqy ; struct { void * AQHandles ; }
nnb1cvvjba ; boolean_T bivpoza4we ; } afflsib4oz ; struct avnv3tkhy1s_ {
EnumHexFailureType P_0 ; EnumHexFailureType P_1 ; EnumHexFailureType P_2 ;
EnumHexFailureType P_3 ; EnumHexFailureType P_4 ; EnumHexFailureType P_5 ;
uint32_T P_6 ; boolean_T P_7 ; } ; struct ap1crwbd30 { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { ftqj4vmsbp rtb ;
afflsib4oz rtdw ; ol3lbvizit rtm ; } buctytp4xb2 ; extern void ojggctlbvm ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , ol3lbvizit * const hndhqygpjk , ftqj4vmsbp * localB , afflsib4oz * localDW , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_failureInputReadHex_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_failureInputReadHex_GetDWork ( const buctytp4xb2 * mdlrefDW ) ; extern void mr_failureInputReadHex_SetDWork ( buctytp4xb2 * mdlrefDW , const mxArray * ssDW ) ; extern void mr_failureInputReadHex_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_failureInputReadHex_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * failureInputReadHex_GetCAPIStaticMap ( void ) ; extern void kgsh1oahvc ( ol3lbvizit * const hndhqygpjk , afflsib4oz * localDW ) ; extern void fpj1bzgggi ( afflsib4oz * localDW ) ; extern void ppjcyknosv ( afflsib4oz * localDW ) ; extern void covkarfzmw ( ol3lbvizit * const hndhqygpjk , afflsib4oz * localDW ) ; extern void h5iepmx1iq ( ftqj4vmsbp * localB , afflsib4oz * localDW ) ; extern void failureInputReadHex ( ol3lbvizit * const hndhqygpjk , const uint32_T * gucy4qdtoy , const EnumHexFailureType * hiupzlrirt , boolean_T * bwkcciorrx , boolean_T * owjd5s4w3u , boolean_T * ehlbieiqe0 , boolean_T * ectnjy2p13 , boolean_T * b0ps1v5lh4 , boolean_T * geed5jtrh0 , ftqj4vmsbp * localB , afflsib4oz * localDW ) ; extern void ineffhb1ao ( ol3lbvizit * const hndhqygpjk ) ;
#endif
