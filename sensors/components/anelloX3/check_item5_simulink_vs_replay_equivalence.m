% Item 5 (P0.3) decisive test: does the COMPILED Simulink MATLAB Function block
% (test_vibration_equivalence_harness/VibrationWrapper, calling the exact same
% ANELLO_X3_Vibration_LevelA_fcn.m as production's blk_29) produce the same output as a plain
% MATLAB for-loop calling the identical .m file, given the identical rotorAngVel_radps sequence?
%
% Three cases per the closure criteria: constant RPM, a known linear ramp, and C1's full real
% recorded trace. For each: first non-matching sample, optimal lag (cross-correlation), max/RMS
% error per axis, PSD/45-70Hz power agreement, and the effect of excluding sample 1.

Ts_harness = 0.004;
modelName = 'test_vibration_equivalence_harness';
load_system(modelName);

results = struct();

cases = struct('name', {}, 'omega', {});

% Case 1: constant RPM (100 rad/s, all 6 motors identical)
N1 = 2000;
cases(1).name = 'constant_rpm';
cases(1).omega = 100 * ones(N1, 6);

% Case 2: known linear ramp, 0 -> maxOmega over the run, all motors identical
N2 = 2000;
maxOmega = 3473 * 2*pi/60;
cases(2).name = 'known_ramp';
rampProfile = linspace(0, maxOmega, N2)';
cases(2).omega = repmat(rampProfile, 1, 6);

% Case 3: C1's full real recorded trace
T = readtable('C:\AS\AnalysisIMU\vibration_full_mission_comparison\extracted\session_phase6c_C1_control_02\rotorAngVel_radps.csv');
cases(3).name = 'c1_real_trace';
cases(3).omega = [T.omega1_radps, T.omega2_radps, T.omega3_radps, T.omega4_radps, T.omega5_radps, T.omega6_radps];

for c = 1:numel(cases)
    name = cases(c).name;
    omega = cases(c).omega;
    N = size(omega, 1);
    fprintf('\n========== CASE: %s (N=%d) ==========\n', name, N);

    % ---- Simulink path ----
    tVec = (0:N-1)' * Ts_harness;
    omegaWsInput = struct('time', tVec, 'signals', struct('values', omega, 'dimensions', 6));
    assignin('base', 'omegaWsInput', omegaWsInput);
    assignin('base', 'Ts_harness', Ts_harness);

    set_param(modelName, 'StopTime', num2str((N-1) * Ts_harness));
    simOut = sim(modelName);

    accelVibOut = simOut.get('accelVibOut');
    gyroVibOut = simOut.get('gyroVibOut');
    % ToWorkspace 'Array' format for a 3x1 vector signal comes back as [3 x 1 x N]; reshape to
    % [N x 3] to match accelReplay/gyroReplay's shape below.
    accelVibOut = reshape(accelVibOut, 3, N)';
    gyroVibOut = reshape(gyroVibOut, 3, N)';

    % ---- Standalone MATLAB-loop replay ----
    clear ANELLO_X3_Vibration_LevelA_fcn
    accelReplay = zeros(N, 3);
    gyroReplay = zeros(N, 3);
    for k = 1:N
        [a, g] = ANELLO_X3_Vibration_LevelA_fcn(omega(k, :)', 1);
        accelReplay(k, :) = a';
        gyroReplay(k, :) = g';
    end

    % ---- Comparison ----
    accelDiff = accelVibOut - accelReplay;
    gyroDiff = gyroVibOut - gyroReplay;

    firstMismatchA = find(any(abs(accelDiff) > 1e-9, 2), 1);
    firstMismatchG = find(any(abs(gyroDiff) > 1e-9, 2), 1);
    fprintf('First accel sample exceeding 1e-9 abs diff: %s\n', mat2str(firstMismatchA));
    fprintf('First gyro  sample exceeding 1e-9 abs diff: %s\n', mat2str(firstMismatchG));

    maxAbsA = max(abs(accelDiff), [], 1);
    rmsA = sqrt(mean(accelDiff.^2, 1));
    maxAbsG = max(abs(gyroDiff), [], 1);
    rmsG = sqrt(mean(gyroDiff.^2, 1));
    fprintf('accel max|diff| per axis [x y z]: %s m/s^2\n', mat2str(maxAbsA, 4));
    fprintf('accel RMS diff per axis   [x y z]: %s m/s^2\n', mat2str(rmsA, 4));
    fprintf('gyro  max|diff| per axis [x y z]: %s rad/s\n', mat2str(maxAbsG, 4));
    fprintf('gyro  RMS diff per axis   [x y z]: %s rad/s\n', mat2str(rmsG, 4));

    % Optimal lag via cross-correlation on the norm signal (only meaningful if N is large enough)
    normVibOut = sqrt(sum(accelVibOut.^2, 2));
    normReplay = sqrt(sum(accelReplay.^2, 2));
    if N > 50 && any(normVibOut) && any(normReplay)
        maxLag = min(20, floor(N/4));
        [xc, lags] = xcorr(normVibOut - mean(normVibOut), normReplay - mean(normReplay), maxLag, 'coeff');
        [~, idxMax] = max(xc);
        optimalLag = lags(idxMax);
        fprintf('Optimal lag (samples), accel norm cross-correlation: %d (corr=%.6f at lag0=%.6f)\n', ...
            optimalLag, xc(idxMax), xc(lags == 0));
    else
        optimalLag = NaN;
        fprintf('Optimal lag: skipped (degenerate/zero signal)\n');
    end

    % Effect of excluding sample 1
    if N > 1
        maxAbsA_excl1 = max(abs(accelDiff(2:end, :)), [], 1);
        rmsA_excl1 = sqrt(mean(accelDiff(2:end, :).^2, 1));
        fprintf('accel max|diff| EXCLUDING sample 1: %s m/s^2\n', mat2str(maxAbsA_excl1, 4));
        fprintf('accel RMS diff  EXCLUDING sample 1: %s m/s^2\n', mat2str(rmsA_excl1, 4));
    end

    % PSD / 45-70Hz power agreement (only meaningful for the long real trace, case 3)
    if N >= 750
        fs = 1 / Ts_harness;
        [pSim, f] = pwelch(normVibOut - mean(normVibOut), hamming(256), 128, 256, fs);
        [pRep, ~] = pwelch(normReplay - mean(normReplay), hamming(256), 128, 256, fs);
        sel = f >= 45 & f < 70;
        pSim4570 = trapz(f(sel), pSim(sel));
        pRep4570 = trapz(f(sel), pRep(sel));
        fprintf('45-70Hz power: Simulink=%.6e, MATLAB-replay=%.6e, ratio=%.6f, dB=%.4f\n', ...
            pSim4570, pRep4570, pSim4570/pRep4570, 10*log10(pSim4570/pRep4570));
    end

    results.(name) = struct('maxAbsA', maxAbsA, 'rmsA', rmsA, 'maxAbsG', maxAbsG, 'rmsG', rmsG, ...
        'firstMismatchA', firstMismatchA, 'optimalLag', optimalLag);
end

fprintf('\n\n========== SUMMARY ==========\n');
for c = 1:numel(cases)
    name = cases(c).name;
    r = results.(name);
    fprintf('%-16s max|accelDiff|=%.3e  max|gyroDiff|=%.3e  firstMismatch=%s  optimalLag=%s\n', ...
        name, max(r.maxAbsA), max(r.maxAbsG), mat2str(r.firstMismatchA), mat2str(r.optimalLag));
end
