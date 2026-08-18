function [accel_vib_mps2, gyro_vib_radps] = C8R_broadband_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32)
% C8R_BROADBAND -- reproducible-noise-source successor to C8, built after C8 was found to be
% impossible to verify for statistical equivalence (2026-08-10): randn() inside a compiled
% Simulink MATLAB Function block does not respond to MATLAB's global rng() in ANY simulation mode
% tested (Normal, Accelerator, Rapid Accelerator -- see M2_C9_C8_STATISTICAL_EQUIVALENCE.md).
% Since this candidate's entire contribution IS its stochastic noise, an uncontrollable noise
% source in the compiled path means neither determinism nor statistical equivalence could ever be
% demonstrated between this file's offline behavior and any real HIL/Simulink execution of it.
%
% Fix: the white-noise generator is now an explicit xorshift32 PRNG carrying its OWN persistent
% uint32 state, completely decoupled from MATLAB's global rng()/randn() machinery -- the same
% algorithm runs identically whether interpreted (offline script) or compiled (Simulink), because
% it never touches whatever internal, uncontrollable mechanism Simulink was using for randn().
%
% SEEDING (signature changed 2026-08-10, second attempt): the first version of this file read its
% seed from a companion function's persistent state (C8R_prng_seed.m), settable from an offline
% test driver before the first call. That does NOT work: confirmed directly that a compiled
% Simulink MATLAB Function block's internal call to a same-named companion function does not share
% persistent storage with an interpreted-session call to that function -- setting the seed from
% the command line had zero effect on what the compiled block saw (it always used the companion
% function's hardcoded default). Fixed by making the seed a genuine INPUT ARGUMENT instead of
% cross-context shared state -- no ambiguity possible, both execution paths receive the literal
% same value because it is wired/passed explicitly, not read from some other function's memory.
% `seed_u32` only takes effect on the FIRST call after a `clear`/model-init (persistent state
% empty); ignored on every subsequent call (the PRNG evolves on its own from there, exactly like
% every other persistent state in this codebase, e.g. production's `phase`). Pass 0 to use the
% hardcoded default (`2463534242`) -- this is what a real HIL/production wiring would do via a
% Constant block feeding this 3rd port, since there is no meaningful "vary the seed" case in a
% deployed system, only in offline/HIL-comparison testing.
%
% Everything else is unchanged from C8: same 4-band filter design (2nd-order Butterworth per band,
% fs=250Hz, DF2T recursion), same tonal+broadband architecture (calls production's
% ANELLO_X3_Vibration_LevelA_fcn directly, adds a filtered broadband term on top), same
% independent-per-axis-per-band noise structure (24 channels: accel 3 axes x 4 bands, gyro 3 x 4).
% Gains are the SAME numeric values as C8 -- the xorshift32+Box-Muller generator was verified
% (200k-sample offline test) to match randn()'s statistics closely (mean 0.00009, std 0.99984,
% skewness 0.013, kurtosis 3.006, KS-test does not reject normality, p=0.58), so C8's gain fit
% transfers without needing to be redone; verify this assumption numerically via D3 before trusting
% it (done in M2_C8R_C9R_REBUILD.md, not assumed here).
%
% Inputs: rotorAngVel_radps, enableVibration -- identical contract to ANELLO_X3_Vibration_LevelA_
% fcn. seed_u32 [1x1 double or uint32] -- PRNG seed, effective on first call only, 0 = use default.
% Outputs: identical contract to ANELLO_X3_Vibration_LevelA_fcn.

persistent w_lp_a w_bp1_a w_bp2_a w_bp3_a;   % accel filter states, [order x 3 axes]
persistent w_lp_g w_bp1_g w_bp2_g w_bp3_g;   % gyro filter states
persistent prng_state;                        % explicit xorshift32 state, uint32 scalar

if isempty(w_lp_a)
    w_lp_a  = zeros(2, 3); w_bp1_a = zeros(4, 3); w_bp2_a = zeros(4, 3); w_bp3_a = zeros(4, 3);
    w_lp_g  = zeros(2, 3); w_bp1_g = zeros(4, 3); w_bp2_g = zeros(4, 3); w_bp3_g = zeros(4, 3);
    if seed_u32 ~= 0
        prng_state = uint32(seed_u32);
    else
        prng_state = uint32(2463534242);
    end
end

% ---- Tonal part: production's own model, called directly (not shadowed) --------------------
[accel_tonal, gyro_tonal] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration);

if enableVibration == 0
    accel_vib_mps2 = accel_tonal;
    gyro_vib_radps = gyro_tonal;
    return;
end

