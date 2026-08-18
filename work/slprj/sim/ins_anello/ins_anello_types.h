#ifndef ins_anello_types_h_
#define ins_anello_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_AccelSensorBus_
#define DEFINED_TYPEDEF_FOR_AccelSensorBus_
typedef struct { real_T device_id ; real_T x_mps2 ; real_T y_mps2 ; real_T
z_mps2 ; real_T temperature_degC ; } AccelSensorBus ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_AirEnvironmentBus_
#define DEFINED_TYPEDEF_FOR_AirEnvironmentBus_
typedef struct { real_T airTemperature_K ; real_T airPressure_Pa ; real_T
airDensity_kgpm3 ; real_T windSpeedInNED_mps [ 3 ] ; real_T speedOfSound_mps
; real_T windsAloftInNED_mps [ 3 ] ; real_T gustInNED_mps [ 3 ] ; real_T
turbulenceInNED_mps [ 3 ] ; } AirEnvironmentBus ;
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
#ifndef DEFINED_TYPEDEF_FOR_struct_8daDgjHl69LL7sHFxHAfoF_
#define DEFINED_TYPEDEF_FOR_struct_8daDgjHl69LL7sHFxHAfoF_
typedef struct { boolean_T debugEnable ; boolean_T residualThermalEnable ;
uint8_T sl_padding0 [ 6 ] ; } struct_8daDgjHl69LL7sHFxHAfoF ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_DBj3z2TsrHhthEcQUJuruF_
#define DEFINED_TYPEDEF_FOR_struct_DBj3z2TsrHhthEcQUJuruF_
typedef struct { struct_8daDgjHl69LL7sHFxHAfoF anello ; }
struct_DBj3z2TsrHhthEcQUJuruF ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_PnJy5R3HzCjafF7TLm4DbE_
#define DEFINED_TYPEDEF_FOR_struct_PnJy5R3HzCjafF7TLm4DbE_
typedef struct { real_T enable ; } struct_PnJy5R3HzCjafF7TLm4DbE ;
#endif
#ifndef DEFINED_TYPEDEF_FOR_struct_EwMtsSUDbGvLjUwS89lioH_
#define DEFINED_TYPEDEF_FOR_struct_EwMtsSUDbGvLjUwS89lioH_
typedef struct { struct_DBj3z2TsrHhthEcQUJuruF imu ;
struct_PnJy5R3HzCjafF7TLm4DbE vibration ; } struct_EwMtsSUDbGvLjUwS89lioH ;
#endif
#ifndef struct_tag_sSnA7I0tk5aKb4sYk5Ud34C
#define struct_tag_sSnA7I0tk5aKb4sYk5Ud34C
struct tag_sSnA7I0tk5aKb4sYk5Ud34C { real_T r_IMU_CG [ 3 ] ; real_T
R_sensor_body [ 9 ] ; real_T gravitySign ; } ;
#endif
#ifndef typedef_btckilxryq
#define typedef_btckilxryq
typedef struct tag_sSnA7I0tk5aKb4sYk5Ud34C btckilxryq ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 31
#endif
#ifndef SS_INT64
#define SS_INT64 32
#endif
typedef struct hfnefzrumxr_ hfnefzrumxr ; typedef struct m3pw3ynq10
psmsewigyz ;
#endif
