% Phase A importer parity test: re-parse the AUTHORITATIVE archive source files independently
% (regex over the raw .py/.json text, not by trusting hand-transcribed literals) and compare
% every value against anelloX3SiPhOGCalibrationData_SN261200001124.m. Guards against
% transcription error in the generated MATLAB data file, per PLAN Phase A item 5.

thisDir = fileparts(mfilename('fullpath'));
calibDir = fullfile(thisDir, 'imu_temp_calib', 'SN261200001124_cal_tables');

cal = anelloX3SiPhOGCalibrationData_SN261200001124();

TOL = 1e-9;

% ---- IC LUTs -----------------------------------------------------------------------------
icText = fileread(fullfile(calibDir, 'SN261200001124_IC_LUTs.py'));

icBlocks = {'real_fog_1_sensorCounts_IC', 'real_fog_1_scaledOutput_IC', ...
            'real_fog_2_sensorCounts_IC', 'real_fog_2_scaledOutput_IC', ...
            'real_fog_3_sensorCounts_IC', 'real_fog_3_scaledOutput_IC'};
icParsed = struct();
for k = 1:numel(icBlocks)
    icParsed.(icBlocks{k}) = parsePythonNumericBlock(icText, icBlocks{k});
end

checkArray('ic.counts_fog1', icParsed.real_fog_1_sensorCounts_IC, cal.ic.counts_fog1);
checkArray('ic.rate_fog1',   icParsed.real_fog_1_scaledOutput_IC, cal.ic.rate_fog1);
checkArray('ic.counts_fog2', icParsed.real_fog_2_sensorCounts_IC, cal.ic.counts_fog2);
checkArray('ic.rate_fog2',   icParsed.real_fog_2_scaledOutput_IC, cal.ic.rate_fog2);
checkArray('ic.counts_fog3', icParsed.real_fog_3_sensorCounts_IC, cal.ic.counts_fog3);
checkArray('ic.rate_fog3',   icParsed.real_fog_3_scaledOutput_IC, cal.ic.rate_fog3);

icNumPts = parsePythonNumericBlock(icText, 'real_fog_1_numOfPts_IC');
assert(icNumPts(1) == cal.ic.numPts(1), 'ic.numPts(1) mismatch');
icNumPts = parsePythonNumericBlock(icText, 'real_fog_2_numOfPts_IC');
assert(icNumPts(1) == cal.ic.numPts(2), 'ic.numPts(2) mismatch');
icNumPts = parsePythonNumericBlock(icText, 'real_fog_3_numOfPts_IC');
assert(icNumPts(1) == cal.ic.numPts(3), 'ic.numPts(3) mismatch');
fprintf('OK  ic.numPts matches [%d %d %d]\n', cal.ic.numPts(1), cal.ic.numPts(2), cal.ic.numPts(3));

% sensAxes: first row of each FOG's 6x3 sensAxes matrix
fog1Axes = parsePythonNumericBlock(icText, 'real_fog_1_sensAxes');
fog2Axes = parsePythonNumericBlock(icText, 'real_fog_2_sensAxes');
fog3Axes = parsePythonNumericBlock(icText, 'real_fog_3_sensAxes');
checkArray('sensAxes.fog1', fog1Axes(1:3), cal.sensAxes(1,:));
checkArray('sensAxes.fog2', fog2Axes(1:3), cal.sensAxes(2,:));
checkArray('sensAxes.fog3', fog3Axes(1:3), cal.sensAxes(3,:));

% ---- TC LUTs ------------------------------------------------------------------------------
tcText = fileread(fullfile(calibDir, 'SN261200001124_TC_LUTs.py'));

checkArray('tc.tempCounts_fog1', parsePythonNumericBlock(tcText, 'real_fog_1_tempCounts_TC'), cal.tc.tempCounts_fog1);
checkArray('tc.offset_fog1',     parsePythonNumericBlock(tcText, 'real_fog_1_sensorCounts_TC'), cal.tc.offset_fog1);
checkArray('tc.tempCounts_fog2', parsePythonNumericBlock(tcText, 'real_fog_2_tempCounts_TC'), cal.tc.tempCounts_fog2);
checkArray('tc.offset_fog2',     parsePythonNumericBlock(tcText, 'real_fog_2_sensorCounts_TC'), cal.tc.offset_fog2);
checkArray('tc.tempCounts_fog3', parsePythonNumericBlock(tcText, 'real_fog_3_tempCounts_TC'), cal.tc.tempCounts_fog3);
checkArray('tc.offset_fog3',     parsePythonNumericBlock(tcText, 'real_fog_3_sensorCounts_TC'), cal.tc.offset_fog3);

% ---- SFvT LUTs (JSON) -- mapping x->FOG1, z->FOG2, y->FOG3 (accepted default, PLAN gate 2) --
sfx = jsondecode(fileread(fullfile(calibDir, '261200001124_sf_calibration_x_05_05_2026__08_09_51_sf_lut.json')));
sfy = jsondecode(fileread(fullfile(calibDir, '261200001124_sf_calibration_y_05_04_2026__18_55_49_sf_lut.json')));
sfz = jsondecode(fileread(fullfile(calibDir, '261200001124_sf_calibration_z_05_05_2026__12_01_57_sf_lut.json')));

