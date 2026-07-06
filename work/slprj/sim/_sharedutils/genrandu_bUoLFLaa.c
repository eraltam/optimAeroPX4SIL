#include "rtwtypes.h"
#include "genrandu_bUoLFLaa.h"

void genrandu_bUoLFLaa(uint32_T s, uint32_T *state, real_T *r)
{
  uint32_T a;
  uint32_T hi;
  hi = s / 127773U;
  a = (s - hi * 127773U) * 16807U;
  hi *= 2836U;
  if (a < hi) {
    *state = ~(hi - a) & 2147483647U;
  } else {
    *state = a - hi;
  }

  *r = (real_T)*state * 4.656612875245797E-10;
}
