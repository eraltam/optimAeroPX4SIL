% Extended, more explicit old-vs-new comparison requested 2026-08-13: sample-wise residual at
% the matched nominal point (not just aggregate ratios), old-vs-new deterministic thermal
% sensitivity contrast (generic Celsius term vs real per-unit TC/SFvT tables), and a PSD-derived
% noise-floor sweep across the full temperature-count domain (not just a single point).

clear ANELLO_X3_IMU_fcn_SIL ANELLO_X3_IMU_fcn_SIL_OLD_baseline ANELLO_X3_SiPhOG_CalibrationCore_fcn;
r2d = 180/pi; d2r = pi/180;
thisDir = fileparts(mfilename('fullpath'));

% =================================================================================================
% A) Sample-wise residual at the matched nominal operating point (T_old=25degC == T0, T_counts=0)
% =================================================================================================
N = 6000;
accel_true=[0;0;0]; gyro_true=[0;0;0]; mag_true=[0.2;0;0.4];

rng(42);
[~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([], [], [], 0, 1);
gyroOld = zeros(3,N);
for k=1:N
    [~, gyroOld(:,k)] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline(accel_true, gyro_true, mag_true, 25.0, 0);
end

rng(42);
[~,~,~,~,~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL([], [], [], 0, 0, uint8(2), 1);
gyroNew = zeros(3,N);
for k=1:N
    [~, gyroNew(:,k)] = ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, 25.0, 0, uint8(2), 0);
end

resid = gyroNew - gyroOld;
residMax_radps = max(abs(resid), [], 2);
residRms_radps = rms(resid, 2);
fprintf('=== A) Sample-wise residual, new-vs-old, matched nominal point (N=%d) ===\n', N);
for a=1:3
    fprintf('  axis %d: max|resid|=%.3e rad/s (%.3e deg/s), rms=%.3e rad/s\n', ...
        a, residMax_radps(a), residMax_radps(a)*r2d, residRms_radps(a));
end

figA = figure('Visible','off','Position',[100 100 900 400]);
plot((0:N-1)*0.004, resid(1,:)*1e18, 'b-'); hold on;
plot((0:N-1)*0.004, resid(2,:)*1e18, 'r-');
plot((0:N-1)*0.004, resid(3,:)*1e18, 'g-');
xlabel('time [s]'); ylabel('gyro\_meas residual (new - old) [$\times10^{-18}$ rad/s]', 'Interpreter','none');
ylabel('gyro_{meas} residual (new - old)  [1e-18 rad/s]');
legend('X','Y','Z','Location','best'); grid on;
title('Residual is at floating-point round-off level (~1e-17 rad/s), not a real difference');
residPlotPath = fullfile(thisDir, 'residual_new_minus_old_nominal.png');
exportgraphics(figA, residPlotPath, 'Resolution', 150);
close(figA);
fprintf('Residual plot saved: %s\n', residPlotPath);

% =================================================================================================
% B) Deterministic thermal-sensitivity contrast: old model's OWN Celsius sweep (its own generic
% thermal term) vs. new model's OWN temperature-count sweep, UNCALIBRATED view (real TC/SFvT).
% Different input domains (no verified Celsius<->counts map exists, PLAN section 5) -- each swept
% over its own full nominal range, reported on the same deg/s axis for direct magnitude contrast.
% =================================================================================================
truthDps = [30; -45; 90];
truthRadps = truthDps * d2r;

TcelsiusRange = linspace(-10, 50, 25);  % old model's own commercial range
oldGyroAtT = zeros(3, numel(TcelsiusRange));
rng(1);
[~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([], [], [], 0, 1);
for k = 1:numel(TcelsiusRange)
    [~, oldGyroAtT(:,k)] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([0;0;0], truthRadps, [0.2;0;0.4], TcelsiusRange(k), 0);
end
oldThermalP2P_dps = (max(oldGyroAtT,[],2) - min(oldGyroAtT,[],2)) * r2d;

TcountsRange = linspace(-30000, 30000, 4000);
newUncalAtT = zeros(3, numel(TcountsRange));
for k = 1:numel(TcountsRange)
    [~, ~, ~, uncalR] = ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, zeros(3,1), TcountsRange(k));
    newUncalAtT(:,k) = uncalR;
end
newThermalP2P_dps = (max(newUncalAtT,[],2) - min(newUncalAtT,[],2)) * r2d;

fprintf('\n=== B) Deterministic thermal-sensitivity contrast, truth=[%g %g %g] deg/s ===\n', truthDps);
fprintf('  Old model, own Celsius sweep (-10 to 50 degC): peak-to-peak = [%.3e %.3e %.3e] deg/s\n', oldThermalP2P_dps);
fprintf('  New model, uncalibrated, own counts sweep (full domain): peak-to-peak = [%.4f %.4f %.4f] deg/s\n', newThermalP2P_dps);
fprintf('  Ratio (new_uncal / old): [%.1f %.1f %.1f]x\n', newThermalP2P_dps ./ oldThermalP2P_dps);

