#ifndef LocalTerrain_cap_host_h__
#define LocalTerrain_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; }
LocalTerrain_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void LocalTerrain_host_InitializeDataMapInfo ( LocalTerrain_host_DataMapInfo_T
* dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
