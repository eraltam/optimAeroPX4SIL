% PLAN section 11.5: compare calibrated gyro noise PSD and Allan deviation against the
% pre-calibration production baseline (ANELLO_X3_IMU_fcn_SIL_OLD_baseline.m, the last-committed
% version before the SiPhOG calibration integration), so the deterministic calibration work is
% shown not to have silently altered the validated noise model. Methodology matches this
% project's own prior 8-test suite (see newIMU_model/CLAUDE.md Tests 5-6): pwelch + allanvar,
% zero-input (truth=0), fixed seed, Ts=0.004s (250 Hz, this SIL's actual rate).

clear ANELLO_X3_IMU_fcn_SIL ANELLO_X3_IMU_fcn_SIL_OLD_baseline;
Ts = 0.004; fs = 1/Ts;
N = 150000;   % 600 s, matching the original 8-test suite's duration
r2d = 180/pi;

accel_true = [0;0;0]; gyro_true = [0;0;0]; mag_true = [0.2;0;0.4];
TcountsMid = 0;

rng(11);
[~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline([], [], [], 0, 1);
gyroOld = zeros(3,N);
for k = 1:N
    [~, gyroOld(:,k)] = ANELLO_X3_IMU_fcn_SIL_OLD_baseline(accel_true, gyro_true, mag_true, 25.0, 0);
end

rng(11);
[~,~,~,~,~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL([], [], [], 0, 0, uint8(2), 1);
gyroNew = zeros(3,N);
for k = 1:N
    [~, gyroNew(:,k)] = ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, 25.0, TcountsMid, uint8(2), 0);
end

axisNames = {'X','Y','Z'};
fprintf('=== PSD comparison (Welch, 2048-pt Hamming, 50%% overlap) ===\n');
fig1 = figure('Visible','off','Position',[100 100 900 700]);
tl1 = tiledlayout(fig1, 3, 1);
title(tl1, 'ANELLO X3 SiPhOG gyro noise PSD: production baseline vs calibration-integrated');

psdResults = struct();
for a = 1:3
    [pOld, fOld] = pwelch(gyroOld(a,:), hamming(2048), 1024, 4096, fs);
    [pNew, fNew] = pwelch(gyroNew(a,:), hamming(2048), 1024, 4096, fs);
    NOld_radps_sqrtHz = sqrt(mean(pOld(f_in_band(fOld,1,50))));
    NNew_radps_sqrtHz = sqrt(mean(pNew(f_in_band(fNew,1,50))));
    ARWold_dpsqrthr = NOld_radps_sqrtHz * r2d * sqrt(3600);
    ARWnew_dpsqrthr = NNew_radps_sqrtHz * r2d * sqrt(3600);
    fprintf('  Axis %s: ARW-equiv old=%.5f new=%.5f deg/sqrt(hr)  ratio=%.4f\n', ...
        axisNames{a}, ARWold_dpsqrthr, ARWnew_dpsqrthr, ARWnew_dpsqrthr/ARWold_dpsqrthr);

    ax = nexttile(tl1);
    loglog(ax, fOld, sqrt(pOld), 'b-'); hold(ax,'on');
    loglog(ax, fNew, sqrt(pNew), 'r--');
    ylabel(ax, sprintf('%s [rad/s/sqrt(Hz)]', axisNames{a}));
    if a==1; legend(ax,'old (production baseline)','new (calibration-integrated)','Location','southwest'); end
    grid(ax,'on');

    psdResults.(['axis' axisNames{a}]) = struct('ARWold_dpsqrthr', ARWold_dpsqrthr, ...
        'ARWnew_dpsqrthr', ARWnew_dpsqrthr, 'ratio', ARWnew_dpsqrthr/ARWold_dpsqrthr);
end
xlabel(tl1.Children(1), 'Frequency [Hz]');
psdPlotPath = fullfile(fileparts(mfilename('fullpath')), 'psd_comparison_old_vs_new.png');
exportgraphics(fig1, psdPlotPath, 'Resolution', 150);
close(fig1);
fprintf('PSD plot saved: %s\n', psdPlotPath);

fprintf('\n=== Allan deviation comparison ===\n');
tau0 = Ts;
fig2 = figure('Visible','off','Position',[100 100 900 700]);
tl2 = tiledlayout(fig2, 3, 1);
title(tl2, 'ANELLO X3 SiPhOG gyro Allan deviation: production baseline vs calibration-integrated');

allanResults = struct();
for a = 1:3
    thetaOld = cumsum(gyroOld(a,:)) * Ts;
    thetaNew = cumsum(gyroNew(a,:)) * Ts;
    [avarOld, tauOld] = allanvar(thetaOld, 'octave', 1/Ts);
    [avarNew, tauNew] = allanvar(thetaNew, 'octave', 1/Ts);
    sigmaOld = sqrt(avarOld); sigmaNew = sqrt(avarNew);

    idx1s_old = find(abs(tauOld-1) == min(abs(tauOld-1)), 1);
    idx1s_new = find(abs(tauNew-1) == min(abs(tauNew-1)), 1);
    arwOld_1s_dpsqrthr = sigmaOld(idx1s_old) * r2d * 60;
    arwNew_1s_dpsqrthr = sigmaNew(idx1s_new) * r2d * 60;
    fprintf('  Axis %s: Allan @ tau=1s, old=%.5f new=%.5f deg/sqrt(hr)  ratio=%.4f\n', ...
        axisNames{a}, arwOld_1s_dpsqrthr, arwNew_1s_dpsqrthr, arwNew_1s_dpsqrthr/arwOld_1s_dpsqrthr);

    ax = nexttile(tl2);
    loglog(ax, tauOld, sigmaOld, 'bo-'); hold(ax,'on');
    loglog(ax, tauNew, sigmaNew, 'rs--');
    ylabel(ax, sprintf('%s [rad/s]', axisNames{a}));
    if a==1; legend(ax,'old (production baseline)','new (calibration-integrated)','Location','southwest'); end
    grid(ax,'on');

    allanResults.(['axis' axisNames{a}]) = struct('ARWold_1s_dpsqrthr', arwOld_1s_dpsqrthr, ...
        'ARWnew_1s_dpsqrthr', arwNew_1s_dpsqrthr, 'ratio', arwNew_1s_dpsqrthr/arwOld_1s_dpsqrthr);
end
xlabel(tl2.Children(1), 'tau [s]');
allanPlotPath = fullfile(fileparts(mfilename('fullpath')), 'allan_comparison_old_vs_new.png');
exportgraphics(fig2, allanPlotPath, 'Resolution', 150);
close(fig2);
fprintf('Allan plot saved: %s\n', allanPlotPath);

save(fullfile(fileparts(mfilename('fullpath')), 'anelloX3SiPhOGNoisePSDAllanResults.mat'), ...
    'psdResults', 'allanResults');
fprintf('\nResults saved: anelloX3SiPhOGNoisePSDAllanResults.mat\n');

function mask = f_in_band(f, lo, hi)
mask = f >= lo & f <= hi;
end
