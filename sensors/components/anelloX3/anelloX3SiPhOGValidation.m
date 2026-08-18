% Phase E validation script (PLAN_ANELLO_X3_SIPHOG_TEMPERATURE_CALIBRATION_OUTPUT_MODES.md
% section 12). Reproducible: fixed seed, fixed temperature profile, no wall-clock/random
% dependence other than the explicit rng() seed below.
%
% Noise-off cases call ANELLO_X3_SiPhOG_CalibrationCore_fcn.m directly with a zero noise
% vector -- this is the "test-only configuration that does not alter production defaults"
% required by PLAN section 11.4 (it bypasses ANELLO_X3_IMU_fcn_SIL.m's stochastic recursions
% entirely rather than adding any noise-disable flag to production code).
% Noise-on cases call ANELLO_X3_IMU_fcn_SIL.m itself (production path, outputMode=CALIBRATED
% and =UNCALIBRATED as needed) so the reported noise-on numbers reflect the real integrated
% stochastic model, not a re-implementation of it.

clear ANELLO_X3_IMU_fcn_SIL ANELLO_X3_SiPhOG_CalibrationCore_fcn;
r2d = 180/pi; d2r = pi/180;

N = 4000;
Tcounts = linspace(-30000, 30000, N);   % deliberate full-range sweep, not a physical thermal rate

cases = struct('name', {'case_90dps_singleaxis', 'case_combined_30_m45_90'}, ...
               'truthDps', {[90;0;0], [30;-45;90]});

results = struct();

for c = 1:numel(cases)
    truthDps = cases(c).truthDps;
    truthRadps = truthDps * d2r;

    % ---- noise OFF: direct calibration-core calls, zero noise -------------------------------
    calDps_off   = zeros(3,N);
    uncalDps_off = zeros(3,N);
    icClampCount_off = 0; tempClampCount_off = 0; satCount_off = 0;
    for k = 1:N
        [rawC, ~, ~, uncalR, calR, ~, ~, satF, tempF, icF] = ...
            ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, zeros(3,1), Tcounts(k));
        calDps_off(:,k) = calR * r2d;
        uncalDps_off(:,k) = uncalR * r2d;
        icClampCount_off = icClampCount_off + sum(icF);
        tempClampCount_off = tempClampCount_off + sum(tempF);
        satCount_off = satCount_off + sum(satF);
    end
    calErr_off = calDps_off - truthDps;
    maxErr_off = max(abs(calErr_off), [], 2);
    rmsErr_off = sqrt(mean(calErr_off.^2, 2));
    uncalP2P_off = max(uncalDps_off, [], 2) - min(uncalDps_off, [], 2);

    % ---- noise ON: production path, seeded --------------------------------------------------
    rng(20260812);
    calDps_on   = zeros(3,N);
    [~,~,~,~,~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL([], [], [], 0, 0, uint8(2), 1);
    for k = 1:N
        [~, gMeas] = ANELLO_X3_IMU_fcn_SIL([0;0;0], truthRadps, [0.2;0;0.4], 25.0, Tcounts(k), uint8(2), 0);
        calDps_on(:,k) = gMeas * r2d;
    end
    calErrOn = calDps_on - truthDps;
    meanErrOn = mean(calErrOn, 2);
    stdOn = std(calDps_on, 0, 2);

    rng(20260812);
    noiseRms = zeros(3,1);
    [~,~,~,~,~,~,~,~,~,~] = ANELLO_X3_IMU_fcn_SIL([], [], [], 0, 0, uint8(2), 1);
    gbiasSeries = zeros(3,N);
    for k = 1:N
        [~, ~, ~, ~, gb] = ANELLO_X3_IMU_fcn_SIL([0;0;0], truthRadps, [0.2;0;0.4], 25.0, Tcounts(k), uint8(2), 0);
        gbiasSeries(:,k) = gb;
    end
    noiseRms = sqrt(mean(gbiasSeries.^2, 2)) * r2d;   % bias+pink+RRW component RMS, deg/s

    results.(cases(c).name) = struct( ...
        'maxErr_noiseOff_degps', maxErr_off, 'rmsErr_noiseOff_degps', rmsErr_off, ...
        'uncalP2P_degps', uncalP2P_off, ...
        'meanErr_noiseOn_degps', meanErrOn, 'stdErr_noiseOn_degps', stdOn, ...
        'biasPinkRrwRms_degps', noiseRms, ...
        'icClamps', icClampCount_off, 'tempClamps', tempClampCount_off, 'countSaturations', satCount_off);

    fprintf('\n=== Case: %s  (truth = [%g %g %g] deg/s) ===\n', cases(c).name, truthDps);
    fprintf('Noise OFF: max err=[%.4f %.4f %.4f] deg/s, rms err=[%.4f %.4f %.4f] deg/s\n', ...
        maxErr_off, rmsErr_off);
    fprintf('Noise OFF: uncalibrated peak-to-peak (thermal movement)=[%.3f %.3f %.3f] deg/s\n', uncalP2P_off);
    fprintf('Noise ON:  mean err=[%+.4f %+.4f %+.4f] deg/s, std=[%.4f %.4f %.4f] deg/s\n', ...
        meanErrOn, stdOn);
    fprintf('Noise ON:  bias+pink+RRW component RMS=[%.4e %.4e %.4e] deg/s\n', noiseRms);
    fprintf('Clamp/saturation counts over %d samples: IC=%d, temp=%d, count-sat=%d\n', ...
        N, icClampCount_off, tempClampCount_off, satCount_off);

    % ---- plot -------------------------------------------------------------------------------
    fig = figure('Visible','off','Position',[100 100 1000 700]);
    tl = tiledlayout(fig, 3, 1);
    title(tl, sprintf('ANELLO X3 SiPhOG calibration validation -- %s', strrep(cases(c).name,'_',' ')));

    ax1 = nexttile(tl);
    hold(ax1,'on');
    plot(ax1, 1:N, repmat(truthDps(1),1,N), 'k--', 'LineWidth', 1);
    plot(ax1, 1:N, uncalDps_off(1,:), 'r-');
    plot(ax1, 1:N, calDps_off(1,:), 'b-');
    ylabel(ax1, 'X [deg/s]'); legend(ax1, 'truth','uncalibrated','calibrated','Location','best');
    title(ax1, 'Noise-off temperature sweep: truth vs uncalibrated vs calibrated');

    ax2 = nexttile(tl);
    hold(ax2,'on');
    plot(ax2, 1:N, repmat(truthDps(2),1,N), 'k--', 'LineWidth', 1);
    plot(ax2, 1:N, uncalDps_off(2,:), 'r-');
    plot(ax2, 1:N, calDps_off(2,:), 'b-');
    ylabel(ax2, 'Y [deg/s]');

    ax3 = nexttile(tl);
    plot(ax3, 1:N, Tcounts, 'm-');
    ylabel(ax3, 'temperature counts'); xlabel(ax3, 'sample');

    plotPath = fullfile(fileparts(mfilename('fullpath')), sprintf('validation_plot_%s.png', cases(c).name));
    exportgraphics(fig, plotPath, 'Resolution', 150);
    close(fig);
    fprintf('Plot saved: %s\n', plotPath);
end

save(fullfile(fileparts(mfilename('fullpath')), 'anelloX3SiPhOGValidationResults.mat'), 'results');
fprintf('\nResults saved: anelloX3SiPhOGValidationResults.mat\n');
