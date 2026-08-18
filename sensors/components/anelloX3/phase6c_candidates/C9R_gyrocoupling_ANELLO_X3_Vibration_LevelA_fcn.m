function [accel_vib_mps2, gyro_vib_radps] = C9R_gyrocoupling_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32)
% C9R_GYROCOUPLING -- reproducible-noise-source successor to C9, built on C8R instead of C8 for the
% same reason C8R replaced C8 (2026-08-10): C8/C9's stochastic noise relied on randn() inside a
% compiled Simulink MATLAB Function block, which does not respond to MATLAB's global rng() in any
% simulation mode -- see M2_C9_C8_STATISTICAL_EQUIVALENCE.md. C9R has NO stochastic content of its
% own (the rotation+filter logic is fully deterministic, already verified exact in Simulink for C9
% itself) -- it inherits the reproducibility fix purely by calling C8R instead of C8 and passing
% the seed straight through.
%
% Everything else is UNCHANGED from C9: same 50-65Hz-only SO(3) rotation (27-31Hz deliberately not
% corrected, see C9's own header for the full identifiability-gate story), same isolation filter,
% same fitted rotation matrix. accel_vib_mps2 passed through from C8R unmodified.
%
% Inputs: rotorAngVel_radps, enableVibration -- identical contract to ANELLO_X3_Vibration_LevelA_
% fcn. seed_u32 -- forwarded to C8R unchanged (see that file's header for semantics: effective on
% first call only, 0 = default).
% Outputs: identical contract to ANELLO_X3_Vibration_LevelA_fcn.

persistent w_5065;   % gyro-only isolation-filter state, [order x 3 axes]
if isempty(w_5065)
    w_5065 = zeros(4, 3);
end

[accel_vib_mps2, gyro_c8r] = C8R_broadband_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32);

if enableVibration == 0
    gyro_vib_radps = gyro_c8r;
    return;
end

% ---- Isolation filter (fs=250 Hz, precomputed via scipy.signal.butter) ----------------------
b_5065 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_5065 = [-0.4434478834, 1.5253364381, -0.3380353093, 0.5869195081];

% ---- Fitted rotation (50-65Hz only, SO(3), unchanged from C9 -- see that file's header) -----
R_5065 = [ 0.3118237837,  0.5329830977,  0.7865716404; ...
          -0.5456064221,  0.7781944404, -0.3110097185; ...
          -0.7778686008, -0.3321783113,  0.5334585358];
D_5065 = R_5065 - eye(3);

comp_5065 = zeros(3, 1);
for ax = 1:3
    [y2, w_5065(:, ax)] = df2t_step(b_5065, a_5065, gyro_c8r(ax), w_5065(:, ax));
    comp_5065(ax) = y2;
end

gyro_vib_radps = gyro_c8r + D_5065 * comp_5065;

end

function [y, w_new] = df2t_step(b, a, x, w)
% Direct-form-II-transposed single-sample update -- identical structure to C8R's, kept local here
% rather than shared, matching this codebase's existing convention.
N = numel(a);
y = b(1) * x + w(1);
w_new = zeros(N, 1);
for i = 1:N-1
    w_new(i) = b(i+1) * x - a(i) * y + w(i+1);
end
w_new(N) = b(N+1) * x - a(N) * y;
end
