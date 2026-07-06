#ifndef inertialDynamics_types_h_
#define inertialDynamics_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_ComponentForcesMomentsBus_
#define DEFINED_TYPEDEF_FOR_ComponentForcesMomentsBus_
typedef struct { real_T forcesInBody_N [ 3 ] ; real_T momentsInBody_Nm [ 3 ]
; } ComponentForcesMomentsBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_AircraftForcesMomentsBus_
#define DEFINED_TYPEDEF_FOR_AircraftForcesMomentsBus_
typedef struct { real_T forcesInBody_N [ 3 ] ; real_T momentsInBody_Nm [ 3 ]
; ComponentForcesMomentsBus AeroForcesMomentsBus ; ComponentForcesMomentsBus
EngineForcesMomentsBus ; ComponentForcesMomentsBus GroundForcesMomentsBus ;
ComponentForcesMomentsBus WeightForcesMomentsBus ; } AircraftForcesMomentsBus
;
#endif
#ifndef DEFINED_TYPEDEF_FOR_BodyStateBus_
#define DEFINED_TYPEDEF_FOR_BodyStateBus_
typedef struct { real_T aircraftVelInNED_mps [ 3 ] ; real_T
aircraftPosInNED_m [ 3 ] ; real_T aircraftEulerAngles_rad [ 3 ] ; real_T
aircraftVelInBody_mps [ 3 ] ; real_T aircraftAngVelInBody_radps [ 3 ] ;
real_T aircraftAngAccelInBody_radps2 [ 3 ] ; real_T aircraftAccelInBody_mps2
[ 3 ] ; real_T DCM_be [ 9 ] ; real_T aircraftAccelbe_mps2 [ 3 ] ; }
BodyStateBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_MassPropertiesBus_
#define DEFINED_TYPEDEF_FOR_MassPropertiesBus_
typedef struct { real_T aircraftMass_kg ; real_T aircraftCg_m [ 3 ] ; real_T
aircraftInertiaInBody_kgm2 [ 9 ] ; real_T fuelLevelFraction ; }
MassPropertiesBus ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 21
#endif
#ifndef SS_INT64
#define SS_INT64 22
#endif
typedef struct gbcsa11b4gr_ gbcsa11b4gr ; typedef struct eiiinonju3
knpvazrmc0 ;
#endif
