#ifndef VehiclePlant_cap_host_h__
#define VehiclePlant_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
#include "environment_capi_host.h"
#include "hexarotor_capi_host.h"
#include "sensors_capi_host.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMappingInfo *
childMMI [ 3 ] ; environment_host_DataMapInfo_T child0 ;
hexarotor_host_DataMapInfo_T child1 ; sensors_host_DataMapInfo_T child2 ; }
VehiclePlant_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void VehiclePlant_host_InitializeDataMapInfo ( VehiclePlant_host_DataMapInfo_T
* dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
