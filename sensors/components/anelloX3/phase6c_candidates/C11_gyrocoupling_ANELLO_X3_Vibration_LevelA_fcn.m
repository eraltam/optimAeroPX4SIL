function [accel_vib_mps2, gyro_vib_radps] = C11_gyrocoupling_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32)
% C11_GYROCOUPLING -- built on C10 instead of C8R, for the same reason C9R was built on C8R instead
% of C8 (this file has no stochastic content of its own; it inherits C10's motor-command-gated
% envelope fix purely by calling C10 and passing the seed straight through). See
% M2_C8R_C9R_HIL_FAIL_ASSESSMENT.md / C10's own header for why the envelope fix exists.
%
% Everything else is UNCHANGED from C9/C9R: same 50-65Hz-only SO(3) rotation (27-31Hz deliberately
% not corrected, see C9's own header for the identifiability-gate rationale), same isolation filter,
% same fitted rotation matrix. Because the rotation is applied to C10's gyro output (which is already
% envelope-scaled), the isolated 50-65Hz component this file corrects is automatically envelope-gated
% too -- exact zero at rotorAngVel=0, no separate gating logic needed here.
%
% Inputs/outputs: identical contract to ANELLO_X3_Vibration_LevelA_fcn. seed_u32 forwarded to C10
% unchanged.

persistent w_5065;   % gyro-only isolation-filter state, [order x 3 axes]
if isempty(w_5065)
    w_5065 = zeros(4, 3);
end

[accel_vib_mps2, gyro_c10] = C10_broadband_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32);

if enableVibration == 0
    gyro_vib_radps = gyro_c10;
    return;
end

% ---- Isolation filter (fs=250 Hz, precomputed via scipy.signal.butter) ----------------------
b_5065 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_5065 = [-0.4434478834, 1.5253364381, -0.3380353093, 0.5869195081];

% ---- Fitted rotation (50-65Hz only, SO(3), unchanged from C9/C9R) --------------------------
R_5065 = [ 0.3118237837,  0.5329830977,  0.7865716404; ...
          -0.5456064221,  0.7781944404, -0.3110097185; ...
          -0.7778686008, -0.3321783113,  0.5334585358];
D_5065 = R_5065 - eye(3);

comp_5065 = zeros(3, 1);
for ax = 1:3
    [y2, w_5065(:, ax)] = df2t_step(b_5065, a_5065, gyro_c10(ax), w_5065(:, ax));
    comp_5065(ax) = y2;
end

gyro_vib_radps = gyro_c10 + D_5065 * comp_5065;

end

function [y, w_new] = df2t_step(b, a, x, w)
N = numel(a);
y = b(1) * x + w(1);
w_new = zeros(N, 1);
for i = 1:N-1
    w_new(i) = b(i+1) * x - a(i) * y + w(i+1);
end
w_new(N) = b(N+1) * x - a(N) * y;
end
