#ifndef failureInputReadHex_cap_host_h__
#define failureInputReadHex_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; }
failureInputReadHex_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void failureInputReadHex_host_InitializeDataMapInfo ( failureInputReadHex_host_DataMapInfo_T * dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
