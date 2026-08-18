function [accel_vib_mps2, gyro_vib_radps] = C10_broadband_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32)
% C10_BROADBAND -- architectural redesign of C8R/C9R's broadband term, per the project owner's
% decision after C8R/C9R's HIL FAIL (arm denied, "Preflight Fail: velocity estimate error",
% M2_C8R_C9R_HIL_FAIL_ASSESSMENT.md) and the offline EKF-sensitivity screen coming back
% inconclusive (~4x short of the real event -- not usable to select a "safe" reduced gain,
% M2_OFFLINE_EKF_REPLAY_SCREEN_PROTOCOL.md).
%
% ROOT CAUSE THIS FIXES: C8R/C9R's broadband term used a CONSTANT gain, active any time
% enableVibration=1, regardless of actual rotor speed -- i.e. it injected full-magnitude correlated
% low-frequency accel noise even while the vehicle sat stationary and disarmed (rotorAngVel=0).
% That is architecturally wrong: this term represents rotor/vehicle-induced vibration, not a
% permanent sensor-internal noise floor -- it should be governed by actual motor state, exactly like
% production's own tonal model already is (see ANELLO_X3_Vibration_LevelA_fcn.m: amplitude scales as
% rotorAngVel_radps(i)^2 per motor, exact zero at rotorAngVel=0, "RMS/omega^2 constant to ~2.5%
% across the full 0-100% throttle range" per that file's own calibration comment).
%
% FIX: multiply the entire broadband contribution (all 4 bands, accel AND gyro) by a single
% envelope term reusing that SAME already-established quadratic omega-dependence -- not a newly-fit
% curve (per explicit project-owner decision: no new fitting against main-flight/VALIDATION data for
% the envelope's growth shape; only its zero-point is being newly introduced/checked, against
% TRAIN):
%
%     envelope = mean_i(rotorAngVel_radps(i)^2) / omega_hover_radps^2
%     broadband = envelope * filtered_noise
%
% omega_hover_radps = 0.5 * maxRPM(3473) * 2*pi/60 = ~181.87 rad/s -- the SAME 50%-throttle hover
% reference point already used and re-verified in ANELLO_X3_Vibration_LevelA_fcn.m's own 2026-07-31
% recalibration (kept as a literal inline constant here for the same MATLAB-Function-block
% struct-call-compile reason documented in optimAeroPX4SIL/CLAUDE.md Section 3 -- do not replace
% with a call to setUpActuators.m).
%
% Properties this gives (all four required by the project owner's redesign spec):
%   - envelope(idle, all rotorAngVel=0) = EXACTLY 0 -- not "near-zero," exactly zero, same guarantee
%     production's own tonal model already has. This removes the arm-denial mechanism at its root
%     (no correlated low-frequency disturbance at all while genuinely stationary), not just shrinks
%     it, and needs no new "is-armed" input -- it is driven by the same rotorAngVel_radps input
%     already wired to this whole vibration chain, which is itself already zero while disarmed
%     (hexActuator.slx zeros all motor commands on disarm, per ANELLO_X3_Vibration_LevelA_fcn.m's
%     own header).
%   - No hard step at the disarmed->armed transition: arming does not itself change rotor speed
%     (motors idle at RPM~0 immediately after arming, same as before); envelope is a continuous
%     function of actual commanded rotor speed, so it ramps up continuously with real spool-up,
%     never discontinuously with the arm/disarm event itself.
%   - envelope(hover, mean omega^2 = omega_hover^2) = 1 -- reproduces C8R's exact fitted gains (and
%     therefore its D3 broadband-power improvement) at the same hover condition C8R/C8/C7 were all
%     originally fit against. Re-verified via D3 on the main flight (M2_C10_C11_REBUILD.md), not
%     assumed.
%   - Smooth (quadratic, continuously differentiable) growth with commanded rotor speed at all other
%     throttle settings -- same functional form as production's own tonal model, not a new curve.
%
% Everything else (PRNG, filters, gains, tonal passthrough, seeding contract) is unchanged from
% C8R -- see that file's header for the xorshift32/DF2T/seeding design, not repeated here.

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

% ---- Motor-command-gated envelope (the actual fix -- see header) -----------------------------
omega_hover_radps = 0.5 * 3473 * 2*pi/60;   % ~181.87 rad/s, same reference as the tonal model
envelope = mean(rotorAngVel_radps(:).^2) / omega_hover_radps^2;

% ---- Filter coefficients (fs=250 Hz, precomputed via scipy.signal.butter) -------------------
b_lp  = [0.0036216815, 0.0072433630, 0.0036216815];       a_lp  = [-1.8226949252,  0.8371816513];
b_bp1 = [0.0133592000, 0.0, -0.0267184001, 0.0, 0.0133592000];
a_bp1 = [-3.5609474528, 4.8388639737, -2.9769296154, 0.7008967812];
b_bp2 = [0.0068678657, 0.0, -0.0137357315, 0.0, 0.0068678657];
a_bp2 = [-3.2301588846, 4.3697378167, -2.8509340829, 0.7797394590];
b_bp3 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_bp3 = [-2.0796728736, 2.5720148550, -1.5853111255, 0.5869195081];

% ---- Gains (unchanged from C8/C8R -- fit at the hover condition where envelope=1) -------------
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

accel_vib_mps2 = accel_tonal + envelope * bb_accel;
gyro_vib_radps = gyro_tonal + envelope * bb_gyro;

end

function [z, new_state] = xorshift32_normal(state)
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
N = numel(a);
y = b(1) * x + w(1);
w_new = zeros(N, 1);
for i = 1:N-1
    w_new(i) = b(i+1) * x - a(i) * y + w(i+1);
end
w_new(N) = b(N+1) * x - a(N) * y;
end
