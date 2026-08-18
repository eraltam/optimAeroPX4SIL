#ifndef sensors_cap_host_h__
#define sensors_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
#include "adc_capi_host.h"
#include "gps_capi_host.h"
#include "ins_anello_v2_capi_host.h"
#include "ins_anello_capi_host.h"
#include "ins_capi_host.h"
#include "ins_ideal_capi_host.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMappingInfo *
childMMI [ 6 ] ; adc_host_DataMapInfo_T child0 ; gps_host_DataMapInfo_T
child1 ; ins_anello_v2_host_DataMapInfo_T child2 ;
ins_anello_host_DataMapInfo_T child3 ; ins_host_DataMapInfo_T child4 ;
ins_ideal_host_DataMapInfo_T child5 ; } sensors_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void sensors_host_InitializeDataMapInfo ( sensors_host_DataMapInfo_T *
dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
