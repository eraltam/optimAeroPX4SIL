#ifndef Earth_types_h_
#define Earth_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_BodyStateBus_
#define DEFINED_TYPEDEF_FOR_BodyStateBus_
typedef struct { real_T aircraftVelInNED_mps [ 3 ] ; real_T
aircraftPosInNED_m [ 3 ] ; real_T aircraftEulerAngles_rad [ 3 ] ; real_T
aircraftVelInBody_mps [ 3 ] ; real_T aircraftAngVelInBody_radps [ 3 ] ;
real_T aircraftAngAccelInBody_radps2 [ 3 ] ; real_T aircraftAccelInBody_mps2
[ 3 ] ; real_T DCM_be [ 9 ] ; real_T aircraftAccelbe_mps2 [ 3 ] ; }
BodyStateBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_EarthEnvironmentBus_
#define DEFINED_TYPEDEF_FOR_EarthEnvironmentBus_
typedef struct { real_T gravityScalar_mps2 ; real_T
worldMagneticFieldInNED_nT [ 3 ] ; real_T mulh [ 3 ] ; } EarthEnvironmentBus
;
#endif
#ifndef SS_UINT64
#define SS_UINT64 19
#endif
#ifndef SS_INT64
#define SS_INT64 20
#endif
typedef struct ffdegmia4yh_ ffdegmia4yh ; typedef struct kofjb25fch
bb1vfhetht ;
#endif
