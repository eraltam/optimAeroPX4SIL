#ifndef adc_h_
#define adc_h_
#ifndef adc_COMMON_INCLUDES_
#define adc_COMMON_INCLUDES_
#include "sl_AsyncioQueue/AsyncioQueueCAPI.h"
#include "rtwtypes.h"
#include "slsv_diagnostic_codegen_c_api.h"
#include "simstruc.h"
#include "fixedpoint.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif
#include "adc_types.h"
#include <stddef.h>
#include "rtw_modelmap.h"
#include "rt_nonfinite.h"
#include <string.h>
typedef struct { real_T ic0gej1mlt ; } na4objl5ck ; typedef struct { real_T
g1tvakypuk ; real_T ipou4ebrbn ; real_T gzi000g5ea [ 8 ] ; real_T jmskdtta42
[ 8 ] ; struct { void * AQHandles ; } cu2ga2vbol ; struct { void * AQHandles
; } jowvtb3a3y ; struct { void * AQHandles ; } hmcbl2e3pg ; struct { void *
AQHandles ; } pjx0adiolj ; struct { void * AQHandles ; } fl3hrvzgti ; struct
{ void * AQHandles ; } nebffmdez1 ; uint32_T nrwdifeed2 ; uint32_T iitw1mflrk
; } fu5g1quldg ; struct bhhzm0uwhmt_ { real_T P_1 ; real_T P_2 ; real_T P_3 ;
real_T P_4 ; real_T P_5 ; real_T P_6 ; real_T P_7 ; real_T P_8 ; real_T P_9 ;
real_T P_10 ; real_T P_11 ; real_T P_12 ; } ; struct lenyh4fmes { struct
SimStruct_tag * _mdlRefSfcnS ; struct { rtwCAPI_ModelMappingInfo mmi ;
rtwCAPI_ModelMapLoggingInstanceInfo mmiLogInstanceInfo ; sysRanDType *
systemRan [ 2 ] ; int_T systemTid [ 2 ] ; } DataMapInfo ; struct { int_T
mdlref_GlobalTID [ 2 ] ; } Timing ; } ; typedef struct { na4objl5ck rtb ;
fu5g1quldg rtdw ; ejys33derb rtm ; } ajsi3uhw15h ; extern
struct_7BigaldNWGj33rb8sElmtH rtP_parameters ; extern void lcjgg50y0j ( SimStruct * _mdlRefSfcnS , int_T mdlref_TID0 , int_T mdlref_TID1 , ejys33derb * const hsrfiqrkgc , na4objl5ck * localB , fu5g1quldg * localDW , void * sysRanPtr , int_T contextTid , rtwCAPI_ModelMappingInfo * rt_ParentMMI , const char_T * rt_ChildPath , int_T rt_ChildMMIIdx , int_T rt_CSTATEIdx ) ; extern void mr_adc_MdlInfoRegFcn ( SimStruct * mdlRefSfcnS , char_T * modelName , int_T * retVal ) ; extern mxArray * mr_adc_GetDWork ( const ajsi3uhw15h * mdlrefDW ) ; extern void mr_adc_SetDWork ( ajsi3uhw15h * mdlrefDW , const mxArray * ssDW ) ; extern void mr_adc_RegisterSimStateChecksum ( SimStruct * S ) ; extern mxArray * mr_adc_GetSimStateDisallowedBlocks ( ) ; extern const rtwCAPI_ModelMappingStaticInfo * adc_GetCAPIStaticMap ( void ) ; extern void datkbpvk0h ( ejys33derb * const hsrfiqrkgc , fu5g1quldg * localDW ) ; extern void d5o2ouci45 ( real_T * ntwzjk5dha , fu5g1quldg * localDW ) ; extern void jzvflsluq2 ( real_T * ntwzjk5dha , fu5g1quldg * localDW ) ; extern void ck42tyjtk5 ( ejys33derb * const hsrfiqrkgc , fu5g1quldg * localDW ) ; extern void dsarn04rkd ( real_T * ntwzjk5dha , fu5g1quldg * localDW ) ; extern void ltbspptju1 ( fu5g1quldg * localDW ) ; extern void adc ( ejys33derb * const hsrfiqrkgc , const real_T * lr4h33hcrc , const real_T * mb2lzjozwu , const real_T * j1jt5wk3rd , const real_T gmmyhrgz1i [ 3 ] , real_T * l1m21ig2xs , real_T * iiuq1qatl5 , real_T * jntj3emcfk , real_T * kyb45tdkqn , real_T * atrlxr2eks , na4objl5ck * localB , fu5g1quldg * localDW ) ; extern void adcTID1 ( real_T * ntwzjk5dha , real_T * nz1wkgxagd ) ; extern void gokq5jjs53 ( ejys33derb * const hsrfiqrkgc ) ;
#endif
