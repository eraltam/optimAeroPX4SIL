#ifndef hexarotor_cap_host_h__
#define hexarotor_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"
#include "hexActuator_capi_host.h"
#include "hexAddFm_capi_host.h"
#include "hexAero_capi_host.h"
#include "cgMassInertia_capi_host.h"
#include "inertialDynamics_capi_host.h"
#include "gravity_capi_host.h"
#include "hexGroundContact_capi_host.h"
typedef struct { rtwCAPI_ModelMappingInfo mmi ; rtwCAPI_ModelMappingInfo *
childMMI [ 7 ] ; hexActuator_host_DataMapInfo_T child0 ;
hexAddFm_host_DataMapInfo_T child1 ; hexAero_host_DataMapInfo_T child2 ;
cgMassInertia_host_DataMapInfo_T child3 ; inertialDynamics_host_DataMapInfo_T
child4 ; gravity_host_DataMapInfo_T child5 ;
hexGroundContact_host_DataMapInfo_T child6 ; } hexarotor_host_DataMapInfo_T ;
#ifdef __cplusplus
extern "C" {
#endif
void hexarotor_host_InitializeDataMapInfo ( hexarotor_host_DataMapInfo_T *
dataMap , const char * path ) ;
#ifdef __cplusplus
}
#endif
#endif
#endif
