#ifndef PX4OutputMappingHex_h_
#define PX4OutputMappingHex_h_
#ifndef PX4OutputMappingHex_COMMON_INCLUDES_
#define PX4OutputMappingHex_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "PX4OutputMappingHex_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { struct { void * AQHandles ; } m1kz0ehbyk ; struct { void *
AQHandles ; } fnik3ibdag ; struct { void * AQHandles ; } lrpu5wkzal ; struct
{ void * AQHandles ; } elzjhtbva1 ; struct { void * AQHandles ; } bvsdr02153
; struct { void * AQHandles ; } mktlndk3dy ; struct { void * AQHandles ; }
mxh3c15nbh ; } cr3hfcxqli ; struct f0oupnkqiwv_ { ServosCommandBus P_0 ; } ;
struct bhapmjnfo5 { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 2 ] ; int_T systemTid [ 2 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ;
typedef struct { cr3hfcxqli rtdw ; jrvsdctowq rtm ; } ol2n430xj13 ; extern
void n5afj0rav5 ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T
mdlref_TID1 , jrvsdctowq * const b0v1pfzjjp , cr3hfcxqli * localDW , void *
sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI ,
const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ;
extern void mr_PX4OutputMappingHex_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS ,
char_T * modelName , int_T * retVal ) ; extern mxArray *
mr_PX4OutputMappingHex_GetDWork ( const ol2n430xj13 * mdlrefDW ) ; extern
void mr_PX4OutputMappingHex_SetDWork ( ol2n430xj13 * mdlrefDW , const mxArray
* ssDW ) ; extern void mr_PX4OutputMappingHex_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_PX4OutputMappingHex_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * PX4OutputMappingHex_GetCAPIStaticMap ( void ) ; extern void pvhk3hhyhy ( jrvsdctowq * const b0v1pfzjjp , cr3hfcxqli * localDW ) ; extern void paly35jijx ( jrvsdctowq * const b0v1pfzjjp , cr3hfcxqli * localDW ) ; extern void PX4OutputMappingHex ( jrvsdctowq * const b0v1pfzjjp , const real_T kgjiws31eu [ 16 ] , real_T * jzi34nl5po , real_T * oao3xkkhlx , real_T * neiwzxdq0y , real_T * lrxfv2uoup , real_T * oeg0n3ffsi , real_T * ncp4ep05m3 , cr3hfcxqli * localDW ) ; extern void kyuedg42rr ( jrvsdctowq * const b0v1pfzjjp ) ;
#endif
