function [xOut, clampFlag] = anelloInverseMonotonicLUT(xKnots, yKnots, yQuery)
%ANELLOINVERSEMONOTONICLUT  Inverse piecewise-linear lookup on a monotonic table.
%
% Given yQuery, finds xOut such that linearly interpolating (xKnots, yKnots) at xOut
% reproduces yQuery. Requires yKnots strictly monotonic increasing -- verified OFFLINE for
% every table this project uses by anelloX3SiPhOGCalibrationImporterTest.m, not re-checked
% here at runtime (keeps this codegen-safe and cheap; a per-sample monotonicity scan would
% cost more than the interpolation itself for no benefit once the frozen tables are verified).
% Clamps to the nearest table endpoint outside [yKnots(1), yKnots(end)] and reports it via
% clampFlag, instead of extrapolating.
%
%#codegen

n = numel(yKnots);

if yQuery <= yKnots(1)
    xOut = xKnots(1);
    clampFlag = (yQuery < yKnots(1));
    return;
end

if yQuery >= yKnots(n)
    xOut = xKnots(n);
    clampFlag = (yQuery > yKnots(n));
    return;
end

clampFlag = false;
idx = 1;
for k = 1:n-1
    if yQuery >= yKnots(k) && yQuery <= yKnots(k+1)
        idx = k;
        break;
    end
end

x0 = xKnots(idx);
x1 = xKnots(idx+1);
y0 = yKnots(idx);
y1 = yKnots(idx+1);
frac = (yQuery - y0) / (y1 - y0);
xOut = x0 + frac * (x1 - x0);

end
