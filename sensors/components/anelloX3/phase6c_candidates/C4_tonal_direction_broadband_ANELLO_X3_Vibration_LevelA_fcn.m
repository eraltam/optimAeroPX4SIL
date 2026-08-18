function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C4 -- DIRECTION + QUASI-BROADBAND (this disposition's top hardware priority).
% Ka_z=2.0 direction change, PLUS a throttle-scaled sum of 6 fixed-frequency, fixed-phase,
% fixed-amplitude sinusoids spanning 5-45Hz standing in for genuine band-limited broadband noise.
%
% UPDATED 2026-08-04 (post R_peak_real correction, see
% ../../../../AnalysisIMU/vibration_full_mission_comparison/ERRATUM_R_PEAK_REAL_CORRECTION_2026-08-04.md):
% the K_fund x0.75/K_bpf x1.25 tonal rebalance this candidate previously inherited from C2/C3 is
% REMOVED -- C2 was dropped from the Phase 6C disposition (worst-ranked lever once R_peak_real's
% gravity/DC-in-norm bug was fixed; corrected target ~0.446, not 1.776). K_fund_accel/K_bpf_accel/
% K_fund_gyro/K_bpf_gyro below are back to production's unchanged values. Ka_z=2.0 is unchanged from
% the prior version and matches this disposition's chosen direction value (see C3's header for why
% 2.0 over the offline screening's best-R_peak-match 3.0 -- Z-fraction guardrail).
%
% Of the three broadband gain levels screened offline (`phase6b_offline_screening.py` Stage 4:
% bb_5_45hz_low/med, bb_5_30hz_med), `bb_5_45hz_med` (the "med" gain level, 5-45Hz span) had the best
% combined per-axis dB error (8.3 dB mean, vs. 9.2/11.4 dB for the other two) AND the best signed-
% per-band guardrail performance (closes the 5-15Hz and 30-45Hz sim-undershoot gaps the most,
% including the only candidate reaching the 30-45Hz band at all among the two "med"-gain options --
% bb_5_30hz_med's 30Hz cutoff misses it entirely -- without widening the already-negative 15-30Hz/
% 45-70Hz gaps beyond the no-broadband reference, which stay numerically unchanged since none of the
% screened broadband candidates inject energy above 45Hz). That screening used filtered Gaussian
% noise (not codegen-safe for a production MATLAB Function block, see below); this candidate's fixed
% sinusoid-bank gains (bbAccelGain/bbGyroGain) were NOT re-derived from bb_5_45hz_med's own gain
% parameter (different noise-generation schemes, not directly convertible) -- they remain this
% file's own pre-existing Class 3 estimate, to be tuned from this candidate's own HIL result.
%
% DELIBERATE SIMPLIFICATION FOR THIS HIL CANDIDATE ONLY (not the plan's proposed final broadband
% architecture): the plan's Section 6/Phase 6B broadband design is `a_bb(t)=G(u)*H_bb(z)*n(t)`
% (filtered white noise). Implementing that inside a MATLAB Function block destined for real-
% hardware HIL requires either a runtime `rand`/`randn` call (the plan's own guardrail explicitly
% prohibits runtime rng in the production path, Section 4/Section 8 item 7 -- risk of codegen
% problems and non-reproducible builds) or a hand-rolled deterministic PRNG + persistent-state IIR
% filter, which is nontrivial to get right and verify quickly for a real-hardware test under time
% pressure. This candidate instead uses a FIXED sum of 6 deterministic sinusoids at moderate,
% non-tonal frequencies (8, 13, 19, 24, 33, 41 Hz -- chosen to spread across the 5-45Hz range Phase
% 6A found sim under-shoots, avoiding the 19-24Hz frequencies' EXACT overlap issue by design since
% those are informational targets, not exclusions) with small individual amplitudes, scaled by the
% same omega^2-times-throttle convention as the tonal terms. This is a pragmatic proxy for "does
% adding broadband-like energy in the under-shot bands help," not a claim that this IS the final
% broadband architecture -- if this candidate's HIL result looks promising, implement the real
% filtered-white-noise version properly (with a validated codegen-safe PRNG+filter) before treating
% it as a production candidate.

persistent phase
if isempty(phase)
    phase = (0:5)' * (2*pi/6);
end

if enableVibration <= 0
    accel_vib_mps2 = zeros(3,1);
    gyro_vib_radps = zeros(3,1);
    return
end

Ts = 0.004;
Nb = 2;

motorLocs = [ 0.0   0.5   0.0;
              0.0  -0.5   0.0;
              0.43 -0.25  0.0;
             -0.43  0.25  0.0;
              0.43  0.25  0.0;
             -0.43 -0.25  0.0];

K_fund_accel = 5.464e-6;
K_bpf_accel  = 1.639e-5;
K_fund_gyro  = 1.366e-7;
K_bpf_gyro   = 4.098e-7;
Ka_z = 2.0;

% CANDIDATE C4 CHANGE: quasi-broadband sinusoid bank (see header). omega_max used for throttle
% normalization is vehicleParams.rotor.maxRPM(3473)*2*pi/60, matching setUpActuators.m.
persistent bbPhase
if isempty(bbPhase)
    bbPhase = zeros(6,1);  % one running phase per quasi-broadband tone
end
bbFreqHz = [8; 13; 19; 24; 33; 41];
% Units match K_fund_accel/K_fund_gyro exactly (per (rad/s)^2, applied to mean-omega^2 below, same
% convention as the tonal terms' w2 scaling) -- same order of magnitude as K_fund_accel/K_fund_gyro
% so this quasi-broadband term is comparable to, not orders of magnitude larger/smaller than, the
% existing tonal terms. Class 3 estimate, tune from HIL result.
bbAccelGain = 6.0e-6;  % m/s^2 per (rad/s)^2
bbGyroGain  = 1.5e-7;  % rad/s per (rad/s)^2

phase = mod(phase + rotorAngVel_radps * Ts, 2*pi);
bbPhase = mod(bbPhase + 2*pi*bbFreqHz*Ts, 2*pi);

accel_vib_mps2 = zeros(3,1);
gyro_vib_radps = zeros(3,1);

for i = 1:6
    w2 = rotorAngVel_radps(i)^2;
    s1 = sin(phase(i));
    s2 = sin(Nb*phase(i));

    r = motorLocs(i, 1:2);
    rn = norm(r);
    if rn > 1e-6
        ux = r(1) / rn;
        uy = r(2) / rn;
    else
        ux = 0.0;
        uy = 0.0;
    end
    accel_dir = [uy; -ux; Ka_z];
    accel_dir = accel_dir / norm(accel_dir);
    gyro_dir = [ux; uy; 1.0];
    gyro_dir = gyro_dir / norm(gyro_dir);

    accel_vib_mps2 = accel_vib_mps2 + w2 * (K_fund_accel * s1 + K_bpf_accel * s2) * accel_dir;
    gyro_vib_radps = gyro_vib_radps + w2 * (K_fund_gyro * s1 + K_bpf_gyro * s2) * gyro_dir;
end

meanOmega2 = mean(rotorAngVel_radps)^2;  % same omega^2 scaling convention as the tonal terms
bbAccelSum = sum(sin(bbPhase)) * bbAccelGain * meanOmega2 / sqrt(numel(bbFreqHz));
bbGyroSum  = sum(sin(bbPhase)) * bbGyroGain  * meanOmega2 / sqrt(numel(bbFreqHz));
% Applied isotropically (equal on all 3 axes) -- simplest choice for this quick screening
% candidate; a real broadband model would derive its own axis coupling from evidence (Phase 4),
% not assumed here.
accel_vib_mps2 = accel_vib_mps2 + bbAccelSum * ones(3,1) / sqrt(3);
gyro_vib_radps = gyro_vib_radps + bbGyroSum * ones(3,1) / sqrt(3);
end
