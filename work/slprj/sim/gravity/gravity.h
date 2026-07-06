#ifndef gravity_h_
#define gravity_h_
#ifndef gravity_COMMON_INCLUDES_
#define gravity_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "gravity_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { struct { void * AQHandles ; } iol413cztc ; struct { void *
AQHandles ; } knapd2jbbq ; struct { void * AQHandles ; } it2n1dtgl4 ; }
jnf12obkru ; struct p4bvg1ccpmt_ { real_T P_0 ; real_T P_1 ; real_T P_2 [ 3 ]
; } ; struct ayhjq3cbdg { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 2 ] ; int_T systemTid [ 2 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ;
typedef struct { jnf12obkru rtdw ; pbuflhclcw rtm ; } e53sj53lngm ; extern
void hjpfxdeo4m ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T
mdlref_TID1 , pbuflhclcw * const iqfommknk5 , jnf12obkru * localDW , void *
sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI ,
const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ;
extern void mr_gravity_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T *
modelName , int_T * retVal ) ; extern mxArray * mr_gravity_GetDWork ( const
e53sj53lngm * mdlrefDW ) ; extern void mr_gravity_SetDWork ( e53sj53lngm *
mdlrefDW , const mxArray * ssDW ) ; extern void
mr_gravity_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray *
mr_gravity_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * gravity_GetCAPIStaticMap ( void ) ; extern
void b4jvsait25 ( pbuflhclcw * const iqfommknk5 , jnf12obkru * localDW ) ;
extern void ei5jam3ejs ( pbuflhclcw * const iqfommknk5 , jnf12obkru * localDW
) ; extern void gravity ( pbuflhclcw * const iqfommknk5 , const real_T *
nfdazcmfoj , const real_T orex3mnac0 [ 9 ] , const real_T * mrckfrznvq ,
real_T h53b50pagx [ 3 ] , jnf12obkru * localDW ) ; extern void gravityTID1 ( real_T l0jne2gj2e [ 3 ] ) ; extern void ebphgotimp ( pbuflhclcw * const iqfommknk5 ) ;
#endif
