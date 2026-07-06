#ifndef PX4OutputMappingHex_types_h_
#define PX4OutputMappingHex_types_h_
#include "rtwtypes.h"
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
#ifndef SS_UINT64
#define SS_UINT64 19
#endif
#ifndef SS_INT64
#define SS_INT64 20
#endif
typedef struct f0oupnkqiwv_ f0oupnkqiwv ; typedef struct bhapmjnfo5
jrvsdctowq ;
#endif