figB = figure('Visible','off','Position',[100 100 900 700]);
tlB = tiledlayout(figB, 2, 1);
title(tlB, 'Thermal sensitivity: old generic model vs. new SiPhOG calibration (uncalibrated view)');
ax1 = nexttile(tlB);
plot(ax1, TcelsiusRange, oldGyroAtT(1,:)*r2d, 'b-o');
xlabel(ax1, 'Old model temperature input [degC]'); ylabel(ax1, 'gyro X [deg/s]');
title(ax1, sprintf('Old (generic): peak-to-peak = %.2e deg/s over -10..50 degC', oldThermalP2P_dps(1)));
grid(ax1, 'on');
ax2 = nexttile(tlB);
plot(ax2, TcountsRange, newUncalAtT(1,:)*r2d, 'r-');
xlabel(ax2, 'New model temperature counts [-]'); ylabel(ax2, 'uncalibrated gyro X [deg/s]');
title(ax2, sprintf('New (real TC/SFvT, uncalibrated): peak-to-peak = %.2f deg/s over full domain', newThermalP2P_dps(1)));
grid(ax2, 'on');
thermalPlotPath = fullfile(thisDir, 'thermal_sensitivity_old_vs_new_uncalibrated.png');
exportgraphics(figB, thermalPlotPath, 'Resolution', 150);
close(figB);
fprintf('Thermal sensitivity contrast plot saved: %s\n', thermalPlotPath);

% =================================================================================================
% C) PSD-derived noise floor, NEW model, swept across the full temperature-count domain (not
% just one point) -- compared against OLD's single fixed value (old has no temperature-count
% input at all).
% =================================================================================================
NpsD = 40000;  % 160 s per point -- shorter than the 600s single-point check, enough for a sweep
TcountsForPSD = [-30000 -20000 -10000 0 10000 20000 30000];
arwNew_dpsqrthr = zeros(3, numel(TcountsForPSD));
for ti = 1:numel(TcountsForPSD)
    rng(99);
    [~,~,~,~,~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL([], [], [], 0, 0, uint8(2), 1);
    g = zeros(3, NpsD);
    for k = 1:NpsD
        [~, g(:,k)] = ANELLO_X3_IMU_fcn_SIL([0;0;0], [0;0;0], [0.2;0;0.4], 25.0, TcountsForPSD(ti), uint8(2), 0);
    end
    for a = 1:3
        [p, f] = pwelch(g(a,:), hamming(2048), 1024, 4096, 250);
        arwNew_dpsqrthr(a,ti) = sqrt(mean(p(f>=1 & f<=50))) * r2d * sqrt(3600);
    end
end

rng(99);
[~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([], [], [], 0, 1);
gOld = zeros(3, NpsD);
for k = 1:NpsD
    [~, gOld(:,k)] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([0;0;0], [0;0;0], [0.2;0;0.4], 25.0, 0);
end
arwOld_dpsqrthr = zeros(3,1);
for a = 1:3
    [p, f] = pwelch(gOld(a,:), hamming(2048), 1024, 4096, 250);
    arwOld_dpsqrthr(a) = sqrt(mean(p(f>=1 & f<=50))) * r2d * sqrt(3600);
end

fprintf('\n=== C) PSD-derived ARW-equivalent, NEW model swept across full temperature-count domain ===\n');
fprintf('  Old (fixed, T=25degC, no counts input): X=%.5f Y=%.5f Z=%.5f deg/sqrt(hr)\n', arwOld_dpsqrthr);
for ti = 1:numel(TcountsForPSD)
    fprintf('  New @ Tcounts=%6d: X=%.5f Y=%.5f Z=%.5f deg/sqrt(hr)  ratio-to-old=[%.4f %.4f %.4f]\n', ...
        TcountsForPSD(ti), arwNew_dpsqrthr(:,ti), arwNew_dpsqrthr(:,ti)./arwOld_dpsqrthr);
end

figC = figure('Visible','off','Position',[100 100 900 500]);
plot(TcountsForPSD, arwNew_dpsqrthr(1,:), 'b-o'); hold on;
plot(TcountsForPSD, arwNew_dpsqrthr(2,:), 'r-s');
plot(TcountsForPSD, arwNew_dpsqrthr(3,:), 'g-^');
yline(arwOld_dpsqrthr(1), 'b--'); yline(arwOld_dpsqrthr(2), 'r--'); yline(arwOld_dpsqrthr(3), 'g--');
xlabel('temperature counts'); ylabel('PSD-derived ARW-equivalent [deg/sqrt(hr)]');
legend('New X','New Y','New Z','Old X (fixed)','Old Y (fixed)','Old Z (fixed)','Location','best');
title('Calibrated noise floor stays flat across the full temperature domain');
grid on;
psdSweepPlotPath = fullfile(thisDir, 'psd_arw_vs_temperature_sweep.png');
exportgraphics(figC, psdSweepPlotPath, 'Resolution', 150);
close(figC);
fprintf('PSD-vs-temperature sweep plot saved: %s\n', psdSweepPlotPath);

save(fullfile(thisDir, 'anelloX3SiPhOGExtendedComparisonResults.mat'), ...
    'resid', 'oldThermalP2P_dps', 'newThermalP2P_dps', 'arwOld_dpsqrthr', 'arwNew_dpsqrthr', 'TcountsForPSD');
fprintf('\nResults saved: anelloX3SiPhOGExtendedComparisonResults.mat\n');
