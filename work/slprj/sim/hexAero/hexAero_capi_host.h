#ifndef hexAero_cap_host_h__
#define hexAero_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
#include "hexAeroCoefficientsModel_capi_host.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMappingInfo *
childMMI [ 1 ] ; hexAeroCoefficientsModel_host_DataMapInfo_T child0 ; }
hexAero_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void hexAero_host_InitializeDataMapInfo ( hexAero_host_DataMapInfo_T *
dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
