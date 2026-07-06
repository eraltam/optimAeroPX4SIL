#ifndef environment_types_h_
#define environment_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_AirDataBus_
#define DEFINED_TYPEDEF_FOR_AirDataBus_
typedef struct { real_T airspeedInBody_mps [ 3 ] ; real_T alpha_rad ; real_T
beta_rad ; } AirDataBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_AirEnvironmentBus_
#define DEFINED_TYPEDEF_FOR_AirEnvironmentBus_
typedef struct { real_T airTemperature_K ; real_T airPressure_Pa ; real_T
airDensity_kgpm3 ; real_T windSpeedInNED_mps [ 3 ] ; real_T speedOfSound_mps
; real_T windsAloftInNED_mps [ 3 ] ; real_T gustInNED_mps [ 3 ] ; real_T
turbulenceInNED_mps [ 3 ] ; } AirEnvironmentBus ;
#endif
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
#ifndef DEFINED_TYPEDEF_FOR_EarthEnvironmentBus_
#define DEFINED_TYPEDEF_FOR_EarthEnvironmentBus_
typedef struct { real_T gravityScalar_mps2 ; real_T
worldMagneticFieldInNED_nT [ 3 ] ; real_T mulh [ 3 ] ; } EarthEnvironmentBus
;
#endif
#ifndef DEFINED_TYPEDEF_FOR_TerrainEnvironmentBus_
#define DEFINED_TYPEDEF_FOR_TerrainEnvironmentBus_
typedef struct { real_T terrainHeightInNED_m ; } TerrainEnvironmentBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_EnvironmentBus_
#define DEFINED_TYPEDEF_FOR_EnvironmentBus_
typedef struct { EarthEnvironmentBus EarthEnvironment ; AirEnvironmentBus
AirEnvironment ; TerrainEnvironmentBus TerrainEnvironment ; } EnvironmentBus
;
#endif
#ifndef DEFINED_TYPEDEF_FOR_MassPropertiesBus_
#define DEFINED_TYPEDEF_FOR_MassPropertiesBus_
typedef struct { real_T aircraftMass_kg ; real_T aircraftCg_m [ 3 ] ; real_T
aircraftInertiaInBody_kgm2 [ 9 ] ; real_T fuelLevelFraction ; }
MassPropertiesBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_VehicleConfigurationBus_
#define DEFINED_TYPEDEF_FOR_VehicleConfigurationBus_
typedef struct { boolean_T isVehicleAirborne ; uint8_T sl_padding0 [ 7 ] ; }
VehicleConfigurationBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_VehicleBus_
#define DEFINED_TYPEDEF_FOR_VehicleBus_
typedef struct { VehicleConfigurationBus VehicleConfigurationBus ;
BodyStateBus BodyStatesBus ; MassPropertiesBus MassPropertiesBus ;
AircraftForcesMomentsBus AircraftForcesMomentsBus ; AirDataBus AirDataBus ; }
VehicleBus ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 31
#endif
#ifndef SS_INT64
#define SS_INT64 32
#endif
typedef struct cgjzqe2tvy mojmojnvtk ;
#endif
