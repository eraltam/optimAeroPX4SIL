#ifndef Air_h_
#define Air_h_
#ifndef Air_COMMON_INCLUDES_
#define Air_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "Air_types.h"
#include "mwmathutil.h"
#include <string.h>
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <stddef.h>
typedef struct { real_T gwqljsmy1m ; } pmjpmc3xj2 ; typedef struct { int8_T
oksyj0jv2v ; boolean_T ozjk35vumb ; } j5szacujht ; typedef struct { real_T
lspp2304ya ; } gn2pgumz4h ; typedef struct { real_T lspp2304ya ; } hv54u51ltk
; typedef struct { boolean_T lspp2304ya ; } lvznggwtdn ; typedef struct {
real_T btypak0xif ; } bcefbm1m2y ; typedef struct { int8_T jh2h5ufsn0 ;
boolean_T e5g4ptdglk ; } kypgzv2ykp ; typedef struct { real_T kschmsqcob ; }
bg0la130cf ; typedef struct { real_T kschmsqcob ; } ilzcqndi0y ; typedef
struct { boolean_T kschmsqcob ; } pefpairmhu ; typedef struct { real_T
ol5oykzv2v ; real_T agyj1iaoug [ 4 ] ; real_T p00ei4l4xd ; real_T pf4lsroyp3
; real_T od3gkgq0lc ; real_T fbd0r1jhcx ; real_T fdmanaxskh ; real_T
ii4q5bfp2o [ 4 ] ; real_T dxcjt5g1fw ; real_T nychfbyo0r [ 2 ] ; real_T
hdlefdc55d [ 2 ] ; real_T hpqgqv2eqz [ 2 ] ; real_T fhbeo1mjnn ; real_T
nvkybncg10 [ 2 ] ; real_T a1bgujcvyk [ 2 ] ; real_T logwagaqhr [ 2 ] ; real_T
fvecychrcf [ 2 ] ; real_T jst5y43x4c [ 2 ] ; real_T jhcic30fag [ 2 ] ; real_T
hv4ubmu1wh [ 2 ] ; real_T i4i2k1cg3i [ 2 ] ; real_T n0yfa2z1g1 ; real_T
aqxp3hd5nf ; uint32_T dabba5xgcm ; bcefbm1m2y gxezufcodw ; bcefbm1m2y
kf3n24jn2o ; pmjpmc3xj2 fj0c5tjfqn ; bcefbm1m2y cp345krnyk ; bcefbm1m2y
irpvfptwfro ; pmjpmc3xj2 hcrr45luqwm ; } edb0zhmkiu ; typedef struct { real_T
lxhaghmv0h [ 4 ] ; struct { void * TimePtr ; void * DataPtr ; void *
RSimInfoPtr ; } h2v1q5jceo ; uint32_T hurhacjprj ; uint32_T hcfj2lop30 [ 4 ]
; uint32_T efh315cdzf ; struct { int_T PrevIndex ; } cidg01mdta ; int8_T
irozwsyzkd ; int8_T mdzsen2vfy ; int8_T hz2jcttmsa ; int8_T bpivcbzlyi ;
int8_T obi4asdoze ; int8_T isiljl3bu1 ; int8_T dj24cjnutv ; int8_T jcuathgxxh
; int8_T gg0vkz2ya5 ; int8_T ls4kddknva ; int8_T pntrgqb3jy ; int8_T
dkukdnfsuk ; int8_T ic0ali3zew ; int8_T k1kdhj0lsd ; int8_T gedgrbiut4 ;
int8_T fodof2x2t4 ; boolean_T nk3sd1bxtt ; boolean_T m5wgsi1kn3 ; boolean_T
aq12d0jlz0 ; boolean_T booxjldhpn ; boolean_T paz31qenjy ; boolean_T
fvkyh5znpd ; boolean_T okuhfrql5t ; boolean_T fvqkhh2vqk ; kypgzv2ykp
gxezufcodw ; kypgzv2ykp kf3n24jn2o ; j5szacujht fj0c5tjfqn ; kypgzv2ykp
cp345krnyk ; kypgzv2ykp irpvfptwfro ; j5szacujht hcrr45luqwm ; } ia44hvjo5y ;
typedef struct { real_T pjwr2w12wd [ 2 ] ; real_T i2oncsl4wy [ 2 ] ; real_T
coimpmfbov [ 2 ] ; real_T jqaupvvrr4 [ 2 ] ; real_T avoxcbo1i4 [ 2 ] ; real_T
mfmpwdweqf [ 2 ] ; real_T jbqlhg13ze [ 2 ] ; real_T etet3jaeye [ 2 ] ;
bg0la130cf gxezufcodw ; bg0la130cf kf3n24jn2o ; gn2pgumz4h fj0c5tjfqn ;
bg0la130cf cp345krnyk ; bg0la130cf irpvfptwfro ; gn2pgumz4h hcrr45luqwm ; }
pjetdmxrlz ; typedef struct { real_T pjwr2w12wd [ 2 ] ; real_T i2oncsl4wy [ 2
] ; real_T coimpmfbov [ 2 ] ; real_T jqaupvvrr4 [ 2 ] ; real_T avoxcbo1i4 [ 2
] ; real_T mfmpwdweqf [ 2 ] ; real_T jbqlhg13ze [ 2 ] ; real_T etet3jaeye [ 2
] ; ilzcqndi0y gxezufcodw ; ilzcqndi0y kf3n24jn2o ; hv54u51ltk fj0c5tjfqn ;
ilzcqndi0y cp345krnyk ; ilzcqndi0y irpvfptwfro ; hv54u51ltk hcrr45luqwm ; }
jshtpncyk1 ; typedef struct { boolean_T pjwr2w12wd [ 2 ] ; boolean_T
i2oncsl4wy [ 2 ] ; boolean_T coimpmfbov [ 2 ] ; boolean_T jqaupvvrr4 [ 2 ] ;
boolean_T avoxcbo1i4 [ 2 ] ; boolean_T mfmpwdweqf [ 2 ] ; boolean_T
jbqlhg13ze [ 2 ] ; boolean_T etet3jaeye [ 2 ] ; pefpairmhu gxezufcodw ;
pefpairmhu kf3n24jn2o ; lvznggwtdn fj0c5tjfqn ; pefpairmhu cp345krnyk ;
pefpairmhu irpvfptwfro ; lvznggwtdn hcrr45luqwm ; } f5nf4hs4w3 ; struct
o3zgyhpjji_ { real_T P_0 ; real_T P_1 ; real_T P_2 ; } ; struct ignwjs1yw5_ {
real_T P_0 ; real_T P_1 ; real_T P_2 ; } ; struct p2wdqwvq55n_ { real_T P_14
; real_T P_15 [ 4 ] ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T P_19 ;
real_T P_20 ; real_T P_21 ; real_T P_22 ; real_T P_23 ; real_T P_24 ; real_T
P_25 ; real_T P_26 ; real_T P_27 [ 4 ] ; boolean_T P_28 ; boolean_T P_29 ;
boolean_T P_30 ; boolean_T P_31 ; boolean_T P_32 ; boolean_T P_33 ; real_T
P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39 ;
real_T P_40 ; real_T P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 ; real_T
P_45 ; real_T P_46 ; real_T P_47 ; real_T P_48 ; real_T P_49 ; real_T P_50 ;
real_T P_51 ; real_T P_52 ; real_T P_53 ; real_T P_54 ; real_T P_55 ; real_T
P_56 ; real_T P_57 ; real_T P_58 ; real_T P_59 ; real_T P_60 ; real_T P_61 ;
real_T P_62 ; real_T P_63 ; real_T P_64 ; real_T P_65 ; real_T P_66 ; real_T
P_67 ; real_T P_68 ; real_T P_69 ; real_T P_70 ; real_T P_71 ; real_T P_72 ;
real_T P_73 [ 12 ] ; real_T P_74 [ 84 ] ; real_T P_75 ; real_T P_76 ; real_T
P_77 ; real_T P_78 ; real_T P_79 ; real_T P_80 ; real_T P_81 ; real_T P_82 [
7 ] ; real_T P_83 ; real_T P_84 ; real_T P_85 ; real_T P_86 ; real_T P_87 ;
real_T P_88 ; real_T P_89 ; real_T P_90 ; real_T P_91 ; real_T P_92 ; real_T
P_93 ; real_T P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ; real_T P_98 ;
real_T P_99 ; real_T P_100 ; uint32_T P_101 [ 2 ] ; uint8_T P_102 ; uint8_T
P_103 ; ignwjs1yw5 gxezufcodw ; ignwjs1yw5 kf3n24jn2o ; o3zgyhpjji fj0c5tjfqn
; ignwjs1yw5 cp345krnyk ; ignwjs1yw5 irpvfptwfro ; o3zgyhpjji hcrr45luqwm ; }
; struct fu4oizcycu { struct SimStruct_tag * _mdlRefSfcnS ; struct { real_T
mr_nonContSig0 [ 4 ] ; } NonContDerivMemory ; ssNonContDerivSigInfo
nonContDerivSignal [ 1 ] ; const rtTimingBridge * timingBridge ; struct {
rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMapLoggingInstanceInfo
mmiLogInstanceInfo ; void * dataAddress [ 14 ] ; int32_T * vardimsAddress [
14 ] ; RTWLoggingFcnPtr loggingPtrs [ 14 ] ; sysRanDType * systemRan [ 23 ] ;
int_T systemTid [ 23 ] ; } DataMapInfo ; struct { int_T mdlref_GlobalTID [ 4
] ; time_T tStart ; } Timing ; } ; typedef struct { edb0zhmkiu rtb ;
ia44hvjo5y rtdw ; eyx3wv2dh2 rtm ; } pp2a15jtu4d ; extern real_T
rtP_WIND_SOURCE ; extern real_T rtP_gustAmplitude_mps [ 3 ] ; extern real_T
rtP_gustDuration_sec ; extern real_T rtP_gustLength_m [ 3 ] ; extern real_T
rtP_gustStartTime_sec ; extern real_T rtP_turbulenceEndTime_sec ; extern
real_T rtP_turbulenceMinAirspeed_mps ; extern real_T
rtP_turbulenceStartTime_sec ; extern real_T rtP_turbulenceWingspan_m ; extern
real_T rtP_xWindInNED_mps ; extern real_T rtP_yWindInNED_mps ; extern real_T
rtP_zWindInNED_mps ; extern boolean_T rtP_gustEnabled ; extern boolean_T
rtP_turbulenceEnabled ; extern void d10mathqwe ( SimStruct * _mdlRefSfcnS ,
int_T mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , int_T mdlref_TID3
, eyx3wv2dh2 * const nvfdhrszo5 , edb0zhmkiu * localB , ia44hvjo5y * localDW
, pjetdmxrlz * localX , void * sysRanPtr , int_T contextTid ,
rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T
rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_Air_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_Air_GetDWork ( const pp2a15jtu4d * mdlrefDW ) ; extern void mr_Air_SetDWork ( pp2a15jtu4d * mdlrefDW , const mxArray * ssDW ) ; extern void mr_Air_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_Air_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * Air_GetCAPIStaticMap ( void ) ; extern void lsqh1j1amb ( pmjpmc3xj2 * localB , o3zgyhpjji * localP , gn2pgumz4h * localX ) ; extern void pl2xgzuyfd ( o3zgyhpjji * localP , gn2pgumz4h * localX ) ; extern void amveagwvyn ( real_T czrclaik1u , real_T b23oompyom , j5szacujht * localDW , o3zgyhpjji * localP , gn2pgumz4h * localX , hv54u51ltk * localXdot ) ; extern void mopdqyjpbc ( j5szacujht * localDW ) ; extern void hcrr45luqw ( eyx3wv2dh2 * const nvfdhrszo5 , boolean_T ggklh3z2e5 , real_T b23oompyom , pmjpmc3xj2 * localB , j5szacujht * localDW , o3zgyhpjji * localP , gn2pgumz4h * localX , lvznggwtdn * localXdis ) ; extern void kaeal31bei ( bcefbm1m2y * localB , ignwjs1yw5 * localP , bg0la130cf * localX ) ; extern void oah2qsrspg ( ignwjs1yw5 * localP , bg0la130cf * localX ) ; extern void in4kdo4fii ( real_T ovv5s253qy , real_T dsnwu1p50a , kypgzv2ykp * localDW , ignwjs1yw5 * localP , bg0la130cf * localX , ilzcqndi0y * localXdot ) ; extern void pd4cmxxlkm ( kypgzv2ykp * localDW ) ; extern void irpvfptwfr ( eyx3wv2dh2 * const nvfdhrszo5 , boolean_T n4jhdk1xsx , real_T dsnwu1p50a , bcefbm1m2y * localB , kypgzv2ykp * localDW , ignwjs1yw5 * localP , bg0la130cf * localX , pefpairmhu * localXdis ) ; extern void eml2bfmjzs ( real_T kmwa5g154p [ 3 ] , real_T blvg3sdhhh [ 3 ] , edb0zhmkiu * localB , ia44hvjo5y * localDW , pjetdmxrlz * localX ) ; extern void jgoqiwhsgd ( ia44hvjo5y * localDW ) ; extern void nbxqyvr5xm ( edb0zhmkiu * localB , ia44hvjo5y * localDW , pjetdmxrlz * localX , jshtpncyk1 * localXdot ) ; extern void lpjxqqjfle ( eyx3wv2dh2 * const nvfdhrszo5 , edb0zhmkiu * localB , ia44hvjo5y * localDW ) ; extern void hqhjziniu5 ( eyx3wv2dh2 * const nvfdhrszo5 , ia44hvjo5y * localDW ) ; extern void Air ( eyx3wv2dh2 * const nvfdhrszo5 , const real_T eg3sbfk44u [ 3 ] , const real_T doffi3prbl [ 3 ] , const real_T cpujxkpeof [ 9 ] , real_T * at5chrxe5y , real_T * fvlq0ypg1x , real_T * mz3d45gzwh , real_T gmsahj4sto [ 3 ] , real_T * c0ld11rjm1 , real_T kmwa5g154p [ 3 ] , real_T blvg3sdhhh [ 3 ] , edb0zhmkiu * localB , ia44hvjo5y * localDW , pjetdmxrlz * localX , f5nf4hs4w3 * localXdis ) ; extern void AirTID3 ( eyx3wv2dh2 * const nvfdhrszo5 , real_T g3egqkr3qt [ 3 ] , edb0zhmkiu * localB , ia44hvjo5y * localDW ) ; extern void f0nfjjh25d ( eyx3wv2dh2 * const nvfdhrszo5 ) ;
#endif
