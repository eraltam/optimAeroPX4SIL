#ifndef VehiclePlant_h_
#define VehiclePlant_h_
#ifndef VehiclePlant_COMMON_INCLUDES_
#define VehiclePlant_COMMON_INCLUDES_
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
#include "VehiclePlant_types.h"
#include "environment.h"
#include "sensors.h"
#include "c172p.h"
#include <string.h>
#include "rtGetNaN.h"
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include <stddef.h>
typedef struct { real_T bqjhakilic [ 3 ] ; real_T a5ohxxrqrt [ 3 ] ; real_T
dodurerrnq [ 9 ] ; real_T looijvfkun [ 3 ] ; real_T expr0rzl25 [ 3 ] ; real_T
bx1scxto2n [ 3 ] ; real_T kc0argbbnf [ 3 ] ; real_T px0hm5xxcz [ 3 ] ; real_T
lueeweqsa2 ; real_T hyukrinvq1 [ 3 ] ; real_T m3444qfxy3 [ 9 ] ; real_T
oq3h5ln2dn ; real_T ldipbnnnjo [ 3 ] ; real_T euacwk1nwa [ 3 ] ; real_T
b15jhl353f [ 3 ] ; real_T nr1z2kw4vv [ 3 ] ; real_T lyavlgnaap [ 3 ] ; real_T
hqfsyz0gws [ 3 ] ; real_T ebsrm0pt0f [ 3 ] ; real_T ckxl0kwbp3 [ 3 ] ; real_T
pk015gkn3t [ 3 ] ; real_T hnwpw5cpbg [ 3 ] ; real_T hpzcreebww ; real_T
jesjnjq4nt ; real_T g33bndehud [ 3 ] ; real_T l02njpb4l0 [ 3 ] ; real_T
nqezebprs0 ; real_T c3upje4rrj ; real_T f1pootioah ; real_T hg3e2b4k10 ;
real_T bj3atmcbie ; real_T mipewzbi5z [ 3 ] ; real_T f4wzraydcv [ 3 ] ;
real_T cudyvx3g2k ; boolean_T ll4ypkaxem ; } n4i4q3iehd ; typedef struct {
real_T gfi4rwv5uv [ 3 ] ; real_T albizm10c2 [ 3 ] ; real_T oqzh4go2fj [ 9 ] ;
real_T f5cqf0v5ma [ 3 ] ; real_T egmzw14bcz [ 3 ] ; real_T jer4mfbauo [ 3 ] ;
real_T mofdx3lgfc [ 3 ] ; real_T cbkc3lpahs [ 3 ] ; real_T pq1usl0ji3 ;
real_T prddltbc3q [ 3 ] ; real_T pflzj3aecu [ 9 ] ; real_T ehq4dn4qy5 ;
real_T blxbp5visn [ 3 ] ; real_T iwfnzut3di [ 3 ] ; real_T btm5zgjsa1 [ 3 ] ;
real_T hi5wgo0pzs [ 3 ] ; real_T lflbnbwxow [ 3 ] ; real_T jb5suc3utz [ 3 ] ;
real_T a545g2ksuw [ 3 ] ; real_T iofhgaqk00 [ 3 ] ; real_T izoq4rgghj [ 3 ] ;
real_T g33wknofwp [ 3 ] ; real_T niniuhz0at ; real_T ivelzrae45 ; real_T
hpcdwfht1y [ 3 ] ; real_T oh42dzxsr1 [ 3 ] ; boolean_T amcgr3jf34 ;
lj3yonrq1cm nfw1ojnku2 ; b3bxwyuagcq l4fx0iisxh ; avyhir3zekl e3vcmf1wna ; }
j0qkcyobx0 ; typedef struct { ar45jsrhtq jn4jwo3o0t ; dqz4wk1hvy nucdhb4glb ;
} hdulqcnnpv ; typedef struct { fgdgb2ruqd jn4jwo3o0t ; iiocpe4xx5 nucdhb4glb
; } p1wcsoehle ; typedef struct { djcyjwlyeb jn4jwo3o0t ; idfb2cojsg
nucdhb4glb ; } o2ljf02bvi ; struct ks3glix3e01_ { real_T P_25 ; real_T P_26 ;
real_T P_27 ; real_T P_28 ; real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T
P_32 ; real_T P_33 ; real_T P_34 ; real_T P_35 ; real_T P_36 ; real_T P_37 ;
real_T P_38 ; real_T P_39 ; real_T P_40 ; real_T P_41 ; real_T P_42 ; real_T
P_43 ; real_T P_44 ; real_T P_45 ; real_T P_46 ; real_T P_47 ; real_T P_48 ;
real_T P_49 ; real_T P_50 ; boolean_T P_51 ; } ; struct d2l3uzft5j { struct
SimStruct_tag * _mdlRefSfcnS ; struct { uint8_T mr_nonContSig0 [ 24 ] ;
uint8_T mr_nonContSig1 [ 24 ] ; uint8_T mr_nonContSig2 [ 8 ] ; uint8_T
mr_nonContSig3 [ 72 ] ; uint8_T mr_nonContSig4 [ 24 ] ; uint8_T
mr_nonContSig5 [ 8 ] ; uint8_T mr_nonContSig6 [ 24 ] ; uint8_T mr_nonContSig7
[ 72 ] ; uint8_T mr_nonContSig8 [ 24 ] ; uint8_T mr_nonContSig9 [ 24 ] ;
uint8_T mr_nonContSig10 [ 24 ] ; uint8_T mr_nonContSig11 [ 24 ] ; uint8_T
mr_nonContSig12 [ 24 ] ; uint8_T mr_nonContSig13 [ 24 ] ; uint8_T
mr_nonContSig14 [ 24 ] ; uint8_T mr_nonContSig15 [ 24 ] ; uint8_T
mr_nonContSig16 [ 24 ] ; uint8_T mr_nonContSig17 [ 24 ] ; uint8_T
mr_nonContSig18 [ 24 ] ; uint8_T mr_nonContSig19 [ 24 ] ; uint8_T
mr_nonContSig20 [ 24 ] ; uint8_T mr_nonContSig21 [ 24 ] ; uint8_T
mr_nonContSig22 [ 24 ] ; uint8_T mr_nonContSig23 [ 1 ] ; uint8_T
mr_nonContSig24 [ 24 ] ; uint8_T mr_nonContSig25 [ 24 ] ; uint8_T
mr_nonContSig26 [ 8 ] ; uint8_T mr_nonContSig27 [ 72 ] ; uint8_T
mr_nonContSig28 [ 24 ] ; uint8_T mr_nonContSig29 [ 8 ] ; uint8_T
mr_nonContSig30 [ 24 ] ; uint8_T mr_nonContSig31 [ 72 ] ; uint8_T
mr_nonContSig32 [ 24 ] ; uint8_T mr_nonContSig33 [ 24 ] ; uint8_T
mr_nonContSig34 [ 24 ] ; uint8_T mr_nonContSig35 [ 24 ] ; uint8_T
mr_nonContSig36 [ 24 ] ; uint8_T mr_nonContSig37 [ 24 ] ; uint8_T
mr_nonContSig38 [ 24 ] ; uint8_T mr_nonContSig39 [ 24 ] ; uint8_T
mr_nonContSig40 [ 24 ] ; uint8_T mr_nonContSig41 [ 24 ] ; uint8_T
mr_nonContSig42 [ 24 ] ; uint8_T mr_nonContSig43 [ 24 ] ; uint8_T
mr_nonContSig44 [ 24 ] ; uint8_T mr_nonContSig45 [ 24 ] ; uint8_T
mr_nonContSig46 [ 24 ] ; uint8_T mr_nonContSig47 [ 1 ] ; real_T
mr_nonContSig48 [ 1 ] ; real_T mr_nonContSig49 [ 3 ] ; real_T mr_nonContSig50
[ 3 ] ; real_T mr_nonContSig51 [ 3 ] ; real_T mr_nonContSig52 [ 1 ] ; real_T
mr_nonContSig53 [ 3 ] ; real_T mr_nonContSig54 [ 3 ] ; } NonContDerivMemory ;
ssNonContDerivSigInfo nonContDerivSignal [ 55 ] ; const rtTimingBridge *
timingBridge ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ;
rtwCAPI_ModelMappingInfo * childMMI [ 3 ] ; sysRanDType * systemRan [ 2 ] ;
int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T mdlref_GlobalTID [ 4 ]
; time_T tStart ; } Timing ; } ; typedef struct { n4i4q3iehd rtb ; j0qkcyobx0
rtdw ; hl0stsvgru rtm ; } owiw5mmjy2f ; extern struct_7BigaldNWGj33rb8sElmtH
rtP_parameters ; extern real_T rtP_INS_VARIANT ; extern real_T
rtP_actuatorDelay_s ; extern real_T rtP_aircraftInitialBodyRates_radps [ 3 ]
; extern real_T rtP_aircraftInitialEuler_rad [ 3 ] ; extern real_T
rtP_aircraftInitialPosInNED_m [ 3 ] ; extern real_T
rtP_aircraftInitialVelInBody_mps [ 3 ] ; extern real_T rtP_gustAmplitude_mps
[ 3 ] ; extern real_T rtP_gustDuration_sec ; extern real_T rtP_gustLength_m [
3 ] ; extern real_T rtP_gustStartTime_sec ; extern real_T rtP_m2ft ; extern
real_T rtP_plantSampleTime_s ; extern real_T rtP_referenceAltitude_m ; extern
real_T rtP_referenceDecimalYear_years ; extern real_T rtP_terrainHeightNED_m
; extern real_T rtP_turbulenceEndTime_sec ; extern real_T
rtP_turbulenceMinAirspeed_mps ; extern real_T rtP_turbulenceStartTime_sec ;
extern real_T rtP_turbulenceWingspan_m ; extern real_T rtP_xWindInNED_mps ;
extern real_T rtP_yWindInNED_mps ; extern real_T rtP_zWindInNED_mps ; extern
boolean_T rtP_gustEnabled ; extern boolean_T rtP_turbulenceEnabled ; extern
void dwjsblzuar ( real_T * ecxaknsqkl , real_T lt4aghk1iw [ 3 ] , real_T
ftw4ivecrm [ 9 ] , real_T * ko4m023alm , real_T n1zvrfkna2 [ 3 ] , real_T
bgol22e1md [ 3 ] , real_T h0rreu5sbd [ 3 ] , real_T * kwufbgkpfu , SimStruct
* _mdlRefSfcnS , ssNonContDerivSigFeedingOutports * * mr_nonContOutputArray ,
int_T mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , int_T mdlref_TID3
, hl0stsvgru * const pyftoj5or2 , n4i4q3iehd * localB , j0qkcyobx0 * localDW
, hdulqcnnpv * localX , void * sysRanPtr , int_T contextTid ,
rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T
rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_VehiclePlant_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName
, int_T * retVal ) ; extern mxArray * mr_VehiclePlant_GetDWork ( const
owiw5mmjy2f * mdlrefDW ) ; extern void mr_VehiclePlant_SetDWork ( owiw5mmjy2f
* mdlrefDW , const mxArray * ssDW ) ; extern void
mr_VehiclePlant_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray *
mr_VehiclePlant_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * VehiclePlant_GetCAPIStaticMap ( void ) ;
extern void gyzb3lqody ( hl0stsvgru * const pyftoj5or2 , j0qkcyobx0 * localDW
) ; extern void cjxjkcm4ra ( real_T * ecxaknsqkl , real_T lt4aghk1iw [ 3 ] ,
real_T ftw4ivecrm [ 9 ] , real_T k00tjyckxa [ 3 ] , real_T dcd2azrgeb [ 3 ] ,
real_T * kwufbgkpfu , j0qkcyobx0 * localDW , hdulqcnnpv * localX ) ; extern
void dapgvgjvnf ( j0qkcyobx0 * localDW , hdulqcnnpv * localX ) ; extern void
dgmiwxg00f ( j0qkcyobx0 * localDW ) ; extern void hqrg4vgws5 ( real_T *
ecxaknsqkl , real_T lt4aghk1iw [ 3 ] , real_T ftw4ivecrm [ 9 ] , real_T *
kwufbgkpfu , j0qkcyobx0 * localDW ) ; extern void m5u2c0ldal ( real_T
auljbjoni3 [ 3 ] , real_T cpvuhtt0lv [ 3 ] , j0qkcyobx0 * localDW ,
hdulqcnnpv * localX , p1wcsoehle * localXdot ) ; extern void iv0fdu4uqg ( j0qkcyobx0 * localDW ) ; extern void bdatrumzy4 ( hl0stsvgru * const pyftoj5or2 , const real_T * ivajpnivog , const real_T * o5aggu03es , const real_T * fo3tsly54o , const real_T * lywo5ttb4s , const real_T * dvvgrn0ejk , boolean_T * hw4ud53t4i , real_T lpknqxvuts [ 3 ] , real_T icurnhfbrt [ 3 ] , real_T ex10gglxu0 [ 3 ] , real_T eohfsxacwo [ 3 ] , real_T bbs1guuyy5 [ 3 ] , real_T auljbjoni3 [ 3 ] , real_T cpvuhtt0lv [ 3 ] , real_T blgyf1f2n2 [ 9 ] , real_T eivnrdlxkh [ 3 ] , real_T * ecxaknsqkl , real_T lt4aghk1iw [ 3 ] , real_T ftw4ivecrm [ 9 ] , real_T ejlqgjk2hg [ 3 ] , real_T ig4vmlian3 [ 3 ] , real_T lo10f1i4iq [ 3 ] , real_T ghbfmul40f [ 3 ] , real_T pi1h410ylh [ 3 ] , real_T gvswycs54x [ 3 ] , real_T iuql2xglpu [ 3 ] , real_T bv0gqz1l5u [ 3 ] , real_T blphgy5ou4 [ 3 ] , real_T fge5ax5fk0 [ 3 ] , real_T ed3b0u3lmx [ 3 ] , real_T * o5qyeuxkac , real_T * fsjpvizlgw , n4i4q3iehd * localB , j0qkcyobx0 * localDW ) ; extern void VehiclePlant ( hl0stsvgru * const pyftoj5or2 , const boolean_T * lozvk3ss52 , const boolean_T * idqvu1xqjj , const boolean_T * duap5wwx55 , const boolean_T * gm3kb504fe , const real_T * ivajpnivog , const real_T * o5aggu03es , const real_T * fo3tsly54o , const real_T * lywo5ttb4s , const real_T * dvvgrn0ejk , real_T * mstyey3rtt , real_T * hufjamtm55 , real_T * ls2onjolga , real_T * f40m32jcmk , real_T * nip4mqz4ij , real_T * pa0fdhsz0p , real_T * ephwh0y3f1 , real_T * iodotc34bs , real_T * n2ngeds1qk , real_T * gabaplfntu , real_T * kbl1kmk11v , real_T * nr5k3so5eg , real_T * h0mjlv2m5d , real_T * d3bmyrhx1l , real_T * nv43byhxzc , real_T * ompa0khus5 , real_T * cggbfg0v5w , real_T * eaj5j4e2bg , real_T * clihtfkqdd , real_T * b3y0ukvlja , real_T * iwlk0s13aq , real_T * hssvnv0gv3 , real_T * b3bh2ei22t , real_T * dmdoff23wk , real_T * mpkhlwbcwa , real_T * a32i3ri33a , real_T * futrjniu4o , real_T * aubh0rmgix , real_T * h0onumuhoc , real_T * o4vxxrplyr , real_T * nlpogoaprs , real_T * dsipu5kvoh , real_T * k4emq5vbc0 , real_T * iqt5hdafs0 , boolean_T * hw4ud53t4i , real_T lpknqxvuts [ 3 ] , real_T icurnhfbrt [ 3 ] , real_T ex10gglxu0 [ 3 ] , real_T eohfsxacwo [ 3 ] , real_T bbs1guuyy5 [ 3 ] , real_T auljbjoni3 [ 3 ] , real_T cpvuhtt0lv [ 3 ] , real_T blgyf1f2n2 [ 9 ] , real_T eivnrdlxkh [ 3 ] , real_T * ecxaknsqkl , real_T lt4aghk1iw [ 3 ] , real_T ftw4ivecrm [ 9 ] , real_T ejlqgjk2hg [ 3 ] , real_T ig4vmlian3 [ 3 ] , real_T lo10f1i4iq [ 3 ] , real_T ghbfmul40f [ 3 ] , real_T pi1h410ylh [ 3 ] , real_T gvswycs54x [ 3 ] , real_T iuql2xglpu [ 3 ] , real_T bv0gqz1l5u [ 3 ] , real_T blphgy5ou4 [ 3 ] , real_T fge5ax5fk0 [ 3 ] , real_T ed3b0u3lmx [ 3 ] , real_T * o5qyeuxkac , real_T * fsjpvizlgw , real_T * ko4m023alm , real_T n1zvrfkna2 [ 3 ] , real_T bgol22e1md [ 3 ] , real_T * ib4lk3cklq , real_T * pbob4bzuvp , real_T * hrdkeldjwa , real_T e3zjgq4jnv [ 3 ] , real_T * jqrjrt5g3q , real_T k00tjyckxa [ 3 ] , real_T dcd2azrgeb [ 3 ] , real_T * kwufbgkpfu , n4i4q3iehd * localB , j0qkcyobx0 * localDW , hdulqcnnpv * localX , o2ljf02bvi * localXdis ) ; extern void VehiclePlantTID3 ( real_T * ecxaknsqkl , real_T lt4aghk1iw [ 3 ] , real_T ftw4ivecrm [ 9 ] , real_T h0rreu5sbd [ 3 ] , real_T * kwufbgkpfu , j0qkcyobx0 * localDW ) ; extern void omtgknbblj ( j0qkcyobx0 * localDW , hl0stsvgru * const pyftoj5or2 ) ;
#endif
