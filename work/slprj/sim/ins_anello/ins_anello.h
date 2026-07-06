#ifndef ins_anello_h_
#define ins_anello_h_
#ifndef ins_anello_COMMON_INCLUDES_
#define ins_anello_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#include "sfrtif/sim_shared_utils.h"
#include "sfrtif/sfc_sdi.h"
#endif
#include "ins_anello_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
#include <stddef.h>
typedef struct { real_T kh3vz5gerh [ 3 ] ; real_T knh3r5acqo [ 3 ] ; real_T
lekl3pkhkb [ 3 ] ; } asr0gh2w2s ; typedef struct { cqn2imq1f3 mwwgimhnfv ;
real_T jcchdjcjl5 [ 3 ] ; real_T dnkrvpzdnt [ 3 ] ; real_T ngxalhmiod [ 3 ] ;
uint32_T mti1oodtgw ; uint32_T mjwpgzgjg5 [ 2 ] ; uint32_T fh34cih4qi ;
uint32_T mwt013giti ; uint32_T pm2g5mkvef [ 2 ] ; uint32_T ok1yphwfw3 [ 625 ]
; } a4mpwoagne ; struct hfnefzrumxr_ { real_T P_0 ; real_T P_1 ; real_T P_2 ;
real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; } ; struct
m3pw3ynq10 { struct SimStruct_tag * _mdlRefSfcnS ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; sysRanDType * systemRan [ 3 ] ; int_T systemTid [ 3 ] ;
} DataMapInfo ; struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ;
typedef struct { asr0gh2w2s rtb ; a4mpwoagne rtdw ; psmsewigyz rtm ; }
doifm542cwv ; extern void l55uemkp5l ( SimStruct * _mdlRefSfcnS , int_T
mdlref_TID0 , int_T mdlref_TID1 , psmsewigyz * const ckf43r0kru , asr0gh2w2s
* localB , a4mpwoagne * localDW , void * sysRanPtr , int_T contextTid ,
rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T
rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_ins_anello_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName ,
int_T * retVal ) ; extern mxArray * mr_ins_anello_GetDWork ( const
doifm542cwv * mdlrefDW ) ; extern void mr_ins_anello_SetDWork ( doifm542cwv *
mdlrefDW , const mxArray * ssDW ) ; extern void
mr_ins_anello_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray *
mr_ins_anello_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * ins_anello_GetCAPIStaticMap ( void ) ;
extern void dwtc0bnvtm ( real_T * b0ypqjdi00 , a4mpwoagne * localDW ) ;
extern void kfkrrajk1l ( real_T * b0ypqjdi00 , a4mpwoagne * localDW ) ;
extern void njopuutrc4 ( real_T * b0ypqjdi00 ) ; extern void ins_anello ( const
real_T * f2s2kzfowg , const real_T d2i1fnaxpu [ 3 ] , const real_T *
cupt41vqn3 , const real_T offmb2kyxd [ 3 ] , const real_T iqqyak1ail [ 3 ] ,
const real_T gk0fgkty0f [ 9 ] , const real_T iybki31sia [ 3 ] , real_T *
porrdmp0jr , real_T * algdtukvm3 , real_T * phhhw42hgb , real_T * n2ruutxtiv
, real_T * j3hgwkembg , real_T * apn4zaehfl , real_T * ka5fnfj3a5 , real_T *
hoexm2xmnl , real_T * hxa1lxpkae , real_T * eojhszhwdi , real_T * c102ivdo3v
, real_T * cli04m0p4a , asr0gh2w2s * localB , a4mpwoagne * localDW ) ; extern
void ins_anelloTID1 ( real_T * b0ypqjdi00 , real_T * ivj0jeid4h , real_T *
chu0xdxmke ) ; extern void namfu1ihid ( psmsewigyz * const ckf43r0kru ) ;
#endif
