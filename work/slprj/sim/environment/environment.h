#ifndef environment_h_
#define environment_h_
#ifndef environment_COMMON_INCLUDES_
#define environment_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "environment_types.h"
#include "Air.h"
#include "Earth.h"
#include "LocalTerrain.h"
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
typedef struct { pp2a15jtu4d b44lmhfkkz ; kzo2mbsde2x onovmkdvpt ;
a3tgdgrc1z2 lbkegtmmtx ; } otxhehhi2w ; typedef struct { pjetdmxrlz
fimefkpimo ; } ar45jsrhtq ; typedef struct { jshtpncyk1 fimefkpimo ; }
fgdgb2ruqd ; typedef struct { f5nf4hs4w3 fimefkpimo ; } djcyjwlyeb ; struct
cgjzqe2tvy { struct SimStruct_tag * _mdlRefSfcnS ; const rtTimingBridge *
timingBridge ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ;
rtwCAPI_ModelMappingInfo * childMMI [ 3 ] ; sysRanDType * systemRan [ 2 ] ;
int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T mdlref_GlobalTID [ 4 ]
; time_T tStart ; } Timing ; } ; typedef struct { otxhehhi2w rtdw ;
mojmojnvtk rtm ; } lj3yonrq1cm ; extern real_T rtP_gustAmplitude_mps [ 3 ] ;
extern real_T rtP_gustDuration_sec ; extern real_T rtP_gustLength_m [ 3 ] ;
extern real_T rtP_gustStartTime_sec ; extern real_T rtP_referenceAltitude_m ;
extern real_T rtP_referenceDecimalYear_years ; extern real_T
rtP_terrainHeightNED_m ; extern real_T rtP_turbulenceEndTime_sec ; extern
real_T rtP_turbulenceMinAirspeed_mps ; extern real_T
rtP_turbulenceStartTime_sec ; extern real_T rtP_turbulenceWingspan_m ; extern
real_T rtP_xWindInNED_mps ; extern real_T rtP_yWindInNED_mps ; extern real_T
rtP_zWindInNED_mps ; extern boolean_T rtP_gustEnabled ; extern boolean_T
rtP_turbulenceEnabled ; extern void d2n3xqbspm ( SimStruct * _mdlRefSfcnS ,
int_T mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , int_T mdlref_TID3
, mojmojnvtk * const chs3edang5 , otxhehhi2w * localDW , ar45jsrhtq * localX
, void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo *
rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T
rt_CSTATEIdx ) ; extern void mr_environment_MdlInfoRegFcn ( SimStruct *
mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray *
mr_environment_GetDWork ( const lj3yonrq1cm * mdlrefDW ) ; extern void
mr_environment_SetDWork ( lj3yonrq1cm * mdlrefDW , const mxArray * ssDW ) ;
extern void mr_environment_RegisterSimStateChecksum ( SimStruct * S ) ;
extern mxArray * mr_environment_GetSimStateDisallowedBlocks ( ) ; extern
const rtwCAPI_ModelMappingStaticInfo * environment_GetCAPIStaticMap ( void )
; extern void btkehhwypc ( mojmojnvtk * const chs3edang5 , otxhehhi2w *
localDW ) ; extern void dqw2pebsxa ( real_T k0jizrqbpm [ 3 ] , real_T
htdjgzerpb [ 3 ] , real_T * jvxwvnqbed , otxhehhi2w * localDW , ar45jsrhtq *
localX ) ; extern void cvgtj0ljml ( otxhehhi2w * localDW ) ; extern void
h04mg41egj ( otxhehhi2w * localDW ) ; extern void frbuztrvod ( real_T *
jvxwvnqbed , otxhehhi2w * localDW ) ; extern void eur0hdmr0t ( otxhehhi2w *
localDW , ar45jsrhtq * localX , fgdgb2ruqd * localXdot ) ; extern void
modkwf3jsj ( otxhehhi2w * localDW ) ; extern void jcjsocq2aa ( mojmojnvtk *
const chs3edang5 , otxhehhi2w * localDW ) ; extern void environment ( mojmojnvtk
* const chs3edang5 , const real_T bdoxnlmvb5 [ 3 ] , const real_T nrbkpcwykv
[ 3 ] , const real_T hdal4gcyr4 [ 9 ] , real_T * lc2l1ifrdc , real_T
jnjk2hkwbs [ 3 ] , real_T iiowwd1ole [ 3 ] , real_T * eh5pp5enrj , real_T *
eqzen13jm1 , real_T * o35xut5vvt , real_T nvlkzczvdw [ 3 ] , real_T *
fwxuqwezyk , real_T k0jizrqbpm [ 3 ] , real_T htdjgzerpb [ 3 ] , otxhehhi2w *
localDW , ar45jsrhtq * localX , djcyjwlyeb * localXdis ) ; extern void
environmentTID3 ( real_T d2bzr133u2 [ 3 ] , real_T * jvxwvnqbed , otxhehhi2w
* localDW ) ; extern void mzncbscp1i ( otxhehhi2w * localDW , mojmojnvtk *
const chs3edang5 ) ;
#endif
