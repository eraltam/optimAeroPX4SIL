% Phase 7 regression check: ANELLO_X3_Vibration_LevelA_fcn_v2.m with
% anelloVibrationLevelA2DefaultParams() (all Ka/Kg=1, broadband off) must reproduce
% ANELLO_X3_Vibration_LevelA_fcn.m bit-for-bit, per-sample, over a realistic per-motor rotor-speed
% trace -- not just a synthetic uniform ramp (phase6c_smoke_test_candidates.m's check), so any
% per-motor phase-offset asymmetry the persistent `phase` state depends on is actually exercised.

traceFile = 'C:\AS\AnalysisIMU\vibration_full_mission_comparison\extracted\session_phase6c_C1_control_02\rotorAngVel_radps.csv';
T = readtable(traceFile);
omega = [T.omega1_radps, T.omega2_radps, T.omega3_radps, T.omega4_radps, T.omega5_radps, T.omega6_radps];
N = size(omega, 1);
fprintf('Loaded %d samples from %s\n', N, traceFile);

clear ANELLO_X3_Vibration_LevelA_fcn ANELLO_X3_Vibration_LevelA_fcn_v2

accelProd = zeros(3, N);
gyroProd  = zeros(3, N);
accelV2   = zeros(3, N);
gyroV2    = zeros(3, N);
defaultParams = anelloVibrationLevelA2DefaultParams();

for k = 1:N
    om6 = omega(k, :)';
    [accelProd(:,k), gyroProd(:,k)] = ANELLO_X3_Vibration_LevelA_fcn(om6, 1);
    [accelV2(:,k), gyroV2(:,k)]     = ANELLO_X3_Vibration_LevelA_fcn_v2(om6, 1, defaultParams);
end

accelMaxAbsDiff = max(abs(accelProd(:) - accelV2(:)));
gyroMaxAbsDiff  = max(abs(gyroProd(:) - gyroV2(:)));
% Not exact isequal(): v2 sums the fundamental/BPF terms as two separately-scaled-by-direction
% terms (needed so per-band direction can differ), vs. production's single combined
% (K_fund*s1+K_bpf*s2)*accel_dir -- a different but mathematically equivalent summation order that
% differs at floating-point roundoff (~1e-15) even when both direction vectors are identical (1.0
% params). Tolerance is set well above float64 eps but far below any physically meaningful signal.
TOL = 1e-10;
isRegressionOk = accelMaxAbsDiff < TOL && gyroMaxAbsDiff < TOL;

fprintf('accel max abs diff: %.3e m/s^2 (tolerance %.1e)\n', accelMaxAbsDiff, TOL);
fprintf('gyro  max abs diff: %.3e rad/s (tolerance %.1e)\n', gyroMaxAbsDiff, TOL);
fprintf('Within floating-point tolerance: %d\n', isRegressionOk);

% enableVibration<=0 exact-zero check, both functions
clear ANELLO_X3_Vibration_LevelA_fcn ANELLO_X3_Vibration_LevelA_fcn_v2
[a0p, g0p] = ANELLO_X3_Vibration_LevelA_fcn(zeros(6,1), 0);
[a0v, g0v] = ANELLO_X3_Vibration_LevelA_fcn_v2(zeros(6,1), 0, defaultParams);
fprintf('enableVibration=0: prod=[%s]/[%s], v2=[%s]/[%s], match=%d\n', ...
    num2str(a0p'), num2str(g0p'), num2str(a0v'), num2str(g0v'), ...
    isequal(a0p,a0v) && isequal(g0p,g0v));

if isRegressionOk
    fprintf('\nPASS: ANELLO_X3_Vibration_LevelA_fcn_v2.m with default params matches production to\n');
    fprintf('floating-point tolerance (%.1e) -- safe to use as the Phase 7/8 base.\n', TOL);
else
    fprintf('\nFAIL: mismatch exceeds floating-point tolerance -- do not use v2 until resolved.\n');
end
