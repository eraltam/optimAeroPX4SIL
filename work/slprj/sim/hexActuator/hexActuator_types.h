#ifndef hexActuator_types_h_
#define hexActuator_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_ComponentForcesMomentsBus_
#define DEFINED_TYPEDEF_FOR_ComponentForcesMomentsBus_
typedef struct { real_T forcesInBody_N [ 3 ] ; real_T momentsInBody_Nm [ 3 ]
; } ComponentForcesMomentsBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_PropulsionBus_
#define DEFINED_TYPEDEF_FOR_PropulsionBus_
typedef struct { real_T rotorAngVel_radps [ 2 ] ; ComponentForcesMomentsBus
EngineForcesMomentsBus ; } PropulsionBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ServosHexBus_
#define DEFINED_TYPEDEF_FOR_ServosHexBus_
typedef struct { real_T rotor1_rpm ; real_T rotor2_rpm ; } ServosHexBus ;
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
typedef struct { real_T cmdRotor1_nd ; real_T cmdRotor2_nd ; }
MotorCommandHexBus ;
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
uint8_T sl_padding0 [ 6 ] ; } FailureBus ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 33
#endif
#ifndef SS_INT64
#define SS_INT64 34
#endif
typedef struct kz2wccbuitc_ kz2wccbuitc ; typedef struct kpr0vnbqqc
pwmxrvh3p0 ;
#endif
