function [yOut, clampFlag] = anelloInterp1Clamped(xKnots, yKnots, xQuery)
%ANELLOINTERP1CLAMPED  Piecewise-linear interpolation on nonuniform, strictly increasing knots.
%
% Clamps to the nearest table endpoint for queries outside [xKnots(1), xKnots(end)] and reports
% it via clampFlag, instead of extrapolating. Codegen-safe: fixed-size vector inputs, linear
% scan (no assumption of uniform spacing, no histc/discretize).
%
%#codegen

n = numel(xKnots);

if xQuery <= xKnots(1)
    yOut = yKnots(1);
    clampFlag = (xQuery < xKnots(1));
    return;
end

if xQuery >= xKnots(n)
    yOut = yKnots(n);
    clampFlag = (xQuery > xKnots(n));
    return;
end

clampFlag = false;
idx = 1;
for k = 1:n-1
    if xQuery >= xKnots(k) && xQuery <= xKnots(k+1)
        idx = k;
        break;
    end
end

x0 = xKnots(idx);
x1 = xKnots(idx+1);
y0 = yKnots(idx);
y1 = yKnots(idx+1);
frac = (xQuery - x0) / (x1 - x0);
yOut = y0 + frac * (y1 - y0);

end
