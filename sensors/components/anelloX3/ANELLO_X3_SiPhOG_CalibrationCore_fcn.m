function [raw_counts_by_fog, tc_corrected_counts_by_fog, ideal_counts_by_fog, ...
          omega_uncal_body_radps, omega_cal_body_radps, ...
          sfvt_by_fog, tc_bias_counts_by_fog, ...
          count_saturation_flags, temp_clamp_flags, ic_clamp_flags] = ...
          ANELLO_X3_SiPhOG_CalibrationCore_fcn(omega_true_body_radps, noise_total_body_radps, temperature_counts)
%ANELLO_X3_SIPHOG_CALIBRATIONCORE_FCN  One physical FOG sample -> raw/uncalibrated/calibrated
%views, per PLAN_ANELLO_X3_SIPHOG_TEMPERATURE_CALIBRATION_OUTPUT_MODES.md sections 4 and 6.3.
%
% Generates exactly ONE physical observation per call from the supplied truth + existing
% (already-realized) stochastic noise, then derives both engineering views from that single
% raw sample. Performs NO random draws and updates NO persistent state -- callers own the
% noise realization (white+pink+RRW+bias-instability, already summed) and pass it in once per
% timestep, so selecting/recomputing a view here can never change the noise draw for that
% sample (PLAN section 1's non-negotiable design rule).
%
% Calibration data (IC/TC/SFvT LUTs, sensAxes) is INLINED as local constants rather than read
% from anelloX3SiPhOGCalibrationData_SN261200001124() -- that function returns a large nested
% struct, which fails to compile inside a MATLAB Function block in this MATLAB release (see
% optimAeroPX4SIL/CLAUDE.md section 3, and PLAN section 6.3's 2026-08-12 correction). Keep the
% two files in sync by hand if the calibration archive is ever regenerated; parity between them
% is checked by anelloX3SiPhOGCalibrationImporterTest.m against the raw archive, not against
% this file directly, so a manual diff against anelloX3SiPhOGCalibrationData_SN261200001124.m
% after any edit here is required.
%
% SFvT-to-FOG mapping (source JSON labelled x/y/z -> FOG1/FOG2/FOG3) and the no-separate-
% Earth-rate-injection assumption are ACCEPTED DEFAULTS (PLAN Section 9 gates 2 and 7), not
% independently vendor-confirmed -- see the plan document for the open-exception record.
%
% Inputs:
%   omega_true_body_radps   [3x1]  true angular rate, body frame [rad/s]
%   noise_total_body_radps  [3x1]  existing stochastic realization for this sample (white +
%                                   pink + RRW + bias-instability, already summed), body frame,
%                                   same units/frame as omega_true_body_radps [rad/s]
%   temperature_counts      [1x1]  SiPhOG internal temperature, in raw temperature counts
%                                   (NOT Celsius -- see PLAN section 5)
%
% Outputs (all debug/telemetry are flat vectors, never a struct -- see header note above):
%   raw_counts_by_fog          [3x1]  modeled raw FOG counts (fractional, pre-saturation-clamp
%                                      value already applied); FOG-indexed, NOT body-axis
%   tc_corrected_counts_by_fog [3x1]  raw_counts_by_fog with TC bias removed; FOG-indexed
%   ideal_counts_by_fog        [3x1]  inverse-IC counts before TC bias was added; FOG-indexed
%   omega_uncal_body_radps     [3x1]  uncalibrated engineering rate, body frame [rad/s]
%   omega_cal_body_radps       [3x1]  calibrated engineering rate, body frame [rad/s]
%   sfvt_by_fog                [3x1]  SFvT scale-factor correction used; FOG-indexed
%   tc_bias_counts_by_fog      [3x1]  TC bias (counts) used; FOG-indexed
%   count_saturation_flags     [3x1]  logical, raw counts hit signed-24-bit range; FOG-indexed
%   temp_clamp_flags           [3x1]  logical, temperature_counts outside TC or SFvT LUT domain
%   ic_clamp_flags              [3x1]  logical, any IC forward/inverse lookup clamped this sample
%
%#codegen

r2d = 180.0 / pi;

% ---- FOG sensor axes (X3 sensor frame; ACCEPTED to equal body frame for this identity mount,
% PLAN section 9 gate 8) -----------------------------------------------------------------------
U_fog = [-0.99997207, -0.00116392, -0.00738314; ...
         -0.00397688, -0.00283814,  0.99998806; ...
          0.00874953, -0.99995686,  0.00311948];

% ---- IC-Rate LUTs: counts -> scaled output [deg/s] --------------------------------------------
ic_counts_fog1 = [-8388608.0, -4766443.0, -4455140.0, -4175222.0, -3897225.0, -3636872.0, -3363393.0, -3102073.0, -2834047.0, -2574818.0, -2299121.0, -2029384.0, -1743713.0, -1460761.0, -1148133.0, -980589.0, -830036.0, -633350.0, -457018.0, -212362.0, 0.0, 12015.0, 203328.0, 436665.0, 612531.0, 807426.0, 958194.0, 1125208.0, 1274968.0, 1438089.0, 1721118.0, 2007076.0, 2277642.0, 2554339.0, 2813910.0, 3082529.0, 3344790.0, 3619629.0, 3881503.0, 4161844.0, 4444892.0, 4761968.0, 8388607.0];
ic_rate_fog1   = [-726.015729, -406.604395, -379.152938, -354.564685, -330.247351, -307.573334, -283.865239, -261.317789, -238.299818, -216.139338, -192.676771, -169.820636, -145.712526, -121.922711, -95.725711, -81.718041, -69.146687, -52.741770, -38.048508, -17.676516, 0.000000, 1.000073, 16.925598, 36.358357, 51.016455, 67.277314, 79.871616, 93.841408, 106.386784, 120.074106, 143.886143, 168.036690, 190.982317, 214.551551, 236.762861, 259.855072, 282.507160, 306.358139, 329.188423, 353.735805, 378.623186, 406.608734, 726.700703];

ic_counts_fog2 = [-8388608.0, -4766443.0, -4481665.0, -4214697.0, -3945041.0, -3688058.0, -3416919.0, -3155494.0, -2885717.0, -2623540.0, -2344802.0, -2071535.0, -1781052.0, -1493322.0, -1173707.0, -849062.0, -646340.0, -466504.0, -212257.0, 0.0, 11620.0, 203437.0, 443603.0, 622852.0, 823387.0, 976616.0, 1147635.0, 1467536.0, 1755382.0, 2046279.0, 2320581.0, 2600610.0, 2863408.0, 3134159.0, 3396936.0, 3669934.0, 3928999.0, 4201545.0, 4472030.0, 4761968.0, 8388607.0];
ic_rate_fog2   = [-750.473931, -419.999932, -394.017757, -369.770480, -345.392516, -322.269188, -297.988381, -274.689703, -250.759589, -227.609446, -203.105313, -179.182992, -153.853083, -128.852765, -101.170738, -73.129601, -55.649314, -40.156396, -18.267643, 0.000000, 1.000040, 17.509681, 38.189790, 53.636138, 70.933693, 84.165907, 98.952898, 126.675369, 151.702589, 177.087919, 201.121026, 225.761036, 248.988229, 273.028366, 296.470869, 320.942449, 344.275809, 368.938051, 393.526171, 419.999822, 751.140868];

ic_counts_fog3 = [-8388608.0, -4766443.0, -4465636.0, -4190022.0, -3915030.0, -3655294.0, -3382983.0, -3121398.0, -2852891.0, -2592142.0, -2316198.0, -2045345.0, -1758550.0, -1473571.0, -1310145.0, -1159731.0, -991498.0, -839269.0, -642332.0, -464336.0, -219210.0, 0.0, 206564.0, 438901.0, 616084.0, 810897.0, 963125.0, 1130632.0, 1281307.0, 1444616.0, 1729439.0, 2016477.0, 2288239.0, 2565434.0, 2826530.0, 3095844.0, 3358695.0, 3632970.0, 3894852.0, 4173244.0, 4453319.0, 4761968.0, 8388607.0];
ic_rate_fog3   = [-716.835820, -401.014901, -374.787177, -350.859941, -327.096138, -304.757339, -281.452061, -259.176905, -236.425691, -214.438921, -191.280746, -168.652822, -144.794806, -121.180103, -107.674340, -95.264582, -81.405436, -68.881084, -52.697534, -38.084919, -17.976093, 0.000000, 16.940381, 36.004017, 50.554338, 66.569716, 79.099987, 92.906953, 105.345989, 118.851405, 142.471488, 166.370518, 189.096016, 212.382939, 234.422356, 257.266466, 279.673336, 303.171166, 325.716850, 349.796403, 374.130344, 401.059119, 717.473383];

% ---- TC LUTs: temperature counts -> count OFFSET ----------------------------------------------
tc_temp_fog1   = [-32768, -10885, -9960, -8879, -2757, 4387, 6811, 7753, 8595, 9595, 10503, 11342, 12125, 12866, 13221, 13568, 13906, 14237, 14560, 14877, 32767];
tc_offset_fog1 = [-1216, -464, -432, -393, -161, 102, 188, 219, 245, 272, 292, 306, 314, 316, 315, 313, 308, 303, 295, 287, -209];

tc_temp_fog2   = [-32768, -10885, -10635, -10381, -9858, -9312, -8741, -8142, -7509, -6834, -6107, -5370, -4555, -3635, -2544, -675, 1624, 3110, 4333, 5182, 5970, 6713, 7421, 8104, 8765, 9407, 10035, 10657, 11270, 11876, 12477, 13075, 13673, 14273, 14877, 32767];
tc_offset_fog2 = [-3599, -1303, -1277, -1252, -1204, -1159, -1116, -1077, -1039, -1003, -969, -937, -905, -871, -833, -771, -697, -651, -615, -592, -572, -555, -542, -531, -522, -516, -513, -512, -513, -518, -524, -534, -546, -561, -578, -1095];

tc_temp_fog3   = [-32768, -10885, -10574, -10252, -9918, -9570, -9208, -8827, -8424, -7994, -7414, -6747, -6114, -4409, -3432, -2520, -1624, -671, 355, 1311, 3518, 4608, 5638, 6606, 7106, 7605, 8110, 8629, 9609, 11752, 12309, 12798, 13384, 13917, 14412, 14648, 14877, 32767];
tc_offset_fog3 = [-2905, -455, -420, -388, -360, -334, -312, -292, -275, -260, -244, -230, -220, -195, -178, -157, -132, -101, -62, -23, 72, 116, 153, 182, 194, 205, 214, 222, 234, 252, 259, 268, 283, 302, 325, 339, 354, 1518];

% ---- SFvT LUTs: temperature counts -> dimensionless scale factor ------------------------------
% Mapping x->FOG1, z->FOG2, y->FOG3 (ACCEPTED DEFAULT, PLAN section 3.4/9 gate 2).
sfvt_temp_fog1  = [-32768.0, -9432.931, -7792.016142857143, -6151.101285714286, -4510.186428571429, -2869.2715714285714, -1228.3567142857137, 412.55814285714223, 2053.473, 3694.3878571428577, 5335.302714285715, 6976.217571428573, 8617.132428571429, 10258.047285714285, 11898.962142857144, 13539.877, 32767.0];
sfvt_scale_fog1 = [0.9535100737928205, 0.9807916920073635, 0.9827101269884275, 0.9848639367423169, 0.987273893638942, 0.989943171304295, 0.9928609266554301, 0.9960058819354461, 0.9993499067484674, 1.0028616000946244, 1.0065098724050372, 1.0102675275767943, 1.0141148450079358, 1.0180431616324344, 1.0220584539551765, 1.026184920086944, 1.0745360405155648];

sfvt_temp_fog2  = [-32768.0, -9433.076, -7828.784142857142, -6224.492285714285, -4620.200428571428, -3015.908571428571, -1411.616714285714, 192.67514285714242, 1796.9670000000006, 3401.258857142857, 5005.550714285713, 6609.842571428571, 8214.134428571428, 9818.426285714284, 11422.71814285714, 13027.01, 32767.0];
sfvt_scale_fog2 = [0.9699282724239136, 0.9866266012433289, 0.9877746226790483, 0.9890168955127379, 0.9904405054146443, 0.9921084058876565, 0.9940615513100122, 0.9963210299780052, 0.9988901971486918, 1.0017568080825978, 1.004895151086425, 1.0082681805557583, 1.0118296500177726, 1.0155262451739386, 1.0192997169427316, 1.023089014502336, 1.0697143810779846];

sfvt_temp_fog3  = [-32768.0, -9498.04, -7862.088571428572, -6226.137142857144, -4590.185714285715, -2954.2342857142867, -1318.2828571428581, 317.66857142857043, 1953.619999999999, 3589.5714285714275, 5225.522857142856, 6861.474285714285, 8497.425714285713, 10133.377142857142, 11769.32857142857, 13405.28, 32767.0];
sfvt_scale_fog3 = [0.9536225451564675, 0.9799835205392717, 0.981836780078009, 0.9838841305160189, 0.9862494525744286, 0.9889807174476928, 0.9920671611955049, 0.9954564591347103, 0.9990719002312186, 1.0028295614919156, 1.0066554823565759, 1.0105028390897752, 1.0143691191728026, 1.018313295695573, 1.0224730017485395, 1.0270817048146053, 1.081626367186536];

rawCountMin = -8388608.0;
rawCountMax =  8388607.0;

% ---- Project truth + existing noise onto FOG axes (deg/s) -------------------------------------
q_dps = U_fog * (omega_true_body_radps .* r2d);
n_dps = U_fog * (noise_total_body_radps .* r2d);
q_noisy_dps = q_dps + n_dps;

raw_counts_by_fog          = zeros(3,1);
tc_corrected_counts_by_fog = zeros(3,1);
ideal_counts_by_fog        = zeros(3,1);
q_uncal_dps                = zeros(3,1);
q_cal_dps                  = zeros(3,1);
sfvt_by_fog                = zeros(3,1);
tc_bias_counts_by_fog      = zeros(3,1);
count_saturation_flags     = false(3,1);
temp_clamp_flags           = false(3,1);
ic_clamp_flags             = false(3,1);

% ==== FOG 1 =====================================================================================
[sfvt_by_fog(1), sfvtClamp1] = anelloInterp1Clamped(sfvt_temp_fog1, sfvt_scale_fog1, temperature_counts);
[tc_bias_counts_by_fog(1), tcClamp1] = anelloInterp1Clamped(tc_temp_fog1, tc_offset_fog1, temperature_counts);
temp_clamp_flags(1) = sfvtClamp1 || tcClamp1;

rate_before_sfvt_1 = q_noisy_dps(1) / sfvt_by_fog(1);
[ideal_counts_by_fog(1), icClampInv1] = anelloInverseMonotonicLUT(ic_counts_fog1, ic_rate_fog1, rate_before_sfvt_1);
rawUnclamped1 = ideal_counts_by_fog(1) + tc_bias_counts_by_fog(1);
count_saturation_flags(1) = (rawUnclamped1 < rawCountMin) || (rawUnclamped1 > rawCountMax);
raw_counts_by_fog(1) = min(max(rawUnclamped1, rawCountMin), rawCountMax);

[q_uncal_dps(1), icClampUncal1] = anelloInterp1Clamped(ic_counts_fog1, ic_rate_fog1, raw_counts_by_fog(1));
tc_corrected_counts_by_fog(1) = raw_counts_by_fog(1) - tc_bias_counts_by_fog(1);
[q_ic_1, icClampCal1] = anelloInterp1Clamped(ic_counts_fog1, ic_rate_fog1, tc_corrected_counts_by_fog(1));
q_cal_dps(1) = sfvt_by_fog(1) * q_ic_1;
ic_clamp_flags(1) = icClampInv1 || icClampUncal1 || icClampCal1;

% ==== FOG 2 =====================================================================================
[sfvt_by_fog(2), sfvtClamp2] = anelloInterp1Clamped(sfvt_temp_fog2, sfvt_scale_fog2, temperature_counts);
[tc_bias_counts_by_fog(2), tcClamp2] = anelloInterp1Clamped(tc_temp_fog2, tc_offset_fog2, temperature_counts);
temp_clamp_flags(2) = sfvtClamp2 || tcClamp2;

rate_before_sfvt_2 = q_noisy_dps(2) / sfvt_by_fog(2);
[ideal_counts_by_fog(2), icClampInv2] = anelloInverseMonotonicLUT(ic_counts_fog2, ic_rate_fog2, rate_before_sfvt_2);
rawUnclamped2 = ideal_counts_by_fog(2) + tc_bias_counts_by_fog(2);
count_saturation_flags(2) = (rawUnclamped2 < rawCountMin) || (rawUnclamped2 > rawCountMax);
raw_counts_by_fog(2) = min(max(rawUnclamped2, rawCountMin), rawCountMax);

[q_uncal_dps(2), icClampUncal2] = anelloInterp1Clamped(ic_counts_fog2, ic_rate_fog2, raw_counts_by_fog(2));
tc_corrected_counts_by_fog(2) = raw_counts_by_fog(2) - tc_bias_counts_by_fog(2);
[q_ic_2, icClampCal2] = anelloInterp1Clamped(ic_counts_fog2, ic_rate_fog2, tc_corrected_counts_by_fog(2));
q_cal_dps(2) = sfvt_by_fog(2) * q_ic_2;
ic_clamp_flags(2) = icClampInv2 || icClampUncal2 || icClampCal2;

% ==== FOG 3 =====================================================================================
[sfvt_by_fog(3), sfvtClamp3] = anelloInterp1Clamped(sfvt_temp_fog3, sfvt_scale_fog3, temperature_counts);
[tc_bias_counts_by_fog(3), tcClamp3] = anelloInterp1Clamped(tc_temp_fog3, tc_offset_fog3, temperature_counts);
temp_clamp_flags(3) = sfvtClamp3 || tcClamp3;

rate_before_sfvt_3 = q_noisy_dps(3) / sfvt_by_fog(3);
[ideal_counts_by_fog(3), icClampInv3] = anelloInverseMonotonicLUT(ic_counts_fog3, ic_rate_fog3, rate_before_sfvt_3);
rawUnclamped3 = ideal_counts_by_fog(3) + tc_bias_counts_by_fog(3);
count_saturation_flags(3) = (rawUnclamped3 < rawCountMin) || (rawUnclamped3 > rawCountMax);
raw_counts_by_fog(3) = min(max(rawUnclamped3, rawCountMin), rawCountMax);

[q_uncal_dps(3), icClampUncal3] = anelloInterp1Clamped(ic_counts_fog3, ic_rate_fog3, raw_counts_by_fog(3));
tc_corrected_counts_by_fog(3) = raw_counts_by_fog(3) - tc_bias_counts_by_fog(3);
[q_ic_3, icClampCal3] = anelloInterp1Clamped(ic_counts_fog3, ic_rate_fog3, tc_corrected_counts_by_fog(3));
q_cal_dps(3) = sfvt_by_fog(3) * q_ic_3;
ic_clamp_flags(3) = icClampInv3 || icClampUncal3 || icClampCal3;

% ---- Reconstruct body-frame rate from the three FOG projections, back to rad/s ---------------
omega_uncal_body_radps = (U_fog \ q_uncal_dps) ./ r2d;
omega_cal_body_radps   = (U_fog \ q_cal_dps)   ./ r2d;

end
