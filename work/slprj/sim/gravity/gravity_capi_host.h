#ifndef gravity_cap_host_h__
#define gravity_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; } gravity_host_DataMapInfo_T
;
#ifdef __cplusplus
extern "C" {
#endif
void gravity_host_InitializeDataMapInfo ( gravity_host_DataMapInfo_T *
dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
