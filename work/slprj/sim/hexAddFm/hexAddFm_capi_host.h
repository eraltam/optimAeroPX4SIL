#ifndef hexAddFm_cap_host_h__
#define hexAddFm_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; } hexAddFm_host_DataMapInfo_T
;
#ifdef __cplusplus
extern "C" {
#endif
void hexAddFm_host_InitializeDataMapInfo ( hexAddFm_host_DataMapInfo_T *
dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
