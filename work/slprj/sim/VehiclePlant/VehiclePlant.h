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
#include "sfrtif/sim_shared_utils.h"
#include "sfrtif/sfc_sdi.h"
#endif
#include "VehiclePlant_types.h"
#include <stddef.h>
#include <string.h>
#include "rtGetNaN.h"
#include "model_reference_types.h"
#include "rtw_modelmap.h"
extern struct_7BigaldNWGj33rb8sElmtH rtP_parameters ; extern real_T
rtP_INS_VARIANT ; extern real_T rtP_actuatorDelay_s ; extern real_T
rtP_gustAmplitude_mps [ 3 ] ; extern real_T rtP_gustDuration_sec ; extern
real_T rtP_gustLength_m [ 3 ] ; extern real_T rtP_gustStartTime_sec ; extern
real_T rtP_plantSampleTime_s ; extern real_T rtP_referenceAltitude_m ; extern
real_T rtP_referenceDecimalYear_years ; extern real_T
rtP_referenceLatitude_deg ; extern real_T rtP_referenceLongitude_deg ; extern
real_T rtP_terrainHeightNED_m ; extern real_T rtP_turbulenceEndTime_sec ;
extern real_T rtP_turbulenceMinAirspeed_mps ; extern real_T
rtP_turbulenceStartTime_sec ; extern real_T rtP_turbulenceWingspan_m ; extern
real_T rtP_xWindInNED_mps ; extern real_T rtP_yWindInNED_mps ; extern real_T
rtP_zWindInNED_mps ; extern boolean_T rtP_gustEnabled ; extern boolean_T
rtP_turbulenceEnabled ; extern void gyzb3lqody ( void ) ; extern void
cjxjkcm4ra ( real_T n14ud1a3ao [ 3 ] , real_T eww1d44iw1 [ 3 ] , real_T *
eagntfiowf , real_T * localX_ ) ; extern void dapgvgjvnf ( real_T * localX_ )
; extern void dgmiwxg00f ( void ) ; extern void hqrg4vgws5 ( real_T *
eagntfiowf ) ; extern void m5u2c0ldal ( real_T * localX_ , real_T *
localXdot_ ) ; extern void iv0fdu4uqg ( void ) ; extern void bdatrumzy4 ( const
real_T * ivajpnivog , const real_T * o5aggu03es , const real_T * fo3tsly54o ,
const real_T * lywo5ttb4s , const real_T * dvvgrn0ejk , boolean_T *
hw4ud53t4i , real_T lpknqxvuts [ 3 ] , real_T icurnhfbrt [ 3 ] , real_T
ex10gglxu0 [ 3 ] , real_T eohfsxacwo [ 3 ] , real_T bbs1guuyy5 [ 3 ] , real_T
auljbjoni3 [ 3 ] , real_T cpvuhtt0lv [ 3 ] , real_T blgyf1f2n2 [ 9 ] , real_T
eivnrdlxkh [ 3 ] , real_T h3ynyageke [ 3 ] , real_T * iess5nvcbh , real_T *
jn1w0eyx3o ) ; extern void VehiclePlant ( const boolean_T * lozvk3ss52 ,
const boolean_T * idqvu1xqjj , const boolean_T * duap5wwx55 , const boolean_T
* gm3kb504fe , const real_T * ivajpnivog , const real_T * o5aggu03es , const
real_T * fo3tsly54o , const real_T * lywo5ttb4s , const real_T * dvvgrn0ejk ,
real_T * mstyey3rtt , real_T * hufjamtm55 , real_T * ls2onjolga , real_T *
f40m32jcmk , real_T * nip4mqz4ij , real_T * pa0fdhsz0p , real_T * ephwh0y3f1
, real_T * iodotc34bs , real_T * n2ngeds1qk , real_T * gabaplfntu , real_T *
kbl1kmk11v , real_T * nr5k3so5eg , real_T * h0mjlv2m5d , real_T * d3bmyrhx1l
, real_T * nv43byhxzc , real_T * ompa0khus5 , real_T * cggbfg0v5w , real_T *
eaj5j4e2bg , real_T * clihtfkqdd , real_T * b3y0ukvlja , real_T * iwlk0s13aq
, real_T * hssvnv0gv3 , real_T * b3bh2ei22t , real_T * dmdoff23wk , real_T *
mpkhlwbcwa , real_T * a32i3ri33a , real_T * futrjniu4o , real_T * aubh0rmgix
, real_T * h0onumuhoc , real_T * o4vxxrplyr , real_T * nlpogoaprs , real_T *
dsipu5kvoh , real_T * k4emq5vbc0 , real_T * iqt5hdafs0 , boolean_T *
hw4ud53t4i , real_T lpknqxvuts [ 3 ] , real_T icurnhfbrt [ 3 ] , real_T
ex10gglxu0 [ 3 ] , real_T eohfsxacwo [ 3 ] , real_T bbs1guuyy5 [ 3 ] , real_T
auljbjoni3 [ 3 ] , real_T cpvuhtt0lv [ 3 ] , real_T blgyf1f2n2 [ 9 ] , real_T
eivnrdlxkh [ 3 ] , real_T h3ynyageke [ 3 ] , real_T * iess5nvcbh , real_T *
jn1w0eyx3o , real_T * fukgj2aqxk , real_T f3cdcyrdrz [ 3 ] , real_T
m0k1dvx21z [ 3 ] , real_T * l4zqz5v11i , real_T * emuwauzlx3 , real_T *
b2zx2dxzlt , real_T ga5wnjqlj4 [ 3 ] , real_T * ept5hne5f0 , real_T
n14ud1a3ao [ 3 ] , real_T eww1d44iw1 [ 3 ] , real_T * localX_ , boolean_T *
localXdis_ ) ; extern void VehiclePlantTID3 ( real_T * ecxaknsqkl , real_T
lt4aghk1iw [ 3 ] , real_T ftw4ivecrm [ 9 ] , real_T * egffeflc1h , real_T
o3msdorc3n [ 3 ] , real_T jccta0jy2p [ 3 ] , real_T jmtf3pgkap [ 3 ] , real_T
pkjc2kzoqd [ 3 ] , real_T gyobvb0xcz [ 3 ] , real_T klpcmcz2nu [ 3 ] , real_T
bmyubdgepx [ 3 ] , real_T fztlcnjl1j [ 3 ] , real_T o4p1ny21bs [ 3 ] , real_T
jelu4e3hs2 [ 3 ] , real_T ifb4fslmqj [ 3 ] , real_T * eagntfiowf ) ; extern
void omtgknbblj ( void ) ; extern void dwjsblzuar ( real_T * fukgj2aqxk ,
real_T f3cdcyrdrz [ 3 ] , real_T m0k1dvx21z [ 3 ] , real_T ifb4fslmqj [ 3 ] ,
real_T * eagntfiowf , SimStruct * _mdlRefSfcnS ,
ssNonContDerivSigFeedingOutports * * mr_nonContOutputArray , int_T
mdlref_TID0 , int_T mdlref_TID1 , int_T mdlref_TID2 , int_T mdlref_TID3 ,
real_T * localX_ , void * sysRanPtr , int_T contextTid ,
rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T
rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void
mr_VehiclePlant_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName
, int_T * retVal ) ; extern mxArray * mr_VehiclePlant_GetDWork ( ) ; extern
void mr_VehiclePlant_SetDWork ( const mxArray * ssDW ) ; extern void
mr_VehiclePlant_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray *
mr_VehiclePlant_GetSimStateDisallowedBlocks ( ) ; extern const
rtwCAPI_ModelMappingStaticInfo * VehiclePlant_GetCAPIStaticMap ( void ) ;
void mr_VehiclePlant_CreateInitRestoreData ( ) ; void
mr_VehiclePlant_CopyFromInitRestoreData ( ) ; void
mr_VehiclePlant_DestroyInitRestoreData ( ) ;
#endif
