#ifndef gps_h_
#define gps_h_
#ifndef gps_COMMON_INCLUDES_
#define gps_COMMON_INCLUDES_
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
#include "gps_types.h"
#include <stddef.h>
#include "rtGetNaN.h"
#include "rtw_modelmap.h"
#include <string.h>
typedef struct { o3ise5hhyf ixw3vsxa4a ; struct { void * AQHandles ; }
i1klam4rbf ; struct { void * AQHandles ; } c15xjzrv5y ; struct { void *
AQHandles ; } gtzzdwm1ha ; boolean_T pkcwxr0laa ; } nnakepwb4s ; struct
hj5kjp0tolf_ { real_T P_1 ; real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5
; } ; struct d2natfdpdq { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 3 ] ; int_T systemTid [ 3 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ;
typedef struct { nnakepwb4s rtdw ; jdu5i0vx12 rtm ; } pi2vdpe5wkl ; extern
struct_7BigaldNWGj33rb8sElmtH rtP_parameters ; extern void ec3hi2jtp3 ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , jdu5i0vx12 * const kobisnulh4 , nnakepwb4s * localDW , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_gps_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_gps_GetDWork ( const pi2vdpe5wkl * mdlrefDW ) ; extern void mr_gps_SetDWork ( pi2vdpe5wkl * mdlrefDW , const mxArray * ssDW ) ; extern void mr_gps_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_gps_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * gps_GetCAPIStaticMap ( void ) ; extern void jmjsan0tw0 ( jdu5i0vx12 * const kobisnulh4 , nnakepwb4s * localDW ) ; extern void iythuls0ml ( real_T * e4kaqlkprq , nnakepwb4s * localDW ) ; extern void i0y1ktvx0c ( real_T * e4kaqlkprq , nnakepwb4s * localDW ) ; extern void gboycxyz1v ( jdu5i0vx12 * const kobisnulh4 , nnakepwb4s * localDW ) ; extern void p0tbb3g51c ( real_T * e4kaqlkprq , nnakepwb4s * localDW ) ; extern void gps ( jdu5i0vx12 * const kobisnulh4 , const real_T jvcspwu2yr [ 3 ] , const real_T cz4erijwgz [ 3 ] , real_T * oddi4kkqlv , real_T * dk3vd5lkq4 , real_T * b1gvd25c2h , real_T * ms5tffbepa , real_T * ktfae4sdkm , real_T * cs4h5ue3zd , real_T * c54iyoav2s , real_T * g1q25c4q3n , nnakepwb4s * localDW ) ; extern void gpsTID1 ( real_T * e4kaqlkprq , real_T * pvkmy3orih , real_T * bbehsltnxc , real_T * cryhxrayy0 ) ; extern void eqbaymsgcx ( jdu5i0vx12 * const kobisnulh4 ) ;
#endif
