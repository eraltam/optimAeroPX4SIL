% Phase B tests for ANELLO_X3_SiPhOG_CalibrationCore_fcn.m / anelloInterp1Clamped.m /
% anelloInverseMonotonicLUT.m, per PLAN_ANELLO_X3_SIPHOG_TEMPERATURE_CALIBRATION_OUTPUT_MODES.md
% section 11.1-11.3, 11.6 (noise-disabled subset; PSD/Allan/noise-enabled sweep is Phase C,
% run against the integrated ANELLO_X3_IMU_fcn_SIL.m, not this standalone core).

cal = anelloX3SiPhOGCalibrationData_SN261200001124();
d2r = pi/180;
r2d = 180/pi;
TOLexact = 1e-9;

% =====================================================================================
% 11.1a: Exact recovery of every IC knot, forward and inverse
% =====================================================================================
fogNames = {'fog1','fog2','fog3'};
icCounts = {cal.ic.counts_fog1, cal.ic.counts_fog2, cal.ic.counts_fog3};
icRate   = {cal.ic.rate_fog1,   cal.ic.rate_fog2,   cal.ic.rate_fog3};

for f = 1:3
    counts = icCounts{f}; rate = icRate{f};
    for i = 1:numel(counts)
        % Exact knot queries -- including the two table endpoints themselves -- are in-domain
        % values, not out-of-range extrapolation, so clampFlag must be false for every i here.
        [yOut, flag] = anelloInterp1Clamped(counts, rate, counts(i));
        assert(abs(yOut - rate(i)) < TOLexact, '%s forward knot %d: got %.9f want %.9f', fogNames{f}, i, yOut, rate(i));
        assert(~flag, '%s forward knot %d: clamp flag unexpectedly true for an in-domain knot', fogNames{f}, i);

        [xOut, flagInv] = anelloInverseMonotonicLUT(counts, rate, rate(i));
        assert(abs(xOut - counts(i)) < TOLexact, '%s inverse knot %d: got %.9f want %.9f', fogNames{f}, i, xOut, counts(i));
        assert(~flagInv, '%s inverse knot %d: clamp flag unexpectedly true for an in-domain knot', fogNames{f}, i);
    end
    fprintf('OK  %s: %d/%d IC knots recovered exactly (forward + inverse)\n', fogNames{f}, numel(counts), numel(counts));
end

% =====================================================================================
% 11.1b: dense-grid forward(inverse(rate)) and inverse(forward(count)) round trip
% =====================================================================================
for f = 1:3
    counts = icCounts{f}; rate = icRate{f};
    rateGrid = linspace(rate(2), rate(end-1), 500);   % stay off the two synthetic full-range endpoints
    maxErrFwdInv = 0;
    for k = 1:numel(rateGrid)
        [cOut, ~] = anelloInverseMonotonicLUT(counts, rate, rateGrid(k));
        [rBack, ~] = anelloInterp1Clamped(counts, rate, cOut);
        maxErrFwdInv = max(maxErrFwdInv, abs(rBack - rateGrid(k)));
    end
    assert(maxErrFwdInv < 1e-6, '%s forward(inverse(rate)) round trip error %.3e too large', fogNames{f}, maxErrFwdInv);

    countGrid = linspace(counts(2), counts(end-1), 500);
    maxErrInvFwd = 0;
    for k = 1:numel(countGrid)
        [rOut, ~] = anelloInterp1Clamped(counts, rate, countGrid(k));
        [cBack, ~] = anelloInverseMonotonicLUT(counts, rate, rOut);
        maxErrInvFwd = max(maxErrInvFwd, abs(cBack - countGrid(k)));
    end
    assert(maxErrInvFwd < 1e-3, '%s inverse(forward(count)) round trip error %.3e too large', fogNames{f}, maxErrInvFwd);
    fprintf('OK  %s: dense round trip max err fwd(inv)=%.3e  inv(fwd)=%.3e\n', fogNames{f}, maxErrFwdInv, maxErrInvFwd);
end

% =====================================================================================
% 11.1c: exact recovery of every TC and SFvT knot
% =====================================================================================
tcTemp = {cal.tc.tempCounts_fog1, cal.tc.tempCounts_fog2, cal.tc.tempCounts_fog3};
tcOff  = {cal.tc.offset_fog1,     cal.tc.offset_fog2,     cal.tc.offset_fog3};
for f = 1:3
    tt = tcTemp{f}; oo = tcOff{f};
    for i = 1:numel(tt)
        [yOut, ~] = anelloInterp1Clamped(tt, oo, tt(i));
        assert(abs(yOut - oo(i)) < TOLexact, '%s TC knot %d mismatch', fogNames{f}, i);
    end
end
fprintf('OK  all TC LUT knots recovered exactly\n');

sfvtTemp = {cal.sfvt.tempCounts_fog1, cal.sfvt.tempCounts_fog2, cal.sfvt.tempCounts_fog3};
sfvtSc   = {cal.sfvt.scale_fog1,      cal.sfvt.scale_fog2,      cal.sfvt.scale_fog3};
for f = 1:3
    tt = sfvtTemp{f}; ss = sfvtSc{f};
    for i = 1:numel(tt)
        [yOut, ~] = anelloInterp1Clamped(tt, ss, tt(i));
        assert(abs(yOut - ss(i)) < TOLexact, '%s SFvT knot %d mismatch', fogNames{f}, i);
    end
end
fprintf('OK  all SFvT LUT knots recovered exactly\n');