assert(strcmp(sfx.axis, 'x') && strcmp(sfy.axis, 'y') && strcmp(sfz.axis, 'z'), 'SFvT axis labels unexpected');
assert(sfx.length == 17 && sfy.length == 17 && sfz.length == 17, 'SFvT length ~= 17');

checkArray('sfvt.tempCounts_fog1(<-x)', sfx.temp_counts, cal.sfvt.tempCounts_fog1);
checkArray('sfvt.scale_fog1(<-x)',      sfx.scale_factor, cal.sfvt.scale_fog1);
checkArray('sfvt.tempCounts_fog2(<-z)', sfz.temp_counts, cal.sfvt.tempCounts_fog2);
checkArray('sfvt.scale_fog2(<-z)',      sfz.scale_factor, cal.sfvt.scale_fog2);
checkArray('sfvt.tempCounts_fog3(<-y)', sfy.temp_counts, cal.sfvt.tempCounts_fog3);
checkArray('sfvt.scale_fog3(<-y)',      sfy.scale_factor, cal.sfvt.scale_fog3);

% ---- Monotonicity / range checks (Phase A item 2) --------------------------------------------
assert(all(diff(cal.ic.counts_fog1) > 0), 'ic.counts_fog1 not strictly increasing');
assert(all(diff(cal.ic.rate_fog1)   > 0), 'ic.rate_fog1 not strictly increasing (non-monotonic IC output)');
assert(all(diff(cal.ic.counts_fog2) > 0), 'ic.counts_fog2 not strictly increasing');
assert(all(diff(cal.ic.rate_fog2)   > 0), 'ic.rate_fog2 not strictly increasing (non-monotonic IC output)');
assert(all(diff(cal.ic.counts_fog3) > 0), 'ic.counts_fog3 not strictly increasing');
assert(all(diff(cal.ic.rate_fog3)   > 0), 'ic.rate_fog3 not strictly increasing (non-monotonic IC output)');
fprintf('OK  all IC LUT abscissas/outputs strictly monotonic increasing\n');

assert(all(diff(cal.tc.tempCounts_fog1) > 0), 'tc.tempCounts_fog1 not strictly increasing');
assert(all(diff(cal.tc.tempCounts_fog2) > 0), 'tc.tempCounts_fog2 not strictly increasing');
assert(all(diff(cal.tc.tempCounts_fog3) > 0), 'tc.tempCounts_fog3 not strictly increasing');
assert(all(diff(cal.sfvt.tempCounts_fog1) > 0), 'sfvt.tempCounts_fog1 not strictly increasing');
assert(all(diff(cal.sfvt.tempCounts_fog2) > 0), 'sfvt.tempCounts_fog2 not strictly increasing');
assert(all(diff(cal.sfvt.tempCounts_fog3) > 0), 'sfvt.tempCounts_fog3 not strictly increasing');
fprintf('OK  all TC/SFvT temperature-count abscissas strictly monotonic increasing\n');

assert(cal.ic.counts_fog1(1) == cal.rawCountMin && cal.ic.counts_fog1(end) == cal.rawCountMax, 'fog1 IC endpoints ~= signed-24-bit range');
assert(cal.ic.counts_fog2(1) == cal.rawCountMin && cal.ic.counts_fog2(end) == cal.rawCountMax, 'fog2 IC endpoints ~= signed-24-bit range');
assert(cal.ic.counts_fog3(1) == cal.rawCountMin && cal.ic.counts_fog3(end) == cal.rawCountMax, 'fog3 IC endpoints ~= signed-24-bit range');
fprintf('OK  all IC LUT count endpoints == signed-24-bit range [%d, %d]\n', cal.rawCountMin, cal.rawCountMax);

% ---- U_fog rank / condition (Phase B prerequisite, checked here since data is frozen here) --
Ufog = cal.sensAxes;
r = rank(Ufog);
c = cond(Ufog);
assert(r == 3, 'U_fog is rank-deficient (rank=%d) -- FOG axes are not independent', r);
fprintf('OK  U_fog rank = 3, condition number = %.4f\n', c);

fprintf('\nAll Phase A importer parity checks passed (%d array/scalar comparisons).\n', 12+3+6+6+3);


function checkArray(name, sourceVals, generatedVals)
TOL = 1e-9;
d = max(abs(sourceVals(:) - generatedVals(:)));
assert(numel(sourceVals) == numel(generatedVals), ...
    '%s: length mismatch (source %d vs generated %d)', name, numel(sourceVals), numel(generatedVals));
assert(d < TOL, '%s: max abs diff %.3e exceeds tolerance %.1e', name, d, TOL);
fprintf('OK  %-28s  n=%3d  maxdiff=%.3e\n', name, numel(sourceVals), d);
end

function vals = parsePythonNumericBlock(text, varName)
% Extract the bracketed numeric list assigned to varName in a Python source file with the
% "varName = \\n [v1, v2, ..., #comment\n  v3, ...]" continuation style used by these archives.
pat = [varName '\s*=\s*\\?\s*\n?\s*\[(.*?)\]'];
m = regexp(text, pat, 'once', 'tokens', 'dotall');
assert(~isempty(m), 'Could not find block "%s" in source text', varName);
body = m{1};
body = regexprep(body, '#[^\n]*', '');       % strip Python comments
nums = regexp(body, '-?\d+\.?\d*', 'match');
vals = str2double(nums);
end
