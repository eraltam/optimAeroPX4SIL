#ifndef hexAero_h_
#define hexAero_h_
#ifndef hexAero_COMMON_INCLUDES_
#define hexAero_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "hexAero_types.h"
#include "hexAeroCoefficientsModel.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { struct { void * AQHandles ; } licougsyou ; struct { void *
AQHandles ; } h00lzgdweh ; struct { void * AQHandles ; } fzoowixqqa ; struct
{ void * AQHandles ; } ew133v45mv ; struct { void * AQHandles ; } j4uthj5xar
; struct { void * AQHandles ; } fgnjpq0epk ; struct { void * AQHandles ; }
m3wqbjrpdq ; struct { void * AQHandles ; } c5vkqcp5km ; struct { void *
AQHandles ; } bqcfr53cbh ; struct { void * AQHandles ; } ecv40dbn4i ; struct
{ void * AQHandles ; } idvglmwd5d ; a442fa03xsq kzkaav1m2c ; } ivogqrdqsb ;
struct hotgs1hkfcv_ { real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 ; real_T P_8 ; real_T P_9 ; } ; struct asnytcii5m { struct
SimStruct_tag * _mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ;
rtwCAPI_ModelMappingInfo * childMMI [ 1 ] ; sysRanDType * systemRan [ 2 ] ;
int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ]
; } Timing ; } ; typedef struct { ivogqrdqsb rtdw ; drsysgzwpn rtm ; }
f0datyenzg5 ; extern real_T rtP_C_D [ 19 ] ; extern real_T rtP_C_L [ 19 ] ;
extern real_T rtP_alphaBkpts_rad [ 19 ] ; extern void kygq1knqq2 ( SimStruct
* _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , drsysgzwpn * const
dlisy2ivad , ivogqrdqsb * localDW , void * sysRanPtr , int_T contextTid ,
rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T
rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_hexAero_MdlInfoRegFcn
( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern
mxArray * mr_hexAero_GetDWork ( const f0datyenzg5 * mdlrefDW ) ; extern void
mr_hexAero_SetDWork ( f0datyenzg5 * mdlrefDW , const mxArray * ssDW ) ;
extern void mr_hexAero_RegisterSimStateChecksum ( SimStruct * S ) ; extern
mxArray * mr_hexAero_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * hexAero_GetCAPIStaticMap ( void ) ; extern
void bguce0ehaz ( drsysgzwpn * const dlisy2ivad , ivogqrdqsb * localDW ) ;
extern void m0053vu3a1 ( real_T hzdv0vh10t [ 3 ] ) ; extern void mjjf1xe5sx ( drsysgzwpn * const dlisy2ivad , ivogqrdqsb * localDW ) ; extern void nvvgf0iwg4 ( real_T hzdv0vh10t [ 3 ] ) ; extern void hexAero ( drsysgzwpn * const dlisy2ivad , const real_T * ekfhkxngqv , const real_T lny2bptdha [ 3 ] , const real_T kolxe10nhw [ 3 ] , const real_T o0ikfmidn3 [ 9 ] , real_T oatswzrrgh [ 3 ] , real_T mny3zuhwny [ 3 ] , real_T * cgqvyhgltm , real_T * bl14v0mswn , ivogqrdqsb * localDW ) ; extern void hexAeroTID1 ( real_T hzdv0vh10t [ 3 ] ) ; extern void fjwoqbhtpm ( ivogqrdqsb * localDW , drsysgzwpn * const dlisy2ivad ) ;
#endif
