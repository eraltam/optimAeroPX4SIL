#include "rtwtypes.h"
#include "eml_rand_shr3cong_idhNEfES.h"
#include "mwmathutil.h"

real_T eml_rand_shr3cong_idhNEfES(uint32_T state[2])
{
  real_T r;
  real_T r_tmp;
  real_T s;
  real_T x;
  real_T y;
  uint32_T icng;
  uint32_T j;
  uint32_T jsr;
  uint32_T ui;
  static const real_T b[65] = { 0.340945, 0.4573146, 0.5397793, 0.6062427,
    0.6631691, 0.7136975, 0.7596125, 0.8020356, 0.8417227, 0.8792102, 0.9148948,
    0.9490791, 0.9820005, 1.0138492, 1.044781, 1.0749254, 1.1043917, 1.1332738,
    1.161653, 1.189601, 1.2171815, 1.2444516, 1.2714635, 1.298265, 1.3249008,
    1.3514125, 1.3778399, 1.4042211, 1.4305929, 1.4569915, 1.4834527, 1.5100122,
    1.5367061, 1.5635712, 1.5906454, 1.617968, 1.6455802, 1.6735255, 1.7018503,
    1.7306045, 1.7598422, 1.7896223, 1.8200099, 1.851077, 1.8829044, 1.9155831,
    1.9492166, 1.9839239, 2.0198431, 2.0571356, 2.095993, 2.136645, 2.1793713,
    2.2245175, 2.2725186, 2.3239338, 2.3795008, 2.4402218, 2.5075117, 2.5834658,
    2.6713916, 2.7769942, 2.7769942, 2.7769942, 2.7769942 };

  icng = 69069U * state[0] + 1234567U;
  jsr = state[1] << 13 ^ state[1];
  jsr ^= jsr >> 17;
  jsr ^= jsr << 5;
  ui = icng + jsr;
  j = (ui & 63U) + 1U;
  r_tmp = b[(int32_T)j];
  r = (real_T)(int32_T)ui * 4.656612873077393E-10 * r_tmp;
  x = muDoubleScalarAbs(r);
  y = b[(int32_T)j - 1];
  if (!(x <= y)) {
    x = (x - y) / (r_tmp - y);
    icng = 69069U * icng + 1234567U;
    jsr ^= jsr << 13;
    jsr ^= jsr >> 17;
    jsr ^= jsr << 5;
    y = (real_T)(int32_T)(icng + jsr) * 2.328306436538696E-10 + 0.5;
    s = x + y;
    if (s > 1.301198) {
      if (r < 0.0) {
        r = 0.4878992 * x - 0.4878992;
      } else {
        r = 0.4878992 - 0.4878992 * x;
      }
    } else if (!(s <= 0.9689279)) {
      x = 0.4878992 - 0.4878992 * x;
      if (y > 12.67706 - muDoubleScalarExp(-0.5 * x * x) * 12.37586) {
        if (r < 0.0) {
          r = -x;
        } else {
          r = x;
        }
      } else if (!(muDoubleScalarExp(-0.5 * r_tmp * r_tmp) + y * 0.01958303 /
                   r_tmp <= muDoubleScalarExp(-0.5 * r * r))) {
        do {
          icng = 69069U * icng + 1234567U;
          jsr ^= jsr << 13;
          jsr ^= jsr >> 17;
          jsr ^= jsr << 5;
          x = muDoubleScalarLog((real_T)(int32_T)(icng + jsr) *
                                2.328306436538696E-10 + 0.5) / 2.776994;
          icng = 69069U * icng + 1234567U;
          jsr ^= jsr << 13;
          jsr ^= jsr >> 17;
          jsr ^= jsr << 5;
        } while (!(muDoubleScalarLog((real_T)(int32_T)(icng + jsr) *
                   2.328306436538696E-10 + 0.5) * -2.0 > x * x));

        if (r < 0.0) {
          r = x - 2.776994;
        } else {
          r = 2.776994 - x;
        }
      }
    }
  }

  state[0] = icng;
  state[1] = jsr;
  return r;
}
