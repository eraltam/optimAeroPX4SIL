#ifndef environment_cap_host_h__
#define environment_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
#include "Air_capi_host.h"
#include "Earth_capi_host.h"
#include "LocalTerrain_capi_host.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMappingInfo *
childMMI [ 3 ] ; Air_host_DataMapInfo_T child0 ; Earth_host_DataMapInfo_T
child1 ; LocalTerrain_host_DataMapInfo_T child2 ; }
environment_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void environment_host_InitializeDataMapInfo ( environment_host_DataMapInfo_T
* dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
