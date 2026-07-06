#ifndef ins_anello_cap_host_h__
#define ins_anello_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; }
ins_anello_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void ins_anello_host_InitializeDataMapInfo ( ins_anello_host_DataMapInfo_T *
dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
