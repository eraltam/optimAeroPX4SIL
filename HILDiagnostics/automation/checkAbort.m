function abortFlag = checkAbort(angVel, pos, velNED, euler)
%#codegen
% Reject invalid states immediately, but require ordinary safety-limit
% violations to persist. A single solver/transition sample should not end a
% long SIL mission; sustained or extreme motion still stops the plant.

persistent violationCount
if isempty(violationCount)
    violationCount = uint16(0);
end

speed = sqrt(sum(velNED.^2));
rollDeg = abs(euler(1))*180/pi;
pitchDeg = abs(euler(2))*180/pi;
bodyRateNorm = sqrt(sum(angVel.^2));

invalidState = any(~isfinite(pos)) || any(~isfinite(velNED)) || ...
    any(~isfinite(euler)) || any(~isfinite(angVel));

ordinaryViolation = (speed > 80) || (rollDeg > 60) || ...
    (pitchDeg > 60) || (bodyRateNorm > 3) || ...
    (pos(3) > 15) || (pos(3) < -600);

extremeViolation = (speed > 120) || (rollDeg > 85) || ...
    (pitchDeg > 85) || (bodyRateNorm > 6) || ...
    (pos(3) > 50) || (pos(3) < -1000);

if ordinaryViolation
    if violationCount < intmax("uint16")
        violationCount = violationCount + uint16(1);
    end
else
    violationCount = uint16(0);
end

% VehicleSilSimulation runs at 250 Hz. Require 0.5 s (125 consecutive
% samples) for ordinary threshold crossings.
abortFlag = invalidState || extremeViolation || (violationCount >= uint16(125));
end