% ---- Filter coefficients (fs=250 Hz, precomputed via scipy.signal.butter) -------------------
b_lp  = [0.0036216815, 0.0072433630, 0.0036216815];       a_lp  = [-1.8226949252,  0.8371816513];
b_bp1 = [0.0133592000, 0.0, -0.0267184001, 0.0, 0.0133592000];
a_bp1 = [-3.5609474528, 4.8388639737, -2.9769296154, 0.7008967812];
b_bp2 = [0.0068678657, 0.0, -0.0137357315, 0.0, 0.0068678657];
a_bp2 = [-3.2301588846, 4.3697378167, -2.8509340829, 0.7797394590];
b_bp3 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_bp3 = [-2.0796728736, 2.5720148550, -1.5853111255, 0.5869195081];

% ---- Gains (unchanged from C8 -- see header) -------------------------------------------------
G_accel = [2.2177146100, 0.5014011500, 1.1786216400, 0.7244736800];   % [0-5, 5-15, 18-25, 30-45] Hz
G_gyro  = [0.7933651000, 0.0000000000, 0.0000000000, 0.0000000000];

% ---- Explicit PRNG: 24 independent standard-normal draws (12 Box-Muller pairs), fixed order:
% accel[band1 x,y,z; band2 x,y,z; band3 x,y,z; band4 x,y,z], then gyro likewise -----------------
noise_a = zeros(3, 4);
noise_g = zeros(3, 4);
for band = 1:4
    for ax = 1:3
        [z, prng_state] = xorshift32_normal(prng_state);
        noise_a(ax, band) = z;
    end
end
for band = 1:4
    for ax = 1:3
        [z, prng_state] = xorshift32_normal(prng_state);
        noise_g(ax, band) = z;
    end
end

bb_accel = zeros(3, 1);
bb_gyro  = zeros(3, 1);
for ax = 1:3
    [y, w_lp_a(:, ax)]  = df2t_step(b_lp,  a_lp,  noise_a(ax, 1), w_lp_a(:, ax));  bb_accel(ax) = bb_accel(ax) + G_accel(1) * y;
    [y, w_bp1_a(:, ax)] = df2t_step(b_bp1, a_bp1, noise_a(ax, 2), w_bp1_a(:, ax)); bb_accel(ax) = bb_accel(ax) + G_accel(2) * y;
    [y, w_bp2_a(:, ax)] = df2t_step(b_bp2, a_bp2, noise_a(ax, 3), w_bp2_a(:, ax)); bb_accel(ax) = bb_accel(ax) + G_accel(3) * y;
    [y, w_bp3_a(:, ax)] = df2t_step(b_bp3, a_bp3, noise_a(ax, 4), w_bp3_a(:, ax)); bb_accel(ax) = bb_accel(ax) + G_accel(4) * y;

    [y, w_lp_g(:, ax)]  = df2t_step(b_lp,  a_lp,  noise_g(ax, 1), w_lp_g(:, ax));  bb_gyro(ax) = bb_gyro(ax) + G_gyro(1) * y;
    [y, w_bp1_g(:, ax)] = df2t_step(b_bp1, a_bp1, noise_g(ax, 2), w_bp1_g(:, ax)); bb_gyro(ax) = bb_gyro(ax) + G_gyro(2) * y;
    [y, w_bp2_g(:, ax)] = df2t_step(b_bp2, a_bp2, noise_g(ax, 3), w_bp2_g(:, ax)); bb_gyro(ax) = bb_gyro(ax) + G_gyro(3) * y;
    [y, w_bp3_g(:, ax)] = df2t_step(b_bp3, a_bp3, noise_g(ax, 4), w_bp3_g(:, ax)); bb_gyro(ax) = bb_gyro(ax) + G_gyro(4) * y;
end

accel_vib_mps2 = accel_tonal + bb_accel;
gyro_vib_radps = gyro_tonal + bb_gyro;

end

function [z, new_state] = xorshift32_normal(state)
% One standard-normal draw via xorshift32 (2 advances) + Box-Muller (cosine branch only -- the
% sine branch is discarded rather than cached across calls, simpler persistent-state bookkeeping
% at the cost of using only half of each pair; negligible efficiency concern here).
x = state;
x = bitxor(x, bitshift(x, 13));
x = bitxor(x, bitshift(x, -17));
x = bitxor(x, bitshift(x, 5));
s1 = x;
u1 = max(double(s1) / 4294967295.0, 1e-10);

x = bitxor(s1, bitshift(s1, 13));
x = bitxor(x, bitshift(x, -17));
x = bitxor(x, bitshift(x, 5));
s2 = x;
u2 = double(s2) / 4294967295.0;

z = sqrt(-2 * log(u1)) * cos(2 * pi * u2);
new_state = s2;
end

function [y, w_new] = df2t_step(b, a, x, w)
% Direct-form-II-transposed single-sample update. b has N+1 coeffs, a has N coeffs (a0=1 implicit,
% not stored), w is the [N x 1] state vector.
N = numel(a);
y = b(1) * x + w(1);
w_new = zeros(N, 1);
for i = 1:N-1
    w_new(i) = b(i+1) * x - a(i) * y + w(i+1);
end
w_new(N) = b(N+1) * x - a(N) * y;
end
