#ifndef hexAddFm_h_
#define hexAddFm_h_
#ifndef hexAddFm_COMMON_INCLUDES_
#define hexAddFm_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "hexAddFm_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { struct { void * AQHandles ; } cl1v5qij1n ; struct { void *
AQHandles ; } pmx2sjqkq0 ; struct { void * AQHandles ; } hetr0fccqb ; struct
{ void * AQHandles ; } fi4c4zb2ai ; struct { void * AQHandles ; } eitjyrsodp
; struct { void * AQHandles ; } ekx43amre3 ; struct { void * AQHandles ; }
hbquyemcom ; struct { void * AQHandles ; } isr4cunwib ; struct { void *
AQHandles ; } h02ochxt05 ; struct { void * AQHandles ; } j4rhvsijn5 ; }
bghop2orkf ; struct j40r5s3ohr { struct SimStruct_tag * _mdlRefSfcnS ; struct
{ rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 2 ] ; int_T systemTid [ 2 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 1 ] ; } Timing ; } ;
typedef struct { bghop2orkf rtdw ; c3qfekj1m4 rtm ; } eckipmdqplk ; extern
void gqyvwzowyf ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , c3qfekj1m4 *
const enpvgx42so , bghop2orkf * localDW , void * sysRanPtr , int_T contextTid
, rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath ,
int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_hexAddFm_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName ,
int_T * retVal ) ; extern mxArray * mr_hexAddFm_GetDWork ( const eckipmdqplk
* mdlrefDW ) ; extern void mr_hexAddFm_SetDWork ( eckipmdqplk * mdlrefDW ,
const mxArray * ssDW ) ; extern void mr_hexAddFm_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_hexAddFm_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * hexAddFm_GetCAPIStaticMap ( void ) ; extern void e5pwjmiupg ( c3qfekj1m4 * const enpvgx42so , bghop2orkf * localDW ) ; extern void km11d4onwl ( c3qfekj1m4 * const enpvgx42so , bghop2orkf * localDW ) ; extern void hexAddFm ( c3qfekj1m4 * const enpvgx42so , const real_T gghd1t3hnm [ 3 ] , const real_T ijpl5pacvu [ 3 ] , const real_T iutbkfvq5s [ 3 ] , const real_T kuqmv3cmp2 [ 3 ] , const real_T oql3ufbind [ 3 ] , const real_T dtxamvvqbu [ 3 ] , const real_T bxqthfdbwy [ 3 ] , const real_T az1mp0ggdy [ 3 ] , real_T a40fr2glcf [ 3 ] , real_T epsiucxkna [ 3 ] , real_T biwzedke2t [ 3 ] , real_T ihfqpczu42 [ 3 ] , real_T eww4gkcli4 [ 3 ] , real_T oxyczpcwvo [ 3 ] , real_T cccau43zoh [ 3 ] , real_T j1k54pow4e [ 3 ] , real_T ptz32l5blv [ 3 ] , real_T p55aa30x0j [ 3 ] , bghop2orkf * localDW ) ; extern void b2123yef1i ( c3qfekj1m4 * const enpvgx42so ) ;
#endif
