#ifndef failureInputReadHex_types_h_
#define failureInputReadHex_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_EnumHexFailureType_
#define DEFINED_TYPEDEF_FOR_EnumHexFailureType_
typedef uint32_T EnumHexFailureType ;
#define EnumHexFailureType_none (0U) 
#define EnumHexFailureType_motor1 (1U)
#define EnumHexFailureType_motor2 (2U)
#define EnumHexFailureType_motor3 (3U)
#define EnumHexFailureType_motor4 (4U)
#define EnumHexFailureType_motor5 (5U)
#define EnumHexFailureType_motor6 (6U)
#endif
#ifndef DEFINED_TYPEDEF_FOR_FailureBus_
#define DEFINED_TYPEDEF_FOR_FailureBus_
typedef struct { boolean_T rotorFailure1_nd ; boolean_T rotorFailure2_nd ;
boolean_T rotorFailure3_nd ; boolean_T rotorFailure4_nd ; boolean_T
rotorFailure5_nd ; boolean_T rotorFailure6_nd ; uint8_T sl_padding0 [ 2 ] ; }
FailureBus ;
#endif
#ifndef SS_UINT64
#define SS_UINT64 19
#endif
#ifndef SS_INT64
#define SS_INT64 20
#endif
typedef struct avnv3tkhy1s_ avnv3tkhy1s ; typedef struct ap1crwbd30
ol3lbvizit ;
#endif
