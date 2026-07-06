#ifndef hexarotor_types_h_
#define hexarotor_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_ComponentForcesMomentsBus_
#define DEFINED_TYPEDEF_FOR_ComponentForcesMomentsBus_
typedef struct { real_T forcesInBody_N [ 3 ] ; real_T momentsInBody_Nm [ 3 ]
; } ComponentForcesMomentsBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_PropulsionBus_
#define DEFINED_TYPEDEF_FOR_PropulsionBus_
typedef struct { real_T rotorAngVel_radps [ 6 ] ; ComponentForcesMomentsBus
EngineForcesMomentsBus ; } PropulsionBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ServosHexBus_
#define DEFINED_TYPEDEF_FOR_ServosHexBus_
typedef struct { real_T rotor1_rpm ; real_T rotor2_rpm ; real_T rotor3_rpm ;
real_T rotor4_rpm ; real_T rotor5_rpm ; real_T rotor6_rpm ; } ServosHexBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ServosBus_
#define DEFINED_TYPEDEF_FOR_ServosBus_
typedef struct { ServosHexBus ServosHexBus ; } ServosBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ActuatorBus_
#define DEFINED_TYPEDEF_FOR_ActuatorBus_
typedef struct { ServosBus ServosBus ; PropulsionBus PropulsionBus ; }
ActuatorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_EngineCommandBus_
#define DEFINED_TYPEDEF_FOR_EngineCommandBus_
typedef struct { real_T cmdThrottle_unit ; } EngineCommandBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_MotorCommandHexBus_
#define DEFINED_TYPEDEF_FOR_MotorCommandHexBus_
typedef struct { real_T cmdRotor1_nd ; real_T cmdRotor2_nd ; real_T
cmdRotor3_nd ; real_T cmdRotor4_nd ; real_T cmdRotor5_nd ; real_T
cmdRotor6_nd ; } MotorCommandHexBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ServosCommandBus_
#define DEFINED_TYPEDEF_FOR_ServosCommandBus_
typedef struct { MotorCommandHexBus MotorCommandHexBus ; } ServosCommandBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ActuatorCommandBus_
#define DEFINED_TYPEDEF_FOR_ActuatorCommandBus_
typedef struct { real_T controllerArmed ; ServosCommandBus ServosCommandBus ;
EngineCommandBus EngineCommandBus ; } ActuatorCommandBus ;
#endif
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
#ifndef DEFINED_TYPEDEF_FOR_FailureBus_
#define DEFINED_TYPEDEF_FOR_FailureBus_
typedef struct { boolean_T rotorFailure1_nd ; boolean_T rotorFailure2_nd ;
boolean_T rotorFailure3_nd ; boolean_T rotorFailure4_nd ; boolean_T
rotorFailure5_nd ; boolean_T rotorFailure6_nd ; uint8_T sl_padding0 [ 2 ] ; }
FailureBus ;
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
#define SS_UINT64 44
#endif
#ifndef SS_INT64
#define SS_INT64 45
#endif
typedef struct cw0e20kstic_ cw0e20kstic ; typedef struct ctkchtprgd
d3tk5bm4nu ;
#endif
