#ifndef cgMassInertia_h_
#define cgMassInertia_h_
#ifndef cgMassInertia_COMMON_INCLUDES_
#define cgMassInertia_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "cgMassInertia_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
typedef struct { const real_T mn02u44wzq ; } btac0vxe4a ; struct cci5zeak1tu_
{ real_T P_0 ; real_T P_1 [ 3 ] ; real_T P_2 [ 9 ] ; } ; struct fr0f4ak1sz {
struct SimStruct_tag * _mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { lh5ctkt1vy rtm ; }
jdfkzpcqy5j ; extern void hpxy3sxbqz ( SimStruct * _mdlRefSfcnS , int_T
mdlref_TID0 , int_T mdlref_TID1 , lh5ctkt1vy * const jp2nr14ddq , void *
sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI ,
const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ;
extern void mr_cgMassInertia_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T
* modelName , int_T * retVal ) ; extern mxArray * mr_cgMassInertia_GetDWork ( const jdfkzpcqy5j * mdlrefDW ) ; extern void mr_cgMassInertia_SetDWork ( jdfkzpcqy5j * mdlrefDW , const mxArray * ssDW ) ; extern void mr_cgMassInertia_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_cgMassInertia_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * cgMassInertia_GetCAPIStaticMap ( void ) ; extern void hldptrix4x ( real_T * kve0m1jdyq , real_T mytftut522 [ 3 ] , real_T ndlmpi30oe [ 9 ] ) ; extern void jjipi5h0uc ( real_T * kve0m1jdyq , real_T mytftut522 [ 3 ] , real_T ndlmpi30oe [ 9 ] ) ; extern void cgMassInertiaTID1 ( real_T * kve0m1jdyq , real_T mytftut522 [ 3 ] , real_T ndlmpi30oe [ 9 ] ) ; extern void pkpeacqtcd ( lh5ctkt1vy * const jp2nr14ddq ) ;
#endif
