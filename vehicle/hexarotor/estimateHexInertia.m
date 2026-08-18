function I = estimateHexInertia(totalMass_kg, rotorRadius_m, armMassFraction, bodyRadius_m)
% ENGINEERING ESTIMATE (parameter-confidence Class 3 -- see
% PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 2). The real inertia tensor is
% Class 4 (unknown, needs measurement -- bifilar pendulum test or CAD mass-properties export are
% the two standard ways to get it; see that plan's prioritized-measurement list). This function
% exists so the estimate is computed from an explicit, editable, physically-motivated model instead
% of a single hardcoded number, and so it can be replaced by a measured 3x3 tensor with one line
% changed at the call site.
%
% Model: armMassFraction of totalMass_kg is lumped as 6 equal point masses at rotorRadius_m (motor +
% ESC + propeller mass on each arm); the remainder is a uniform solid sphere of bodyRadius_m
% centered at the CG (battery + avionics + frame). For 6 point masses evenly spaced around a
% circle, symmetry splits their contribution evenly between the two in-plane axes and puts the full
% r^2 term on the vertical (yaw) axis -- this is why a flat multirotor's yaw inertia is always
% roughly double its roll/pitch inertia, which the OLD (wrong-mass) plant value
% (diag([11.8 11.8 23.5]), Izz/Ixx=1.99) already showed and this estimate preserves.
%
% Inputs:
%   totalMass_kg     - vehicle total mass [kg] (Class 1/2, from the named configuration)
%   rotorRadius_m    - motor-center radius [m] (Class 1, 0.4992 m per the reference spec)
%   armMassFraction  - fraction of totalMass_kg on the 6 arms combined [0-1] (Class 3 estimate,
%                      default 0.45 -- T-motor-class hexarotors typically carry a large fraction of
%                      total mass in motors/ESCs/props on a flat frame)
%   bodyRadius_m     - equivalent solid-sphere radius for the central body [m] (Class 3 estimate)
%
% Output:
%   I - 3x3 diagonal inertia tensor [kg*m^2], body FRD axes, no products of inertia (Class 3 --
%       products of inertia are assumed zero for a nominally symmetric layout, not measured; do not
%       treat as validated for an asymmetric payload/CG configuration, see the audit plan Section 3.2)

arguments
    totalMass_kg    (1,1) double {mustBePositive}
    rotorRadius_m   (1,1) double {mustBePositive} = 0.4992
    armMassFraction (1,1) double {mustBeInRange(armMassFraction, 0, 1)} = 0.45
    bodyRadius_m    (1,1) double {mustBePositive} = 0.15
end

armMass_kg  = totalMass_kg * armMassFraction;
bodyMass_kg = totalMass_kg - armMass_kg;

Ixx_arms = 0.5 * armMass_kg * rotorRadius_m^2;
Iyy_arms = Ixx_arms;
Izz_arms = armMass_kg * rotorRadius_m^2;

Ibody = (2/5) * bodyMass_kg * bodyRadius_m^2;

I = diag([Ixx_arms + Ibody, Iyy_arms + Ibody, Izz_arms + Ibody]);
end
