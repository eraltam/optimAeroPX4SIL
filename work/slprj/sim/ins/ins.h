#ifndef ins_h_
#define ins_h_
#ifndef ins_COMMON_INCLUDES_
#define ins_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "ins_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { real_T l0ih0det4u ; real_T pdax3wlrgr ; real_T atfhfnmmae ;
real_T po5uamy2hu ; real_T cyfjxcttj3 [ 3 ] ; real_T pbwwh2nkvb ; real_T
ozysca1nd3 ; real_T p2gxahskvv ; real_T ekx4sxwyxb ; real_T domdzspl2s [ 3 ]
; } emykx1nmjl ; typedef struct { real_T brno0o4wuu [ 6 ] ; real_T mp3p522fzj
[ 6 ] ; real_T jyqq1qj33f [ 3 ] ; real_T gqizyc2oi4 [ 3 ] ; real_T d21nh1lcaf
[ 3 ] ; real_T basioxh5gx [ 3 ] ; real_T jzkylhyeis [ 3 ] ; struct { void *
AQHandles ; } hflzav5r0b ; uint32_T d05tkpsfc4 [ 3 ] ; uint32_T kxf2c0s2vq [
3 ] ; uint32_T b0f1jscqi2 [ 3 ] ; } nh5zucdt3e ; struct do1whjbq3ev_ { real_T
P_1 ; real_T P_2 ; real_T P_3 [ 3 ] ; real_T P_4 [ 3 ] ; real_T P_5 [ 3 ] ;
real_T P_6 [ 3 ] ; real_T P_7 [ 3 ] ; real_T P_8 [ 3 ] ; real_T P_9 ; real_T
P_10 ; real_T P_11 ; real_T P_12 ; real_T P_13 ; real_T P_14 ; real_T P_15 ;
real_T P_16 ; real_T P_17 ; real_T P_18 ; real_T P_19 [ 3 ] ; real_T P_20 [ 3
] ; real_T P_21 [ 3 ] ; real_T P_22 ; real_T P_23 ; real_T P_24 ; real_T P_25
; real_T P_26 [ 3 ] ; real_T P_27 [ 3 ] ; real_T P_28 ; real_T P_29 ; real_T
P_30 ; real_T P_31 ; real_T P_32 ; real_T P_33 ; real_T P_34 ; real_T P_35 ;
real_T P_36 ; real_T P_37 ; real_T P_38 ; real_T P_39 ; real_T P_40 ; real_T
P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 ; real_T P_45 [ 3 ] ; real_T
P_46 ; real_T P_47 ; } ; struct lvllbrn3ps { struct SimStruct_tag *
_mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; void * dataAddress [
2 ] ; int32_T * vardimsAddress [ 2 ] ; RTWLoggingFcnPtr loggingPtrs [ 2 ] ;
sysRanDType * systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ;
struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct {
emykx1nmjl rtb ; nh5zucdt3e rtdw ; ntnem3iwts rtm ; } imyuptdnp20 ; extern
struct_7BigaldNWGj33rb8sElmtH rtP_parameters ; extern void g3l4esxyac ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , ntnem3iwts * const lsn0hi3qme , emykx1nmjl * localB , nh5zucdt3e * localDW , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_ins_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_ins_GetDWork ( const imyuptdnp20 * mdlrefDW ) ; extern void mr_ins_SetDWork ( imyuptdnp20 * mdlrefDW , const mxArray * ssDW ) ; extern void mr_ins_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_ins_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * ins_GetCAPIStaticMap ( void ) ; extern void e5xa1mkrpt ( ntnem3iwts * const lsn0hi3qme , nh5zucdt3e * localDW ) ; extern void l55wdp3u0r ( real_T * ismygy0xgt , nh5zucdt3e * localDW ) ; extern void mweeq0xsoy ( real_T * ismygy0xgt , nh5zucdt3e * localDW ) ; extern void pntvwlk3k0 ( ntnem3iwts * const lsn0hi3qme , nh5zucdt3e * localDW ) ; extern void ovcoxgbkug ( real_T * ismygy0xgt ) ; extern void nm4estrdnl ( nh5zucdt3e * localDW ) ; extern void ins ( ntnem3iwts * const lsn0hi3qme , const real_T * cjylq4bqmw , const real_T pl3nafs54f [ 3 ] , const real_T i1slxuw0zu [ 3 ] , const real_T crbq2xrijr [ 3 ] , const real_T jq4qhyxtrq [ 9 ] , const real_T knw0cqdbmx [ 3 ] , real_T * ctqxy1udr1 , real_T * piyal1okyo , real_T * jagye3ecyu , real_T * a4y3xt4y5s , real_T * cl3xdhk01s , real_T * mz5b13e2we , real_T * jahldtcrmc , real_T * dsslmuoyau , real_T * bvdn3xzrhq , emykx1nmjl * localB , nh5zucdt3e * localDW ) ; extern void insTID1 ( real_T * ismygy0xgt , real_T * j2q1t3zdez , real_T * egpew3xyfw , real_T * fy41xuk3ee , real_T * ood5w1s0ty , real_T * g0ax02umg3 , emykx1nmjl * localB ) ; extern void kodwqddmp4 ( ntnem3iwts * const lsn0hi3qme ) ;
#endif
