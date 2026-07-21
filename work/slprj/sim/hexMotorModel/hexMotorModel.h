#ifndef hexMotorModel_h_
#define hexMotorModel_h_
#ifndef hexMotorModel_COMMON_INCLUDES_
#define hexMotorModel_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "hexMotorModel_types.h"
#include <stddef.h>
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { real_T efpirfwwsu ; real_T dgfcm4215r ; real_T k5wcsrnqyl ;
real_T pop0yha42x ; } lfqv4zwlnp ; typedef struct { struct { void * AQHandles
; } h3elteyby5 ; struct { void * AQHandles ; } o5qmacthzp ; } gthktmeton ;
typedef struct { real_T k1t51gdo2b [ 2 ] ; } kdyp32nh4i ; typedef struct {
real_T k1t51gdo2b [ 2 ] ; } hm0e3y4pg0 ; typedef struct { boolean_T
k1t51gdo2b [ 2 ] ; } d0undy0vqf ; typedef struct { real_T imjlp5ycgo [ 2 ] ;
real_T dam4snlvvq ; real_T kcwalh4nks ; real_T nkhie4gmyq [ 2 ] ; lfqv4zwlnp
lwwh0gpqcj [ 2 ] ; } mtoapzzkht ; typedef struct { real_T ogntsftgwx [ 4097 ]
; void * fcp15ch4rw [ 4 ] ; struct { void * AQHandles ; } c52smhxwrv ; struct
{ void * AQHandles ; } kgep1cebnu ; struct { void * AQHandles ; } d3g5iduzuj
; struct { void * AQHandles ; } clao04jqx3 ; struct { void * AQHandles ; }
fq4ccyruvj ; struct { void * AQHandles ; } plxevs24bu ; struct { void *
AQHandles ; } ifgve5svwz ; struct { void * AQHandles ; } m3bzxaplmx ; struct
{ void * AQHandles ; } ga3qipuru2 ; struct { void * AQHandles ; } i40nxkdvdr
; struct { void * AQHandles ; } kejorhhtiz ; struct { void * AQHandles ; }
ec4qkerm3d ; struct { void * AQHandles ; } iqjbhqqlkd ; struct { void *
AQHandles ; } cmecc3qdyx ; struct { void * AQHandles ; } mo5afeqlxl ; struct
{ void * AQHandles ; } byduglk5lc ; int_T k1qj0jvudi [ 8 ] ; gthktmeton
lwwh0gpqcj [ 2 ] ; } m2w44t0qes ; typedef struct { kdyp32nh4i lwwh0gpqcj [ 2
] ; } ddcad1gntt ; typedef struct { hm0e3y4pg0 lwwh0gpqcj [ 2 ] ; }
njf3kis3qn ; typedef struct { d0undy0vqf lwwh0gpqcj [ 2 ] ; } kd0hyhvl4l ;
struct o4h5qult3v_ { real_T P_0 ; real_T P_1 ; real_T P_2 [ 2 ] ; real_T P_3
[ 2 ] ; real_T P_4 [ 2 ] ; real_T P_5 [ 2 ] ; real_T P_6 ; real_T P_7 ;
real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T
P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; } ; struct
mkus21ebquw_ { real_T P_3 ; real_T P_4 ; real_T P_5 [ 6 ] ; real_T P_6 [ 2 ]
; real_T P_7 [ 2 ] ; o4h5qult3v lwwh0gpqcj ; } ; struct fm4tule1iw { struct
SimStruct_tag * _mdlRefSfcnS ; const rtTimingBridge * timingBridge ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 4 ] ; int_T systemTid [ 4 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 3 ] ; time_T tStart ; }
Timing ; } ; typedef struct { mtoapzzkht rtb ; m2w44t0qes rtdw ; ncjrvbve1g
rtm ; } cw0pxo3kgck ; extern real_T rtP_actuatorDelay_s ; extern real_T
rtP_airDensityAtStd_kgpm3 ; extern real_T rtP_rpm2radps ; extern void
euoytwxg03 ( SimStruct * _mdlRefSfcnS , ssNonContDerivSigFeedingOutports * *
mr_nonContOutputArray , int_T mdlref_TID0 , int_T mdlref_TID1 , int_T
mdlref_TID2 , ncjrvbve1g * const drxq1j1q32 , mtoapzzkht * localB ,
m2w44t0qes * localDW , ddcad1gntt * localX , void * sysRanPtr , int_T
contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T *
rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_hexMotorModel_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName
, int_T * retVal ) ; extern mxArray * mr_hexMotorModel_GetDWork ( const
cw0pxo3kgck * mdlrefDW ) ; extern void mr_hexMotorModel_SetDWork ( cw0pxo3kgck
* mdlrefDW , const mxArray * ssDW ) ; extern void
mr_hexMotorModel_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray
* mr_hexMotorModel_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * hexMotorModel_GetCAPIStaticMap ( void ) ;
extern void euejti40ny ( ncjrvbve1g * const drxq1j1q32 , m2w44t0qes * localDW
) ; extern void mtfp45ysnb ( ncjrvbve1g * const drxq1j1q32 , m2w44t0qes *
localDW , ddcad1gntt * localX ) ; extern void kojsg3tyy0 ( ncjrvbve1g * const
drxq1j1q32 , m2w44t0qes * localDW , ddcad1gntt * localX ) ; extern void
bxvkxfooky ( ncjrvbve1g * const drxq1j1q32 , m2w44t0qes * localDW ) ; extern
void jroluecfsa ( ncjrvbve1g * const drxq1j1q32 , mtoapzzkht * localB ,
m2w44t0qes * localDW ) ; extern void k5o4oudfcj ( mtoapzzkht * localB ,
ddcad1gntt * localX , njf3kis3qn * localXdot ) ; extern void cgknnxzlts ( ncjrvbve1g * const drxq1j1q32 , mtoapzzkht * localB , m2w44t0qes * localDW ) ; extern void hexMotorModel ( ncjrvbve1g * const drxq1j1q32 , const real_T * bpqlkwgdlt , const real_T * ldd3c25f1m , const real_T * fhftzjj0vb , const real_T * infpcmy0ca , const boolean_T * g2lwwszvw5 , const boolean_T * iij1rdbtkz , PropulsionBus * jevomsfltp , mtoapzzkht * localB , m2w44t0qes * localDW , ddcad1gntt * localX ) ; extern void hexMotorModelTID2 ( mtoapzzkht * localB ) ; extern void knzha50npu ( ncjrvbve1g * const drxq1j1q32 ) ;
#endif
