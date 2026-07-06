#ifndef gps_types_h_
#define gps_types_h_
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
#ifndef DEFINED_TYPEDEF_FOR_GPSSensorBus_
#define DEFINED_TYPEDEF_FOR_GPSSensorBus_
typedef struct { real_T device_id ; real_T lat_deg ; real_T lon_deg ; real_T
alt_m ; real_T eph_m ; real_T epv_m ; real_T vel_mps ; real_T vel_n_mps ;
real_T vel_e_mps ; real_T vel_d_mps ; real_T cog_deg ; real_T
satellites_used_nd ; } GPSSensorBus ;
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
#ifndef struct_tag_w4tojZfPSThQFKwJE3XCWB
#define struct_tag_w4tojZfPSThQFKwJE3XCWB
struct tag_w4tojZfPSThQFKwJE3XCWB { uint32_T Seed ; uint32_T State [ 625 ] ;
} ;
#endif
#ifndef typedef_g52hfdtl5q
#define typedef_g52hfdtl5q
typedef struct tag_w4tojZfPSThQFKwJE3XCWB g52hfdtl5q ;
#endif
#ifndef typedef_b0i0hja0px
#define typedef_b0i0hja0px
typedef int32_T b0i0hja0px ;
#endif
#ifndef coder_internal_RngNt_constants
#define coder_internal_RngNt_constants
#define coder_internal_RngNt_ziggurat (0)
#define coder_internal_RngNt_polar (1)
#define coder_internal_RngNt_inversion (2)
#endif
#ifndef struct_tag_zkWDp8qybCRcfv0bzj6TR
#define struct_tag_zkWDp8qybCRcfv0bzj6TR
struct tag_zkWDp8qybCRcfv0bzj6TR { b0i0hja0px NtMethod ; real_T
SavedPolarValue ; boolean_T HaveSavedPolarValue ; g52hfdtl5q * Generator ;
g52hfdtl5q MtGenerator ; } ;
#endif
#ifndef typedef_kpta0c52b1
#define typedef_kpta0c52b1
typedef struct tag_zkWDp8qybCRcfv0bzj6TR kpta0c52b1 ;
#endif
#ifndef struct_tag_QSiAJE9RRAOaHSqtzkI8IC
#define struct_tag_QSiAJE9RRAOaHSqtzkI8IC
struct tag_QSiAJE9RRAOaHSqtzkI8IC { boolean_T tunablePropertyChanged [ 4 ] ;
int32_T isInitialized ; boolean_T TunablePropsChanged ; real_T
HorizontalPositionAccuracy ; real_T VerticalPositionAccuracy ; real_T
VelocityAccuracy ; real_T DecayFactor ; kpta0c52b1 * pStream ; real_T
pPositionErrorFilterNum ; real_T pPositionErrorFilterDen [ 2 ] ; real_T
pPositionErrorFilterStates [ 3 ] ; real_T pSigmaScaled [ 3 ] ; kpta0c52b1
coder_buffer_pobj0 ; } ;
#endif
#ifndef typedef_o3ise5hhyf
#define typedef_o3ise5hhyf
typedef struct tag_QSiAJE9RRAOaHSqtzkI8IC o3ise5hhyf ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 29
#endif
#ifndef SS_INT64
#define SS_INT64 30
#endif
typedef struct hj5kjp0tolf_ hj5kjp0tolf ; typedef struct d2natfdpdq
jdu5i0vx12 ;
#endif
