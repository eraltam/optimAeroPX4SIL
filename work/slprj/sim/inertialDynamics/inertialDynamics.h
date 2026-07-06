#ifndef inertialDynamics_h_
#define inertialDynamics_h_
#ifndef inertialDynamics_COMMON_INCLUDES_
#define inertialDynamics_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "inertialDynamics_types.h"
#include <stddef.h>
#include <string.h>
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
typedef struct { real_T ky2q1e5bs2 [ 3 ] ; real_T os3juaf0bk [ 3 ] ; real_T
pdrhm2y1ox [ 4 ] ; real_T odv22m1qtt ; real_T eqsflawjuf ; real_T fgvroi5jrs
; real_T ba2w2wapem ; real_T iwqqtigxei [ 3 ] ; real_T avyaepajst ; real_T
gamsmcoxfg ; } d1ldtpt2uv ; typedef struct { real_T i1pbgixnyc [ 9 ] ; struct
{ void * AQHandles ; } df3w5gy4w4 ; struct { void * AQHandles ; } ljrrfd0li1
; struct { void * AQHandles ; } n0xnwhk4yr ; struct { void * AQHandles ; }
ofbivrsrru ; struct { void * AQHandles ; } jrn5rbjbaj ; struct { void *
AQHandles ; } chwzqmo3yg ; struct { void * AQHandles ; } dwc0fjndru ; struct
{ void * AQHandles ; } c5cozalbvp ; struct { void * AQHandles ; } bhw0ql5bm3
; struct { void * AQHandles ; } oa013zwwnn ; int8_T iifl2dwr1w ; int8_T
fnu3z0do3w ; int8_T pou2pibjcr ; int8_T a2qel35vnh ; int8_T ewx5cx1amh ;
int8_T p0o04ghnpb ; int8_T cn011ea52d ; int8_T nhiluv4y4e ; int8_T niwv0zpcul
; int8_T bq2bf2jl1a ; int8_T bxo2dasr0u ; int8_T dkklgbpgan ; int8_T
izb5qunafm ; int8_T l04ga0oywa ; int8_T fmz4dmdvz4 ; int8_T c14vd4y1qk ;
int8_T bgzclzzhfw ; int8_T d5jvwpkuiu ; int8_T lrtxwofxvp ; int8_T hgp5oylal5
; int8_T gk43f4mbi3 ; int8_T fmrnddkmir ; boolean_T dbjgga4msx ; } eme3h2xopw
; typedef struct { real_T f31o2xbo0p [ 3 ] ; real_T fcdsavymb5 [ 3 ] ; real_T
ewqcbelfa0 [ 4 ] ; real_T imy3vblrz0 [ 3 ] ; } bsfwj4wmdv ; typedef struct {
real_T f31o2xbo0p [ 3 ] ; real_T fcdsavymb5 [ 3 ] ; real_T ewqcbelfa0 [ 4 ] ;
real_T imy3vblrz0 [ 3 ] ; } fwfvoq03ou ; typedef struct { boolean_T
f31o2xbo0p [ 3 ] ; boolean_T fcdsavymb5 [ 3 ] ; boolean_T ewqcbelfa0 [ 4 ] ;
boolean_T imy3vblrz0 [ 3 ] ; } gtr31lho03 ; struct gbcsa11b4gr_ { real_T P_4
; real_T P_5 ; uint8_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ; real_T
P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 [ 9 ] ; real_T
P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T P_19 ; real_T P_20 ;
real_T P_21 ; real_T P_22 ; real_T P_23 ; real_T P_24 ; real_T P_25 ; real_T
P_26 ; real_T P_27 [ 3 ] ; real_T P_28 [ 2 ] ; real_T P_29 [ 2 ] ; real_T
P_30 [ 9 ] ; real_T P_31 ; real_T P_32 ; boolean_T P_33 ; boolean_T P_34 ;
boolean_T P_35 ; boolean_T P_36 ; } ; struct eiiinonju3 { struct
SimStruct_tag * _mdlRefSfcnS ; struct { real_T mr_nonContSig0 [ 3 ] ; }
NonContDerivMemory ; ssNonContDerivSigInfo nonContDerivSignal [ 1 ] ; const
rtTimingBridge * timingBridge ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; void * dataAddress [
4 ] ; int32_T * vardimsAddress [ 4 ] ; RTWLoggingFcnPtr loggingPtrs [ 4 ] ;
sysRanDType * systemRan [ 22 ] ; int_T systemTid [ 22 ] ; } DataMapInfo ;
struct { int_T mdlref_GlobalTID [ 3 ] ; time_T tStart ; } Timing ; } ;
typedef struct { d1ldtpt2uv rtb ; eme3h2xopw rtdw ; knpvazrmc0 rtm ; }
hjuis3flssz ; extern real_T rtP_aircraftInitialBodyRates_radps [ 3 ] ; extern
real_T rtP_aircraftInitialEuler_rad [ 3 ] ; extern real_T
rtP_aircraftInitialPosInNED_m [ 3 ] ; extern real_T
rtP_aircraftInitialVelInBody_mps [ 3 ] ; extern void b0l3jpudlu ( SimStruct *
_mdlRefSfcnS , ssNonContDerivSigFeedingOutports * * mr_nonContOutputArray ,
int_T mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , knpvazrmc0 *
const ag5wddik2h , d1ldtpt2uv * localB , eme3h2xopw * localDW , bsfwj4wmdv *
localX , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo *
rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T
rt_CSTATEIdx ) ; extern void mr_inertialDynamics_MdlInfoRegFcn ( SimStruct *
mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray *
mr_inertialDynamics_GetDWork ( const hjuis3flssz * mdlrefDW ) ; extern void
mr_inertialDynamics_SetDWork ( hjuis3flssz * mdlrefDW , const mxArray * ssDW
) ; extern void mr_inertialDynamics_RegisterSimStateChecksum ( SimStruct * S
) ; extern mxArray * mr_inertialDynamics_GetSimStateDisallowedBlocks ( ) ;
extern const rtwCAPI_ModelMappingStaticInfo *
inertialDynamics_GetCAPIStaticMap ( void ) ; extern void hfomxlcg2i ( knpvazrmc0
* const ag5wddik2h , eme3h2xopw * localDW ) ; extern void mpfq2mbxda ( knpvazrmc0 * const ag5wddik2h , eme3h2xopw * localDW , bsfwj4wmdv * localX ) ; extern void iussljnjzj ( knpvazrmc0 * const ag5wddik2h , eme3h2xopw * localDW , bsfwj4wmdv * localX ) ; extern void n3buml54cw ( knpvazrmc0 * const ag5wddik2h , eme3h2xopw * localDW ) ; extern void myvohxmyhq ( eme3h2xopw * localDW ) ; extern void bzjk22avbz ( real_T l2sbadi5f0 [ 3 ] , real_T lu3ww1tthm [ 3 ] , d1ldtpt2uv * localB , fwfvoq03ou * localXdot ) ; extern void oggmnjw5po ( knpvazrmc0 * const ag5wddik2h , eme3h2xopw * localDW ) ; extern void inertialDynamics ( knpvazrmc0 * const ag5wddik2h , const real_T pwz5vhxu5b [ 3 ] , const real_T mwfozkt4y3 [ 3 ] , const real_T * lhrezalqhg , const real_T jztgzc1a1t [ 9 ] , real_T ij1tgyusmb [ 3 ] , real_T goq5cscctd [ 3 ] , real_T f1i0cmg53p [ 3 ] , real_T ad1yqvdt34 [ 3 ] , real_T nxtwsqs0g5 [ 3 ] , real_T l2sbadi5f0 [ 3 ] , real_T lu3ww1tthm [ 3 ] , real_T of32zf34ps [ 9 ] , real_T nti542wsqh [ 3 ] , d1ldtpt2uv * localB , eme3h2xopw * localDW , bsfwj4wmdv * localX ) ; extern void inertialDynamicsTID2 ( d1ldtpt2uv * localB ) ; extern void ef45itsocu ( knpvazrmc0 * const ag5wddik2h ) ;
#endif
