#ifndef LocalTerrain_h_
#define LocalTerrain_h_
#ifndef LocalTerrain_COMMON_INCLUDES_
#define LocalTerrain_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "LocalTerrain_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
struct lm33fj4cvi { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 2 ] ; int_T systemTid [ 2 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ;
typedef struct { ahmwprd4ow rtm ; } a3tgdgrc1z2 ; extern real_T
rtP_terrainHeightNED_m ; extern void etquuaa4x0 ( SimStruct * _mdlRefSfcnS ,
int_T mdlref_TID0 , int_T mdlref_TID1 , ahmwprd4ow * const f0adfneflq , void
* sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI ,
const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ;
extern void mr_LocalTerrain_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T
* modelName , int_T * retVal ) ; extern mxArray * mr_LocalTerrain_GetDWork ( const a3tgdgrc1z2 * mdlrefDW ) ; extern void mr_LocalTerrain_SetDWork ( a3tgdgrc1z2 * mdlrefDW , const mxArray * ssDW ) ; extern void mr_LocalTerrain_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_LocalTerrain_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * LocalTerrain_GetCAPIStaticMap ( void ) ; extern void kcrhqktkmm ( real_T * oeqxxvbgut ) ; extern void n3llx4wcyn ( real_T * oeqxxvbgut ) ; extern void LocalTerrainTID1 ( real_T * oeqxxvbgut ) ; extern void nktmhlhgc5 ( ahmwprd4ow * const f0adfneflq ) ;
#endif