% =====================================================================================
% 11.1d: endpoint clamp + diagnostic flag (well beyond every table's domain)
% =====================================================================================
[yLo, flagLo] = anelloInterp1Clamped(cal.ic.counts_fog1, cal.ic.rate_fog1, -1e9);
[yHi, flagHi] = anelloInterp1Clamped(cal.ic.counts_fog1, cal.ic.rate_fog1, 1e9);
assert(flagLo && yLo == cal.ic.rate_fog1(1), 'low clamp failed');
assert(flagHi && yHi == cal.ic.rate_fog1(end), 'high clamp failed');
fprintf('OK  endpoint clamp + flag correct for out-of-range forward query\n');

% =====================================================================================
% 11.2: axis reconstruction test (rank/condition already checked in importer test) --
% four body-rate vectors, zero noise, temperature at a representative in-domain value.
% =====================================================================================
Tcounts0 = 0;
testVectorsDps = [90 0 0; 0 90 0; 0 0 90; 30 -45 90];
axisTol_degps = 0.05;   % looser than knot-exact -- interpolation + inverse/forward chain error

for r = 1:size(testVectorsDps,1)
    truthDps = testVectorsDps(r,:)';
    truthRadps = truthDps * d2r;
    zeroNoise = zeros(3,1);
    [~, ~, ~, ~, omegaCal, ~, ~, ~, ~, icClamp] = ...
        ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, zeroNoise, Tcounts0);
    omegaCalDps = omegaCal * r2d;
    err = max(abs(omegaCalDps - truthDps));
    assert(~any(icClamp), 'unexpected IC clamp for in-range test vector [%g %g %g]', truthDps);
    assert(err < axisTol_degps, 'axis test [%g %g %g] deg/s: max err %.4f deg/s exceeds %.4f', ...
        truthDps(1), truthDps(2), truthDps(3), err, axisTol_degps);
    fprintf('OK  axis test truth=[%7.2f %7.2f %7.2f] deg/s  calibrated max err=%.4e deg/s\n', ...
        truthDps(1), truthDps(2), truthDps(3), err);
end

% =====================================================================================
% 11.3: TC add/subtract test -- raw_counts - TC(T) == ideal_counts, noise disabled
% =====================================================================================
testTemps = [-20000, -10000, 0, 5000, 10000, 14000, 20000];
for ti = 1:numel(testTemps)
    T = testTemps(ti);
    truthRadps = [10;-20;30] * d2r;
    [rawC, tcCorrC, idealC, ~, ~, ~, tcBias] = ...
        ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, zeros(3,1), T);
    diffVec = (rawC - tcBias) - idealC;
    assert(max(abs(diffVec)) < 1e-6, 'TC add/subtract mismatch at T=%d counts: %s', T, mat2str(diffVec));
    diffVec2 = tcCorrC - idealC;
    assert(max(abs(diffVec2)) < 1e-6, 'tc_corrected ~= ideal_counts at T=%d counts (noise disabled): %s', T, mat2str(diffVec2));
end
fprintf('OK  TC add/subtract identity holds at %d temperatures\n', numel(testTemps));

% =====================================================================================
% 11.4: temperature sweep, noise disabled -- calibrated recovers truth, uncalibrated moves
% =====================================================================================
sweepTemps = linspace(-32768, 32767, 25);
truthDps = [30; -45; 90];
truthRadps = truthDps * d2r;

calErr = zeros(numel(sweepTemps),1);
uncalVals = zeros(numel(sweepTemps),3);
for k = 1:numel(sweepTemps)
    [~, ~, ~, omegaUncal, omegaCal] = ...
        ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, zeros(3,1), sweepTemps(k));
    calErr(k) = max(abs(omegaCal*r2d - truthDps));
    uncalVals(k,:) = (omegaUncal*r2d)';
end
assert(max(calErr) < 0.5, 'calibrated output does not track truth across temperature sweep, max err %.4f deg/s', max(calErr));
uncalSpread = max(uncalVals,[],1) - min(uncalVals,[],1);
assert(all(uncalSpread > 0.05), 'uncalibrated output unexpectedly flat across temperature sweep: spread=%s', mat2str(uncalSpread));
fprintf('OK  temp sweep noise-off: calibrated max err=%.4f deg/s, uncalibrated peak-to-peak=%s deg/s\n', ...
    max(calErr), mat2str(uncalSpread,4));

% =====================================================================================
% 11.6 (noise-disabled subset): determinism -- identical inputs -> bit-identical outputs.
% This core function performs no random draws, so this is a direct structural guarantee,
% not a statistical one; the test exists to catch an accidental future randn()/state leak.
% =====================================================================================
[a1,b1,c1,d1,e1,f1,g1,h1,i1,j1] = ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, [1e-4;-2e-4;3e-4], 5000);
[a2,b2,c2,d2,e2,f2,g2,h2,i2,j2] = ANELLO_X3_SiPhOG_CalibrationCore_fcn(truthRadps, [1e-4;-2e-4;3e-4], 5000);
assert(isequal(a1,a2) && isequal(b1,b2) && isequal(c1,c2) && isequal(d1,d2) && isequal(e1,e2) ...
    && isequal(f1,f2) && isequal(g1,g2) && isequal(h1,h2) && isequal(i1,i2) && isequal(j1,j2), ...
    'calibration core is not deterministic for identical inputs');
fprintf('OK  determinism: identical inputs produce bit-identical raw/uncal/cal views\n');

fprintf('\nAll Phase B calibration-core tests passed.\n');
