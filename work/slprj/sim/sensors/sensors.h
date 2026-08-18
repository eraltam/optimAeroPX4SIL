#ifndef sensors_h_
#define sensors_h_
#ifndef sensors_COMMON_INCLUDES_
#define sensors_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif
#include "sensors_types.h"
#include "gps.h"
#include "ins.h"
#include "ins_anello.h"
#include "ins_ideal.h"
#include "ins_anello_v2.h"
#include "adc.h"
#include "rtGetInf.h"
#include "rtGetNaN.h"
#include "rtw_modelmap.h"
#include <string.h>
#include <stddef.h>
typedef struct { real_T lk3piz4iuq ; real_T d44uhpk3uv ; real_T lfpxmxedwp ;
real_T ftlb4q2bpy ; real_T b4ckxuvhvt ; real_T njz01f1bls ; real_T ndhwzdkucx
; real_T nbum2p4q2l ; real_T drxisnudpt ; real_T neapywupj5 ; real_T
kekot5rn4d ; real_T hqbmcd2xcw ; real_T o0amnypjbz ; real_T feo5k25xsq ;
real_T fconjwkdyc ; real_T b5tzt53xdk ; real_T ment1jwzgs ; real_T n3inrdj0ka
; real_T jdh3rrj21z ; real_T bf13qjhy2b ; real_T kwusmz5ypj ; real_T
chovkahcpj ; } ol4emqpvsb ; typedef struct { pi2vdpe5wkl imlq10tlva ;
imyuptdnp20 o13ifkr2rx ; doifm542cwv lo1vzejkm0 ; ko1xfny3dgv b51jlyw3ea ;
ed0heanfedl dwmdafyntp ; ajsi3uhw15h abc2mgehbv ; } e2ptrxcrjh ; struct
irkooafj3n { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; rtwCAPI_ModelMappingInfo * childMMI [ 6 ] ; sysRanDType
* systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { ol4emqpvsb rtb ;
e2ptrxcrjh rtdw ; fcgqozqvuz rtm ; } b3bxwyuagcq ; extern
struct_7BigaldNWGj33rb8sElmtH rtP_parameters ; extern
struct_EwMtsSUDbGvLjUwS89lioH rtP_vehicle ; extern real_T rtP_INS_VARIANT ;
extern real_T rtP_anelloSiPhOGTemperatureCounts ; extern uint8_T
rtP_anelloOutputMode ; extern void a0zks0ycbp ( SimStruct * _mdlRefSfcnS ,
int_T mdlref_TID0 , int_T mdlref_TID1 , fcgqozqvuz * const nxtbamgl1k ,
ol4emqpvsb * localB , e2ptrxcrjh * localDW , void * sysRanPtr , int_T
contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T *
rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_sensors_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName ,
int_T * retVal ) ; extern mxArray * mr_sensors_GetDWork ( const b3bxwyuagcq *
mdlrefDW ) ; extern void mr_sensors_SetDWork ( b3bxwyuagcq * mdlrefDW , const
mxArray * ssDW ) ; extern void mr_sensors_RegisterSimStateChecksum ( SimStruct
* S ) ; extern mxArray * mr_sensors_GetSimStateDisallowedBlocks ( ) ; extern
const rtwCAPI_ModelMappingStaticInfo * sensors_GetCAPIStaticMap ( void ) ;
extern void p5rhivt1qj ( fcgqozqvuz * const nxtbamgl1k , e2ptrxcrjh * localDW
) ; extern void nxchj2otxt ( ol4emqpvsb * localB , e2ptrxcrjh * localDW ) ;
extern void e5og5o404u ( ol4emqpvsb * localB , e2ptrxcrjh * localDW ) ;
extern void m4f2mhs2e2 ( e2ptrxcrjh * localDW ) ; extern void ihfrza3mwz ( ol4emqpvsb * localB , e2ptrxcrjh * localDW ) ; extern void ppnzf4flpg ( e2ptrxcrjh * localDW ) ; extern void sensors ( const real_T * gw2o54hyug , const real_T pc1xqipjxh [ 3 ] , const real_T * mkuinhvhra , const real_T * m2cfhwgjar , const real_T * getakumxzj , const real_T egt5e4v2sm [ 3 ] , const real_T i54drskj2d [ 3 ] , const real_T aseybhdv4v [ 3 ] , const real_T jif0c1eig0 [ 3 ] , const real_T ive15jk0dv [ 9 ] , const real_T padxf0mpd2 [ 3 ] , const real_T cq1xjt4ohl [ 3 ] , const real_T deqciklimi [ 6 ] , SensorsBus * clxl221l0s , ol4emqpvsb * localB , e2ptrxcrjh * localDW ) ; extern void sensorsTID1 ( ol4emqpvsb * localB , e2ptrxcrjh * localDW ) ; extern void bmwvoaufe2 ( e2ptrxcrjh * localDW , fcgqozqvuz * const nxtbamgl1k ) ;
#endif
