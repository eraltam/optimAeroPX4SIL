#ifndef hexAeroCoefficientsModel_h_
#define hexAeroCoefficientsModel_h_
#ifndef hexAeroCoefficientsModel_COMMON_INCLUDES_
#define hexAeroCoefficientsModel_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "hexAeroCoefficientsModel_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
struct omjaewjshtj_ { real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ;
real_T P_7 [ 3 ] ; } ; struct nsjwh01qyt { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { mrrbvppoau rtm ; }
a442fa03xsq ; extern real_T rtP_C_D [ 19 ] ; extern real_T rtP_C_L [ 19 ] ;
extern real_T rtP_alphaBkpts_rad [ 19 ] ; extern void jvaz4icrry ( SimStruct
* _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , mrrbvppoau * const
imbcyjzm1j , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo *
rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T
rt_CSTATEIdx ) ; extern void mr_hexAeroCoefficientsModel_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_hexAeroCoefficientsModel_GetDWork ( const a442fa03xsq * mdlrefDW ) ; extern void mr_hexAeroCoefficientsModel_SetDWork ( a442fa03xsq * mdlrefDW , const mxArray * ssDW ) ; extern void mr_hexAeroCoefficientsModel_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_hexAeroCoefficientsModel_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * hexAeroCoefficientsModel_GetCAPIStaticMap ( void ) ; extern void l0mrvwrozf ( real_T ei0caupkdq [ 3 ] ) ; extern void ixegvndvdo ( real_T ei0caupkdq [ 3 ] ) ; extern void hexAeroCoefficientsModel ( const real_T * ohothlrcxr , const real_T * csnsczifqr , real_T n4gptmxx0q [ 3 ] ) ; extern void hexAeroCoefficientsModelTID1 ( real_T ei0caupkdq [ 3 ] ) ; extern void ifc1ugjrlf ( mrrbvppoau * const imbcyjzm1j ) ;
#endif
