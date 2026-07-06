#ifndef hexActuator_cap_host_h__
#define hexActuator_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
#include "hexMotorModel_capi_host.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMappingInfo *
childMMI [ 1 ] ; hexMotorModel_host_DataMapInfo_T child0 ; }
hexActuator_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void hexActuator_host_InitializeDataMapInfo ( hexActuator_host_DataMapInfo_T
* dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
