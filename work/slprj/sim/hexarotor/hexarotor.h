#ifndef hexarotor_h_
#define hexarotor_h_
#ifndef hexarotor_COMMON_INCLUDES_
#define hexarotor_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "hexarotor_types.h"
#include "hexAddFm.h"
#include "hexActuator.h"
#include "cgMassInertia.h"
#include "inertialDynamics.h"
#include "gravity.h"
#include "hexGroundContact.h"
#include "hexAero.h"
#include <string.h>
#include "model_reference_types.h"
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <stddef.h>
typedef struct { real_T pkibsq1qve [ 3 ] ; real_T hln5bknilm [ 3 ] ; real_T
biif5qc31h [ 3 ] ; real_T i2ixzkbk3a [ 3 ] ; real_T fm1kkkeazl [ 3 ] ; real_T
dn0lo0he5b [ 3 ] ; real_T oiydyssyu2 [ 3 ] ; real_T e0ghytr4tt [ 3 ] ; real_T
ozbif4y5li [ 3 ] ; real_T nposw3mdip [ 3 ] ; real_T mayhkvslkd [ 3 ] ; real_T
jxntpo1upb [ 3 ] ; real_T dvgcq2vei3 ; real_T de4z54og02 ; real_T kzjhbpuixx
[ 3 ] ; real_T ptxc4xohqo [ 3 ] ; real_T iers3y0vyu [ 3 ] ; real_T niblp4v1ek
; } lcs4qsfhas ; typedef struct { real_T iw4ied421v [ 3 ] ; real_T pt5mgpaqyj
[ 3 ] ; real_T cu1gkhcrdx [ 3 ] ; real_T gdhlwcgsiw [ 3 ] ; real_T pfnaed2abh
[ 3 ] ; real_T cfkvxqqnrq [ 3 ] ; real_T iin4l0vil5 [ 3 ] ; real_T hgpjgm0n5c
[ 3 ] ; real_T ognq3ystny [ 3 ] ; real_T hwg1mx20go ; real_T nzfoyimojx ;
struct { void * AQHandles [ 8 ] ; } k1b2hkzxz4 ; struct { void * AQHandles [
2 ] ; } hv0i51ivtm ; eckipmdqplk j5yjzzrzy3 ; phqu2wcoxqx fbo1p5ueqs ;
jdfkzpcqy5j eldj4zbwth ; hjuis3flssz chjbm4cvkx ; e53sj53lngm kx2pmlps33 ;
dz2poepecm5 pnviwaj1fk ; f0datyenzg5 psc2f00o1x ; } pzfrokszxg ; typedef
struct { c1n3c5jiq0 jvyy50taxt ; bsfwj4wmdv dsjqtb5tzu ; } plypikssk0 ;
typedef struct { a3p5k3jlcf jvyy50taxt ; fwfvoq03ou dsjqtb5tzu ; } lpahjwu50g
; typedef struct { l3fqfaivlt jvyy50taxt ; gtr31lho03 dsjqtb5tzu ; }
bx03knnvpe ; struct cw0e20kstic_ { real_T P_10 ; real_T P_11 ; real_T P_12 ;
real_T P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T
P_18 ; real_T P_19 ; real_T P_20 ; real_T P_21 ; } ; struct ctkchtprgd {
struct SimStruct_tag * _mdlRefSfcnS ; struct { real_T mr_nonContSig0 [ 1 ] ;
real_T mr_nonContSig1 [ 9 ] ; real_T mr_nonContSig2 [ 3 ] ; real_T
mr_nonContSig3 [ 1 ] ; uint8_T mr_nonContSig4 [ 8 ] ; uint8_T mr_nonContSig5
[ 8 ] ; uint8_T mr_nonContSig6 [ 8 ] ; uint8_T mr_nonContSig7 [ 8 ] ; uint8_T
mr_nonContSig8 [ 8 ] ; uint8_T mr_nonContSig9 [ 8 ] ; uint8_T mr_nonContSig10
[ 8 ] ; uint8_T mr_nonContSig11 [ 8 ] ; uint8_T mr_nonContSig12 [ 8 ] ;
uint8_T mr_nonContSig13 [ 8 ] ; uint8_T mr_nonContSig14 [ 8 ] ; uint8_T
mr_nonContSig15 [ 8 ] ; uint8_T mr_nonContSig16 [ 8 ] ; uint8_T
mr_nonContSig17 [ 8 ] ; uint8_T mr_nonContSig18 [ 8 ] ; uint8_T
mr_nonContSig19 [ 8 ] ; real_T mr_nonContSig20 [ 3 ] ; real_T mr_nonContSig21
[ 3 ] ; real_T mr_nonContSig22 [ 3 ] ; real_T mr_nonContSig23 [ 3 ] ; real_T
mr_nonContSig24 [ 3 ] ; real_T mr_nonContSig25 [ 3 ] ; real_T mr_nonContSig26
[ 3 ] ; real_T mr_nonContSig27 [ 3 ] ; } NonContDerivMemory ;
ssNonContDerivSigInfo nonContDerivSignal [ 28 ] ; const rtTimingBridge *
timingBridge ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ;
rtwCAPI_ModelMappingInfo * childMMI [ 7 ] ; sysRanDType * systemRan [ 2 ] ;
int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T mdlref_GlobalTID [ 3 ]
; time_T tStart ; } Timing ; } ; typedef struct { lcs4qsfhas rtb ; pzfrokszxg
rtdw ; d3tk5bm4nu rtm ; } et34rxtuyzb ; extern real_T rtP_C_D [ 19 ] ; extern
real_T rtP_C_L [ 19 ] ; extern real_T rtP_actuatorDelay_s ; extern real_T
rtP_airDensityAtStd_kgpm3 ; extern real_T rtP_aircraftInitialBodyRates_radps
[ 3 ] ; extern real_T rtP_aircraftInitialEuler_rad [ 3 ] ; extern real_T
rtP_aircraftInitialPosInNED_m [ 3 ] ; extern real_T
rtP_aircraftInitialVelInBody_mps [ 3 ] ; extern real_T rtP_alphaBkpts_rad [
19 ] ; extern real_T rtP_rpm2radps ; extern void axfdkowicg ( real_T *
olq3kllmwd , real_T ekyylkzoy4 [ 3 ] , real_T k23tmtsbkt [ 9 ] , SimStruct *
_mdlRefSfcnS , ssNonContDerivSigFeedingOutports * * mr_nonContOutputArray ,
int_T mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , d3tk5bm4nu *
const kq2a4ledd0 , lcs4qsfhas * localB , pzfrokszxg * localDW , plypikssk0 *
localX , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo *
rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T
rt_CSTATEIdx ) ; extern void mr_hexarotor_MdlInfoRegFcn ( SimStruct *
mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray *
mr_hexarotor_GetDWork ( const et34rxtuyzb * mdlrefDW ) ; extern void
mr_hexarotor_SetDWork ( et34rxtuyzb * mdlrefDW , const mxArray * ssDW ) ;
extern void mr_hexarotor_RegisterSimStateChecksum ( SimStruct * S ) ; extern
mxArray * mr_hexarotor_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * hexarotor_GetCAPIStaticMap ( void ) ; extern
void d4tvbollve ( d3tk5bm4nu * const kq2a4ledd0 , pzfrokszxg * localDW ) ;
extern void bc02h231hx ( real_T * olq3kllmwd , real_T ekyylkzoy4 [ 3 ] ,
real_T k23tmtsbkt [ 9 ] , lcs4qsfhas * localB , pzfrokszxg * localDW ,
plypikssk0 * localX ) ; extern void kp54z2ru0m ( pzfrokszxg * localDW ,
plypikssk0 * localX ) ; extern void lzgk1uwtzq ( d3tk5bm4nu * const
kq2a4ledd0 , pzfrokszxg * localDW ) ; extern void di1idyujsf ( real_T *
olq3kllmwd , real_T ekyylkzoy4 [ 3 ] , real_T k23tmtsbkt [ 9 ] , lcs4qsfhas *
localB , pzfrokszxg * localDW ) ; extern void btnkmpv1va ( real_T krte1h5lak
[ 3 ] , real_T ihyfxrslag [ 3 ] , pzfrokszxg * localDW , plypikssk0 * localX
, lpahjwu50g * localXdot ) ; extern void o22h4v45uk ( d3tk5bm4nu * const
kq2a4ledd0 , real_T n52eny10fe [ 3 ] , real_T * hyo0bibwxu , real_T *
e3rcdz0mgg , real_T fqodejaqf4 [ 3 ] , real_T bkuw2qhsyc [ 3 ] , lcs4qsfhas *
localB , pzfrokszxg * localDW ) ; extern void hexarotor ( d3tk5bm4nu * const
kq2a4ledd0 , const real_T * hwiqmm33rj , const real_T * f34kkfu1o5 , const
real_T * ez35gbzsix , const real_T * krief5lekd , const real_T * cj3ef4dnmb ,
const real_T * b1bdhzmxfy , const real_T * ahlbiugxbf , const real_T *
nj1dkkyfcv , const real_T * nrawcjfu4x , const real_T * iefo1sgzli , const
real_T kieymxhb3y [ 3 ] , const real_T * b3es0qr1sh , const boolean_T *
dijt2owhra , const boolean_T * ldf1mqn2uu , const boolean_T * ffxuppcxl2 ,
const boolean_T * b4fiyshdab , const boolean_T * i1iuhb0nmb , const boolean_T
* jn03ifh1sv , boolean_T * j3qp52shss , real_T kohh1xkgtq [ 3 ] , real_T
apz3rurava [ 3 ] , real_T p1yg1cwxqj [ 3 ] , real_T kpa0rcxbqq [ 3 ] , real_T
afrovcmrof [ 3 ] , real_T krte1h5lak [ 3 ] , real_T ihyfxrslag [ 3 ] , real_T
nirqvnlytv [ 9 ] , real_T bgzuapfjlf [ 3 ] , real_T * olq3kllmwd , real_T
k23tmtsbkt [ 9 ] , real_T dfcpssh5km [ 3 ] , real_T ownkt052ik [ 3 ] , real_T
depu5tfso5 [ 3 ] , real_T gzfgudeazz [ 3 ] , real_T oskskuuo3a [ 3 ] , real_T
alh5ewkkfp [ 3 ] , real_T am5wj0dker [ 3 ] , real_T fahx4l521d [ 3 ] , real_T
mkjccnygsx [ 3 ] , real_T ianflf2y11 [ 3 ] , real_T n52eny10fe [ 3 ] , real_T
* hyo0bibwxu , real_T * e3rcdz0mgg , real_T * awebqacifm , real_T *
lhloeaf2ab , real_T * kstlpugno1 , real_T * awvjppmso0 , real_T * h33n4qaexq
, real_T * onuarw4y4v , real_T cj4wiczcob [ 6 ] , real_T fqodejaqf4 [ 3 ] ,
real_T bkuw2qhsyc [ 3 ] , lcs4qsfhas * localB , pzfrokszxg * localDW ,
plypikssk0 * localX ) ; extern void hexarotorTID2 ( real_T * olq3kllmwd ,
real_T ekyylkzoy4 [ 3 ] , real_T k23tmtsbkt [ 9 ] , lcs4qsfhas * localB ,
pzfrokszxg * localDW ) ; extern void o1lq3fucf2 ( pzfrokszxg * localDW ,
d3tk5bm4nu * const kq2a4ledd0 ) ;
#endif
