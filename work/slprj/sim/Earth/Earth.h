#ifndef Earth_h_
#define Earth_h_
#ifndef Earth_COMMON_INCLUDES_
#define Earth_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif
#include "Earth_types.h"
#include <stddef.h>
#include "rtGetNaN.h"
#include "rtw_modelmap.h"
#include <string.h>
typedef struct { real_T pm3nrbvxxm ; real_T apafgf2k2s ; real_T f4r1cxthcq ;
real_T ghkkleqr1a ; real_T oem0g31b22 [ 13 ] ; real_T opjqv05h3l [ 13 ] ;
real_T amhr4ztdqn ; real_T aefjuppi2y ; real_T lorv0yeaou ; real_T b53ge4wnnw
; real_T i0imcq1k1s ; real_T c1bjpqvcxd ; real_T hjshxyt31g ; real_T
icckgf5jjc ; real_T br3alfc4gk ; real_T fxv0xkdhvd ; real_T ar4frea44j ;
real_T dxiexlunn5 [ 4 ] ; real_T cm4b0iph2d ; real_T oh4x12c21u ; real_T
dobqmapgcb ; real_T lgoian24sx ; real_T oyzs4kglnk [ 169 ] ; real_T
eruxz5lmof [ 169 ] ; real_T jlcyjzy53c [ 169 ] ; real_T fueem34euq [ 169 ] ;
real_T mfsxdyqgra ; real_T bot4iu1vya [ 169 ] ; real_T ioze5tizcn ; real_T
nqf4g1utdk [ 169 ] ; real_T b2urywdhsq [ 13 ] ; real_T hyyza0dd2a ; real_T
mtsx3lfryn [ 13 ] ; real_T lvhp0spzvu [ 13 ] ; boolean_T jxxfz4pktv ; }
gi01s1hcpx ; typedef struct { real_T aq4saffazm [ 2 ] ; real_T nlhu1a3bnw ;
real_T gkws4s554s [ 4 ] ; real_T mfokmdqwwn ; real_T plzcprkyqj ; real_T
ht50pmmim3 ; real_T iwkwllhrgj ; real_T o1ximcjf0n [ 169 ] ; real_T
a1lw0ye4rx [ 169 ] ; real_T kepstjkn2c [ 169 ] ; real_T o5zi5azywh [ 169 ] ;
real_T dt4rii0abk [ 13 ] ; real_T lcdnbqky3j ; real_T owtegp5jzr ; real_T
jigx51emaz ; real_T e0lcla325c ; struct { void * AQHandles ; } ppcbmxkucn ;
struct { void * AQHandles ; } iagvdmjskk ; struct { void * AQHandles ; }
gv5ma14h25 ; int8_T j3plydqzjg ; int8_T bv0fo1kf4j ; int8_T pin4nicaxa ;
int8_T pyj3gbvmey ; int8_T pnvt13ahqb ; int8_T d4yyi3btmt ; int8_T ginf43jyeg
; int8_T cy2vzgbbze ; int8_T a1wbmrcgjp ; int8_T iq40nm4sqy ; int8_T
mvvyi2x3ss ; int8_T mopryukfaa ; int8_T cwaubofgz1 ; int8_T e1sy0aue55 ;
int8_T db43rtjmwy ; int8_T an4cuvbvfz ; int8_T f0e5t0qls2 ; uint8_T
adyjlexvl0 [ 6 ] ; boolean_T djdx3qugxj ; } micoo1q4de ; struct ffdegmia4yh_
{ real_T P_2 ; real_T P_3 ; real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7
; real_T P_8 ; real_T P_9 ; real_T P_10 ; real_T P_11 ; real_T P_12 ; real_T
P_13 ; real_T P_14 ; real_T P_15 ; real_T P_16 ; real_T P_17 ; real_T P_18 ;
real_T P_19 ; real_T P_20 ; real_T P_21 [ 13 ] ; real_T P_22 ; real_T P_23 [
13 ] ; real_T P_24 [ 169 ] ; real_T P_25 ; real_T P_26 [ 13 ] ; real_T P_27 ;
real_T P_28 ; real_T P_29 ; real_T P_30 ; real_T P_31 ; real_T P_32 ; real_T
P_33 ; real_T P_34 ; real_T P_35 ; real_T P_36 [ 169 ] ; real_T P_37 [ 169 ]
; real_T P_38 [ 169 ] ; real_T P_39 [ 169 ] ; real_T P_40 [ 169 ] ; real_T
P_41 ; real_T P_42 ; real_T P_43 ; real_T P_44 [ 169 ] ; real_T P_45 [ 169 ]
; real_T P_46 [ 169 ] ; real_T P_47 [ 169 ] ; real_T P_48 [ 169 ] ; real_T
P_49 [ 169 ] ; real_T P_50 ; real_T P_51 ; real_T P_52 ; real_T P_53 ; real_T
P_54 ; real_T P_55 ; real_T P_56 ; real_T P_57 ; real_T P_58 ; real_T P_59 ;
real_T P_60 ; real_T P_61 [ 13 ] ; real_T P_62 [ 13 ] ; real_T P_63 ; real_T
P_64 [ 4 ] ; real_T P_65 ; real_T P_66 [ 4 ] ; real_T P_67 ; real_T P_68 ;
real_T P_69 ; real_T P_70 ; real_T P_71 ; real_T P_72 ; real_T P_73 ; real_T
P_74 ; real_T P_75 ; real_T P_76 [ 11 ] ; real_T P_77 [ 11 ] ; real_T P_78 ;
real_T P_79 ; real_T P_80 ; real_T P_81 [ 11 ] ; real_T P_82 [ 11 ] ; real_T
P_83 [ 13 ] ; real_T P_84 [ 13 ] ; real_T P_85 ; real_T P_86 ; real_T P_87 ;
real_T P_88 ; real_T P_89 [ 2 ] ; real_T P_90 ; real_T P_91 ; real_T P_92 ;
real_T P_93 ; real_T P_94 ; real_T P_95 ; real_T P_96 ; real_T P_97 ; real_T
P_98 ; real_T P_99 ; real_T P_100 ; real_T P_101 ; real_T P_102 ; real_T
P_103 ; real_T P_104 ; int32_T P_105 ; int32_T P_106 ; int32_T P_107 ;
int32_T P_108 ; int32_T P_109 ; int32_T P_110 ; int32_T P_111 ; int32_T P_112
; int32_T P_113 ; int32_T P_114 ; int32_T P_115 ; int32_T P_116 ; int32_T
P_117 ; int32_T P_118 ; int32_T P_119 ; int32_T P_120 ; int32_T P_121 ;
int32_T P_122 ; int32_T P_123 ; int32_T P_124 ; int32_T P_125 ; int32_T P_126
; int32_T P_127 ; int32_T P_128 ; int32_T P_129 ; int32_T P_130 ; int32_T
P_131 ; int32_T P_132 ; int32_T P_133 ; int32_T P_134 ; boolean_T P_135 ;
boolean_T P_136 ; boolean_T P_137 ; boolean_T P_138 ; } ; struct kofjb25fch {
struct SimStruct_tag * _mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; void * dataAddress [
12 ] ; int32_T * vardimsAddress [ 12 ] ; RTWLoggingFcnPtr loggingPtrs [ 12 ]
; sysRanDType * systemRan [ 27 ] ; int_T systemTid [ 27 ] ; } DataMapInfo ;
struct { int_T mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct {
gi01s1hcpx rtb ; micoo1q4de rtdw ; bb1vfhetht rtm ; } kzo2mbsde2x ; extern
real_T rtP_referenceAltitude_m ; extern real_T rtP_referenceDecimalYear_years
; extern void o1evou4ho2 ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 ,
int_T mdlref_TID1 , bb1vfhetht * const craevkrdm5 , gi01s1hcpx * localB ,
micoo1q4de * localDW , void * sysRanPtr , int_T contextTid ,
rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T
rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_Earth_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_Earth_GetDWork ( const kzo2mbsde2x * mdlrefDW ) ; extern void mr_Earth_SetDWork ( kzo2mbsde2x * mdlrefDW , const mxArray * ssDW ) ; extern void mr_Earth_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_Earth_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * Earth_GetCAPIStaticMap ( void ) ; extern void fgj5iijlyx ( bb1vfhetht * const craevkrdm5 , micoo1q4de * localDW ) ; extern void h0xamgxegy ( gi01s1hcpx * localB , micoo1q4de * localDW ) ; extern void hhmbei1r1d ( micoo1q4de * localDW ) ; extern void a0favbk2uu ( bb1vfhetht * const craevkrdm5 , micoo1q4de * localDW ) ; extern void jvoajx2hic ( gi01s1hcpx * localB , micoo1q4de * localDW ) ; extern void cnzfydxze0 ( gi01s1hcpx * localB , micoo1q4de * localDW ) ; extern void Earth ( bb1vfhetht * const craevkrdm5 , const real_T kht3jneztn [ 3 ] , real_T * cbrmrb3qj0 , real_T plhtl5ufw1 [ 3 ] , real_T fthh55kjai [ 3 ] , gi01s1hcpx * localB , micoo1q4de * localDW ) ; extern void EarthTID1 ( gi01s1hcpx * localB ) ; extern void gyaf1bhjum ( bb1vfhetht * const craevkrdm5 ) ;
#endif
