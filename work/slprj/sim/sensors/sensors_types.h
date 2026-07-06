#ifndef sensors_types_h_
#define sensors_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_BaroSensorBus_
#define DEFINED_TYPEDEF_FOR_BaroSensorBus_
typedef struct { real_T device_id ; real_T pressure_pa ; real_T altitude_m ;
real_T temperature_degC ; } BaroSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_DiffPressureSensorBus_
#define DEFINED_TYPEDEF_FOR_DiffPressureSensorBus_
typedef struct { real_T device_id ; real_T differential_pressure_pa ; real_T
temperature_degC ; } DiffPressureSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_ADCSensorBus_
#define DEFINED_TYPEDEF_FOR_ADCSensorBus_
typedef struct { BaroSensorBus BaroSensorBus ; DiffPressureSensorBus
DiffPressureSensorBus ; } ADCSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_AccelSensorBus_
#define DEFINED_TYPEDEF_FOR_AccelSensorBus_
typedef struct { real_T device_id ; real_T x_mps2 ; real_T y_mps2 ; real_T
z_mps2 ; real_T temperature_degC ; } AccelSensorBus ;
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
#ifndef DEFINED_TYPEDEF_FOR_GPSSensorBus_
#define DEFINED_TYPEDEF_FOR_GPSSensorBus_
typedef struct { real_T device_id ; real_T lat_deg ; real_T lon_deg ; real_T
alt_m ; real_T eph_m ; real_T epv_m ; real_T vel_mps ; real_T vel_n_mps ;
real_T vel_e_mps ; real_T vel_d_mps ; real_T cog_deg ; real_T
satellites_used_nd ; } GPSSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_GyroSensorBus_
#define DEFINED_TYPEDEF_FOR_GyroSensorBus_
typedef struct { real_T device_id ; real_T x_radps ; real_T y_radps ; real_T
z_radps ; real_T temperature_degC ; } GyroSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_MagSensorBus_
#define DEFINED_TYPEDEF_FOR_MagSensorBus_
typedef struct { real_T device_id ; real_T x_Gauss ; real_T y_Gauss ; real_T
z_Gauss ; real_T temperature_degC ; } MagSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_INSSensorBus_
#define DEFINED_TYPEDEF_FOR_INSSensorBus_
typedef struct { GyroSensorBus GyroSensorBus ; MagSensorBus MagSensorBus ;
AccelSensorBus AccelSensorBus ; } INSSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_MassPropertiesBus_
#define DEFINED_TYPEDEF_FOR_MassPropertiesBus_
typedef struct { real_T aircraftMass_kg ; real_T aircraftCg_m [ 3 ] ; real_T
aircraftInertiaInBody_kgm2 [ 9 ] ; real_T fuelLevelFraction ; }
MassPropertiesBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_SensorsBus_
#define DEFINED_TYPEDEF_FOR_SensorsBus_
typedef struct { GPSSensorBus GPSSensorBus ; INSSensorBus INSSensorBus ;
ADCSensorBus ADCSensorBus ; } SensorsBus ;
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
#ifndef DEFINED_TYPEDEF_FOR_struct_IVpqA0yyKk8nRugFEHRJDE_
#define DEFINED_TYPEDEF_FOR_struct_IVpqA0yyKk8nRugFEHRJDE_
typedef struct { real_T noisePower_gauss [ 3 ] ; }
struct_IVpqA0yyKk8nRugFEHRJDE ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_MFjz4vOn8SNJ0JyHHswFsH_
#define DEFINED_TYPEDEF_FOR_struct_MFjz4vOn8SNJ0JyHHswFsH_
typedef struct { real_T naturalFrequency_radps ; real_T dampingRatio_nd ;
real_T scaleFactCrossCoupling_nd [ 9 ] ; real_T measurementBias_radps [ 3 ] ;
real_T gSensitiveBias_radps [ 3 ] ; real_T lowerUpperLimits_radps [ 6 ] ;
real_T noisePower_radps [ 3 ] ; } struct_MFjz4vOn8SNJ0JyHHswFsH ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_OO3zWRFBjujepItMqiV82G_
#define DEFINED_TYPEDEF_FOR_struct_OO3zWRFBjujepItMqiV82G_
typedef struct { real_T naturalFrequency_radps ; real_T dampingRatio_nd ;
real_T scaleFactCrossCoupling_nd [ 9 ] ; real_T measurementBias_mps2 [ 3 ] ;
real_T lowerUpperLimits_mps2 [ 6 ] ; real_T noisePower_mps2 [ 3 ] ; }
struct_OO3zWRFBjujepItMqiV82G ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_Phs4FavQbVrPK50520HBKB_
#define DEFINED_TYPEDEF_FOR_struct_Phs4FavQbVrPK50520HBKB_
typedef struct { real_T noisePower_degC ; } struct_Phs4FavQbVrPK50520HBKB ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_V9nA33gu1ePUpjmEfnf1vG_
#define DEFINED_TYPEDEF_FOR_struct_V9nA33gu1ePUpjmEfnf1vG_
typedef struct { real_T vertPositionAccuracy_m ; real_T
horzPositionAccuracy_m ; real_T velocityAccuracy_mps ; real_T decayFactor_nd
; } struct_V9nA33gu1ePUpjmEfnf1vG ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_rs1DZdtWDGxlEyRlO4xwxF_
#define DEFINED_TYPEDEF_FOR_struct_rs1DZdtWDGxlEyRlO4xwxF_
typedef struct { real_T minThrottle ; } struct_rs1DZdtWDGxlEyRlO4xwxF ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_xSxKrgCnQGn980yRf6NLJD_
#define DEFINED_TYPEDEF_FOR_struct_xSxKrgCnQGn980yRf6NLJD_
typedef struct { real_T noisePower_Pa ; } struct_xSxKrgCnQGn980yRf6NLJD ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_7BigaldNWGj33rb8sElmtH_
#define DEFINED_TYPEDEF_FOR_struct_7BigaldNWGj33rb8sElmtH_
typedef struct { struct_V9nA33gu1ePUpjmEfnf1vG gps ;
struct_OO3zWRFBjujepItMqiV82G accel ; struct_MFjz4vOn8SNJ0JyHHswFsH gyro ;
struct_IVpqA0yyKk8nRugFEHRJDE mag ; struct_xSxKrgCnQGn980yRf6NLJD baro ;
struct_Phs4FavQbVrPK50520HBKB thermo ; struct_rs1DZdtWDGxlEyRlO4xwxF engine ;
} struct_7BigaldNWGj33rb8sElmtH ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_gcu1l27Ac4krknw6L9ArAE_
#define DEFINED_TYPEDEF_FOR_struct_gcu1l27Ac4krknw6L9ArAE_
typedef struct { real_T lat_deg ; real_T lon_deg ; real_T alt_m ; real_T
yawAngle_rad ; } struct_gcu1l27Ac4krknw6L9ArAE ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 50
#endif
#ifndef SS_INT64
#define SS_INT64 51
#endif
typedef struct irkooafj3n fcgqozqvuz ;
#endif
