function p = anelloVibrationLevelA2TwoBandBroadbandParams()
% "Two-band quasi-broadband" candidate (2026-08-05) -- NOT the C4 "6-tone" candidate, and
% deliberately not called "broadband" alone: it is still a small, fixed sinusoid bank, just
% targeting two bands instead of pretending to be flat 5-45Hz coverage.
%
% WHY this replaces C4's 6-tone bbFreqHz=[8;13;19;24;33;41]: PCA (true covariance/principal-
% eigenvector, 50-65Hz methodology, see pca_direction_analysis.py) on the direction-unchanged
% ("Ka_z=1") + 6-tone-broadband candidate showed the 18-25Hz guardrail collapsing from ~0.98 (real
% target ~0.99, already close, broadband OFF) to ~0.59 when broadband is added -- root-caused
% exactly: the broadband term adds the SAME scalar realization to x/y/z via a fixed [1,1,1]/sqrt(3)
% direction (see ANELLO_X3_Vibration_LevelA_fcn_v2.m's broadband block), which is a rank-1
% covariance contribution with principal-eigenvector Z-component exactly 1/sqrt(3)=0.577 --
% matching the observed ~0.59 collapse exactly, not coincidentally. The 19Hz and 24Hz tones sit
% inside the 18-25Hz guardrail band and are the specific cause.
%
% Fix: drop the 19Hz and 24Hz tones, keep only 8/13/33/41Hz -- these are exactly the tones that sit
% inside the 5-15Hz and 30-45Hz bands the broadband term was actually meant to fix (Phase 6A found
% sim under-shoots real there); nothing inside 18-25Hz remains to interact with that guardrail.
% Gains rescaled by sqrt(4/6) so each SURVIVING tone's own per-tone amplitude is unchanged (the
% original formula divides by sqrt(numel(bbFreqHz)); going from 6 to 4 tones without rescaling
% would silently amplify each remaining tone by sqrt(6/4), not preserve them).
p = anelloVibrationLevelA2DefaultParams();
p.broadbandEnable = true;
p.bbFreqHz = [8; 13; 33; 41];
p.bbAccelGain = 6.0e-6 * sqrt(4/6);   % = 4.899e-6
p.bbGyroGain  = 1.5e-7 * sqrt(4/6);   % = 1.225e-7
% Direction gains (Ka_*, Kg_*) intentionally left at DefaultParams' 1.0 -- this candidate is
% "production direction, unchanged" per the corrected PCA finding that Ka_z=2.0 (C3/C4's choice)
% over-verticalizes the 50-65Hz band relative to the real target.
end
