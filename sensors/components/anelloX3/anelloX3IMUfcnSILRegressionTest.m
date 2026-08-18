% Phase C regression test: compares the LUT-calibration-integrated ANELLO_X3_IMU_fcn_SIL.m
% against ANELLO_X3_IMU_fcn_SIL_OLD_baseline.m (last committed pre-change version, extracted
% via `git show HEAD:...` in optimAeroPX4SIL's own repo) under an IDENTICAL RNG stream.
%
% Confirms PLAN section 11.7 (accel/mag unchanged) and section 11.6 (gyro stochastic state
% recursions unchanged / same random-draw order), then reports -- NOT gates on an arbitrary
% pass/fail threshold, per this project's standing rule against overclaiming (see
% feedback-imu-fidelity-rigor memory) -- how the calibration-path gyro_meas noise character
% compares to the pre-calibration generic model at a representative in-domain operating point.

clear ANELLO_X3_IMU_fcn_SIL ANELLO_X3_IMU_fcn_SIL_OLD_baseline ANELLO_X3_SiPhOG_CalibrationCore_fcn;

N = 6000;
T_degC = 25.0;
TcountsMid = 0;          % representative in-domain SiPhOG temperature-count operating point

accel_true = [0;0;0];
gyro_true  = [0;0;0];
mag_true   = [0.2;0;0.4];

accelOld = zeros(3,N); gyroOld = zeros(3,N); magOld = zeros(3,N); gbiasOld = zeros(3,N);
accelNew = zeros(3,N); gyroNew = zeros(3,N); magNew = zeros(3,N); gbiasNew = zeros(3,N);

rng(42);
[~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([], [], [], 0, 1);   % reset-only call
for k = 1:N
    [accelOld(:,k), gyroOld(:,k), magOld(:,k), ~, gbiasOld(:,k), ~] = ...
        ANELLO_X3_IMU_fcn_SIL_OLD_baseline(accel_true, gyro_true, mag_true, T_degC, 0);
end

rng(42);
[~,~,~,~,~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL([], [], [], 0, 0, uint8(2), 1);   % reset-only call, no randn consumed
for k = 1:N
    [accelNew(:,k), gyroNew(:,k), magNew(:,k), ~, gbiasNew(:,k)] = ...
        ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, T_degC, TcountsMid, uint8(2), 0);
end

% ---- Section 11.7: accel/mag regression, must be bit-identical ------------------------------
accelDiff = max(abs(accelOld(:) - accelNew(:)));
magDiff   = max(abs(magOld(:)   - magNew(:)));
assert(accelDiff == 0, 'accel_meas regression: max abs diff %.3e (expected exactly 0)', accelDiff);
assert(magDiff   == 0, 'mag_meas regression: max abs diff %.3e (expected exactly 0)', magDiff);
fprintf('OK  accel_meas bit-identical old vs new (%d samples)\n', N);
fprintf('OK  mag_meas   bit-identical old vs new (%d samples)\n', N);

% ---- Section 11.6: gyro bias-instability+pink+RRW state recursion unchanged -----------------
gbiasDiff = max(abs(gbiasOld(:) - gbiasNew(:)));
assert(gbiasDiff == 0, 'gyro_bias (bias+pink+RRW) regression: max abs diff %.3e (expected exactly 0)', gbiasDiff);
fprintf('OK  gyro_bias (bias-instability+pink+RRW) bit-identical old vs new (%d samples)\n', N);
fprintf('    -> confirms randn() call count/order for gyro is unchanged by the calibration integration\n');

% ---- gyro_meas: report noise character old (generic deterministic model) vs new (LUT chain) --
stdOld = std(gyroOld, 0, 2);
stdNew = std(gyroNew, 0, 2);
meanOld = mean(gyroOld, 2);
meanNew = mean(gyroNew, 2);
ratio = stdNew ./ stdOld;

fprintf('\ngyro_meas zero-input noise character @ T=25degC, temperature_counts=%d, N=%d:\n', TcountsMid, N);
fprintf('  axis    std_old[rad/s]   std_new[rad/s]   ratio(new/old)   mean_old   mean_new\n');
for a = 1:3
    fprintf('  %-4d    %.4e       %.4e        %.4f          %+.2e   %+.2e\n', ...
        a, stdOld(a), stdNew(a), ratio(a), meanOld(a), meanNew(a));
end
fprintf(['\nThis is a DESCRIPTIVE comparison, not a pass/fail gate: the calibration path is\n' ...
    'expected to differ from the generic model by design (that is the point of using real\n' ...
    'per-unit LUTs). Reported here so a large, unexplained departure from 1x is visible before\n' ...
    'Phase D/E rather than discovered later. See PLAN section 11.5 for the full noise-enabled\n' ...
    'temperature-sweep + PSD/Allan comparison this feeds into.\n']);
