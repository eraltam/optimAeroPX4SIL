#ifndef hexGroundContact_h_
#define hexGroundContact_h_
#ifndef hexGroundContact_COMMON_INCLUDES_
#define hexGroundContact_COMMON_INCLUDES_
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
#include "hexGroundContact_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include <string.h>
typedef struct { struct { void * AQHandles ; } bakhvkv2y2 ; struct { void *
AQHandles ; } ooyhprs0gz ; struct { void * AQHandles ; } biqb5ohiss ; struct
{ void * AQHandles ; } gi32eqnx5k ; struct { void * AQHandles ; } osdqtpqgkt
; struct { void * AQHandles ; } dunxtzt4an ; struct { void * AQHandles ; }
neyuogd2ny ; struct { void * AQHandles ; } jebrb3uv5h ; struct { void *
AQHandles ; } emxha1omgj ; struct { void * AQHandles ; } glo0ebxpmg ; struct
{ void * AQHandles ; } neyuogd2nyb ; struct { void * AQHandles ; } e24qws2gfa
; struct { void * AQHandles ; } blxelmouu4 ; struct { void * AQHandles ; }
lyosawgd1y ; struct { void * AQHandles ; } hb44rtvflo ; struct { void *
AQHandles ; } c3cyjvdpwv ; struct { void * AQHandles ; } afevsv2cy4 ; struct
{ void * AQHandles ; } lgayewcbgz ; struct { void * AQHandles ; } ksqeyaocgw
; struct { void * AQHandles ; } nc5i33mske ; struct { void * AQHandles ; }
gi32eqnx5k5 ; struct { void * AQHandles ; } hhsksnzc2b ; struct { void *
AQHandles ; } ex5ee5qh3w ; } eau03cdx20 ; struct o55aay0h0fy_ { real_T P_0 ;
real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T
P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 [ 3 ] ; real_T P_16 [ 3 ] ; }
; struct hpzazzelz2 { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 4 ] ; int_T systemTid [ 4 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ;
typedef struct { eau03cdx20 rtdw ; lovmqf3ufw rtm ; } dz2poepecm5 ; extern
void bjz0njqoy1 ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T
mdlref_TID1 , lovmqf3ufw * const ji5fp3oui2 , eau03cdx20 * localDW , void *
sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI ,
const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ;
extern void mr_hexGroundContact_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS ,
char_T * modelName , int_T * retVal ) ; extern mxArray *
mr_hexGroundContact_GetDWork ( const dz2poepecm5 * mdlrefDW ) ; extern void
mr_hexGroundContact_SetDWork ( dz2poepecm5 * mdlrefDW , const mxArray * ssDW
) ; extern void mr_hexGroundContact_RegisterSimStateChecksum ( SimStruct * S
) ; extern mxArray * mr_hexGroundContact_GetSimStateDisallowedBlocks ( ) ;
extern const rtwCAPI_ModelMappingStaticInfo *
hexGroundContact_GetCAPIStaticMap ( void ) ; extern void oejdm1huzg ( lovmqf3ufw
* const ji5fp3oui2 , eau03cdx20 * localDW ) ; extern void mpzatt4icf ( lovmqf3ufw * const ji5fp3oui2 , eau03cdx20 * localDW ) ; extern void hexGroundContact ( lovmqf3ufw * const ji5fp3oui2 , const real_T k4ofg520oi [ 3 ] , const real_T c0bg2dhgm4 [ 3 ] , const real_T kbsnye2bao [ 3 ] , const real_T bphcvdbsyr [ 3 ] , const real_T bsvhovlzcv [ 3 ] , const real_T alwlstoyvm [ 3 ] , const real_T mxbuvuan2m [ 3 ] , const real_T mdr551pbek [ 3 ] , const real_T clqqlly0sv [ 3 ] , const real_T dad2g1d12f [ 9 ] , const real_T * kowmvbtd4c , boolean_T * jsg2wsuk5t , real_T pwnxq1b3ny [ 3 ] , real_T grio4lydc0 [ 3 ] , eau03cdx20 * localDW ) ; extern void irs14nz5it ( lovmqf3ufw * const ji5fp3oui2 ) ;
#endif
