#ifndef hexActuator_h_
#define hexActuator_h_
#ifndef hexActuator_COMMON_INCLUDES_
#define hexActuator_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "hexActuator_types.h"
#include "hexMotorModel.h"
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
typedef struct { PropulsionBus nuuz2o2teb ; } g4vajkqywi ; typedef struct {
cw0pxo3kgck al1yhguzap ; } k4jzygpeid ; typedef struct { ddcad1gntt
o2ufze3pgk ; } c1n3c5jiq0 ; typedef struct { njf3kis3qn o2ufze3pgk ; }
a3p5k3jlcf ; typedef struct { kd0hyhvl4l o2ufze3pgk ; } l3fqfaivlt ; struct
kz2wccbuitc_ { ActuatorBus P_3 ; } ; struct kpr0vnbqqc { struct SimStruct_tag
* _mdlRefSfcnS ; const rtTimingBridge * timingBridge ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; rtwCAPI_ModelMappingInfo * childMMI [ 1 ] ; sysRanDType
* systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 3 ] ; time_T tStart ; } Timing ; } ; typedef struct {
g4vajkqywi rtb ; k4jzygpeid rtdw ; pwmxrvh3p0 rtm ; } phqu2wcoxqx ; extern
real_T rtP_actuatorDelay_s ; extern real_T rtP_airDensityAtStd_kgpm3 ; extern
real_T rtP_rpm2radps ; extern void aoked5yefw ( SimStruct * _mdlRefSfcnS ,
ssNonContDerivSigFeedingOutports * * mr_nonContOutputArray , int_T
mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , pwmxrvh3p0 * const
iu5ahikyu1 , g4vajkqywi * localB , k4jzygpeid * localDW , c1n3c5jiq0 * localX
, void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo *
rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T
rt_CSTATEIdx ) ; extern void mr_hexActuator_MdlInfoRegFcn ( SimStruct *
mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray *
mr_hexActuator_GetDWork ( const phqu2wcoxqx * mdlrefDW ) ; extern void
mr_hexActuator_SetDWork ( phqu2wcoxqx * mdlrefDW , const mxArray * ssDW ) ;
extern void mr_hexActuator_RegisterSimStateChecksum ( SimStruct * S ) ;
extern mxArray * mr_hexActuator_GetSimStateDisallowedBlocks ( ) ; extern
const rtwCAPI_ModelMappingStaticInfo * hexActuator_GetCAPIStaticMap ( void )
; extern void ehudmthbfy ( pwmxrvh3p0 * const iu5ahikyu1 , k4jzygpeid *
localDW ) ; extern void isfesx5c5f ( k4jzygpeid * localDW , c1n3c5jiq0 *
localX ) ; extern void l01m5oasi2 ( k4jzygpeid * localDW , c1n3c5jiq0 *
localX ) ; extern void c4whjcydkw ( k4jzygpeid * localDW ) ; extern void
fhlpbjwxhc ( k4jzygpeid * localDW ) ; extern void gmkh4jpvvl ( k4jzygpeid *
localDW , c1n3c5jiq0 * localX , a3p5k3jlcf * localXdot ) ; extern void
mlabmlngdd ( k4jzygpeid * localDW ) ; extern void hexActuator ( const real_T
* l0fqx3vm2x , const real_T * j0kkmgx0ft , const real_T * cd4ehsparb , const
real_T * la0ycpgm1x , const real_T * fuvngbi5kl , const real_T * i3vpa10bz5 ,
const real_T * b55kzj0qbe , const real_T * pfnhpx5dfw , const boolean_T *
nvqdvwlbkt , const boolean_T * bq14spxznh , const boolean_T * k4zxlgpaik ,
const boolean_T * mwly5adbzk , const boolean_T * phrpw5avyv , const boolean_T
* ogse253bxp , real_T * kmkdd445wr , real_T * pq3y3izqww , real_T *
nrggeotkgi , real_T * o4d4wm1vvk , real_T * dg1ddfkmwa , real_T * btne03fscm
, real_T lfrvladest [ 6 ] , real_T dlg0dyphom [ 3 ] , real_T eubtino1pt [ 3 ]
, g4vajkqywi * localB , k4jzygpeid * localDW , c1n3c5jiq0 * localX ) ; extern
void hexActuatorTID2 ( k4jzygpeid * localDW ) ; extern void eaqlfcbv4w ( k4jzygpeid * localDW , pwmxrvh3p0 * const iu5ahikyu1 ) ;
#endif
