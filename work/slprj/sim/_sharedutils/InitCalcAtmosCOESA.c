#include "InitCalcAtmosCOESA.h"
#include "COESA76DATA_MACRO.h"
#include "COESAATMOS_MACRO.h"
#include "mwmathutil.h"
#include "rtwtypes.h"

void InitCalcAtmosCOESA(real_T *temperature76, real_T *pressureRatio76)
{
  if (temperature76[0] != TEMPERATURE0 ) {
    int_T k;
    temperature76[0] = TEMPERATURE0;
    pressureRatio76[0] = 1.0;
    for (k=0; k<(NUM1976PTS-1); k++) {
      if (tempGradient76[k] != 0.0) {
        temperature76[k+1] = temperature76[k] +
          tempGradient76[k]*(altitude76[k+1] - altitude76[k]);
        pressureRatio76[k+1] = pressureRatio76[k] *
          muDoubleScalarExp(muDoubleScalarLog(temperature76[k]/temperature76[k+1])
                            * GMR/tempGradient76[k]);
      } else {
        temperature76[k+1] = temperature76[k];
        pressureRatio76[k+1] = pressureRatio76[k] *
          muDoubleScalarExp((-GMR)*(altitude76[k+1] - altitude76[k])/
                            temperature76[k]);
      }
    }
  }
}
