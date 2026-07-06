#ifndef adc_cap_host_h__
#define adc_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; } adc_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void adc_host_InitializeDataMapInfo ( adc_host_DataMapInfo_T * dataMap ,
const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
