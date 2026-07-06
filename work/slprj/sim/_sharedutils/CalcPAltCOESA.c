#include "CalcPAltCOESA.h"
#include "COESA76DATA_MACRO.h"
#include "COESAATMOS_MACRO.h"
#include "mwmathutil.h"
#include "rtwtypes.h"

void CalcPAltCOESA(const real_T *pressure, real_T *altitude, real_T
                   *temperature76, real_T *pressureRatio76, int_T numPoints)
{
  int_T i;
  real_T ptemp;
  for (i=0; i<numPoints; i++) {
    int_T bottom = 0;
    int_T top = NUM1976PTS-1;
    int_T idx;
    if (pressure[i] >= pressureRatio76[bottom]*PRESSURE0) {
      idx = bottom;
    } else if (pressure[i] <= pressureRatio76[top]*PRESSURE0) {
      idx = NUM1976PTS-2;
    } else {
      for (;;) {
        idx = (bottom + top)/2;
        if (pressure[i] > pressureRatio76[idx]*PRESSURE0) {
          top = idx - 1;
        } else if (pressure[i] <= pressureRatio76[idx+1]*PRESSURE0) {
          bottom = idx + 1;
        } else {
          break;
        }
      }
    }

    if (pressure[i] == (PRESSURE0 * pressureRatio76[idx])) {
      altitude[i] = altitude76[idx];
    } else {
      if (tempGradient76[idx] != 0.0 ) {
        ptemp = muDoubleScalarPower(pressure[i]/(PRESSURE0 * pressureRatio76[idx]),
          (tempGradient76[idx]/GMR));
        altitude[i] = altitude76[idx] + ((1.0 - ptemp)/
          (tempGradient76[idx] * ptemp))*temperature76[idx];
      } else {
        altitude[i] = altitude76[idx]-((temperature76[idx]/GMR)*
          muDoubleScalarLog(pressure[i]/(PRESSURE0 * pressureRatio76[idx])));
      }
    }
  }
}
