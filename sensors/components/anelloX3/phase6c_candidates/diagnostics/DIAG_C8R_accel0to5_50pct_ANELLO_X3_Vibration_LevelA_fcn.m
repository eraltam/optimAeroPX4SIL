function [accel_vib_mps2, gyro_vib_radps] = DIAG_C8R_accel0to5_50pct_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration, seed_u32)
% DIAGNOSTIC ONLY -- not a candidate, never wired into any .slx, never flown. Identical to C8R
% except G_accel(1) (0-5Hz gain) scaled to 50% of its full value (2.2177146100), per the
% graduated isolation-test sweep in M2_C8R_C9R_HIL_FAIL_ASSESSMENT.md. Purpose: quantify the D3
% fidelity cost at intermediate gain levels between full (C8R, arm-denied) and zero (DIAG_C8R_
% no_accel_0to5hz, D3 gap reopens) -- does NOT test the EKF-arming mechanism itself (no offline
% EKF2 replay capability exists yet). Any gain level's actual safety behavior still requires a
% physical HIL retest or new replay infrastructure -- not established by this diagnostic alone.

persistent w_lp_a w_bp1_a w_bp2_a w_bp3_a;
persistent w_lp_g w_bp1_g w_bp2_g w_bp3_g;
persistent prng_state;

if isempty(w_lp_a)
    w_lp_a  = zeros(2, 3); w_bp1_a = zeros(4, 3); w_bp2_a = zeros(4, 3); w_bp3_a = zeros(4, 3);
    w_lp_g  = zeros(2, 3); w_bp1_g = zeros(4, 3); w_bp2_g = zeros(4, 3); w_bp3_g = zeros(4, 3);
    if seed_u32 ~= 0
        prng_state = uint32(seed_u32);
    else
        prng_state = uint32(2463534242);
    end
end

[accel_tonal, gyro_tonal] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration);

if enableVibration == 0
    accel_vib_mps2 = accel_tonal;
    gyro_vib_radps = gyro_tonal;
    return;
end

b_lp  = [0.0036216815, 0.0072433630, 0.0036216815];       a_lp  = [-1.8226949252,  0.8371816513];
b_bp1 = [0.0133592000, 0.0, -0.0267184001, 0.0, 0.0133592000];
a_bp1 = [-3.5609474528, 4.8388639737, -2.9769296154, 0.7008967812];
b_bp2 = [0.0068678657, 0.0, -0.0137357315, 0.0, 0.0068678657];
a_bp2 = [-3.2301588846, 4.3697378167, -2.8509340829, 0.7797394590];
b_bp3 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_bp3 = [-2.0796728736, 2.5720148550, -1.5853111255, 0.5869195081];

G_accel = [2.2177146100 * 50/100, 0.5014011500, 1.1786216400, 0.7244736800];
G_gyro  = [0.7933651000, 0.0000000000, 0.0000000000, 0.0000000000];

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
