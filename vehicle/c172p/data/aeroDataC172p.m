% Data For Cessna 172P Aero Model
% Source: jsbsim/aircraft/c172p/c172p.xml (JSBSim project, community-validated reference
% aircraft dataset -- see PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md).
%
% DOCUMENTATION/REFERENCE ONLY -- NOT called from Simulink-facing code. The actual aero model
% (vehicle/c172p/components/C172pAeroCoefficientsModel.slx, built by
% build_C172pAeroCoefficientsModel.m) inlines every constant below as a local literal inside its
% MATLAB Function block, the same convention already used by
% vehicle/fixedwing_plane/components/build_FixedwingPlaneAeroCoefficientsModel.m and documented in
% optimAeroPX4SIL/CLAUDE.md section 3 (MATLAB Function blocks in this MATLAB release cannot
% reliably compile a call to a large-nested-struct-returning params function). Keep this file and
% the inlined copy in sync BY HAND if the source c172p.xml ever changes.
%
% IMPORTANT UNIT NOTE: every <function> under c172p.xml's <axis> elements is wrapped in a
% <product> that multiplies by aero/qbar-psf * metrics/Sw-sqft (and, for moments, also
% metrics/bw-ft or metrics/cbarw-ft). That wrapper is JSBSim's OWN internal coefficient->force/
% moment conversion -- it is NOT part of the aerodynamic coefficient itself. This SIL's aero.slx
% pipeline already does that same qbar*Sref(*bref/cref) conversion generically, downstream, for
% every vehicle (see aero.slx's "sumForcesMoments" subsystem). So every constant captured below is
% the RAW coefficient value (the JSBSim <value> element), with the qbar/Sw/bw/cbarw wrapper
% stripped out -- do not re-multiply by qbar*Sref when using these numbers in this SIL.
%
% Explicitly NOT ported (documented fidelity gaps, same practice as this project's other vehicles):
%   - Ground effect (kCDge/kCLge vs height/span) -- assumed 1 (out of ground effect).
%   - Flap channel (CDDf/CLDf/Cmdf vs flap-pos-deg) -- this SIL has no flap actuator; flap=0 always.
%   - CLadot/Cmadot (lift/pitch-moment due to alpha-RATE) -- C172pAeroCoefficientsModel.slx's
%     interface (matching F16AeroCoefficientsModel.slx/FixedwingPlaneAeroCoefficientsModel.slx) has
%     no alphadot input.
%   - Stall hysteresis (aero/stall-hyst-norm) -- CLwbh's stall-hyst-norm=0 (pre-stall / attached-
%     flow) column is used unconditionally; the post-stall hysteresis loop is not modeled.
%   - qbar-induced (propeller-slipstream-augmented dynamic pressure) used by JSBSim for
%     Cmde/Cndr -- irrelevant here since the qbar wrapper itself is stripped (see note above).

%% Geometry (jsbsim/aircraft/c172p/c172p.xml <metrics>, converted to SI -- must match
%% vehicle/common/setUpVehicle.m's "c172p" case)
bRef_m = 35.8 * 0.3048;      % wingspan, 10.9118 m
cRef_m = 4.9 * 0.3048;       % mean chord, 1.4935 m
SRef_m2 = 174 * 0.3048^2;    % wing area, 16.1652 m^2

%% DRAG axis
CD0 = 0.027;                  % aero/coefficient/CDo
CDde_coeff = 0.06;             % aero/coefficient/CDDe, multiplies |elevator_rad|
CDbeta_coeff = 0.17;           % aero/coefficient/CDbeta, multiplies |beta_rad|
% aero/coefficient/CDwbh, table vs alpha-rad at flap-pos-deg=0 (drag due to alpha)
CDwbh_alpha_bkpts_rad = [-0.0873 -0.0698 -0.0524 -0.0349 -0.0175 0.0000 0.0175 0.0349 0.0524 ...
    0.0698 0.0873 0.1047 0.1222 0.1396 0.1571 0.1745 0.1920 0.2094 0.2269 0.2443 0.2618 0.2793 ...
    0.2967 0.3142 0.3316 0.3491];
CDwbh_data = [0.0041 0.0013 0.0001 0.0003 0.0020 0.0052 0.0099 0.0162 0.0240 0.0334 0.0442 ...
    0.0566 0.0706 0.0860 0.0962 0.1069 0.1180 0.1298 0.1424 0.1565 0.1727 0.1782 0.1716 0.1618 ...
    0.1475 0.1097];

%% SIDE axis
CYbeta_coeff = -0.1370 / 0.3490;   % aero/coefficient/CYb, linearized from the 3-point beta table
                                     % (flap=0 col): beta=[-0.349,0,0.349] -> CY=[0.137,0,-0.137]
CYda_coeff = 0.0;                  % aero/coefficient/CYda (JSBSim value is literally 0)
CYdr_coeff = 0.187;                % aero/coefficient/CYdr, multiplies rudder_rad
% aero/coefficient/CYp, 2-pt table vs alpha-rad at flap=0, multiplies bi2vel*p_radps
CYp_alpha_bkpts_rad = [0.0000 0.0940];
CYp_data = [-0.0750 -0.1450];
% aero/coefficient/CYr, 2-pt table vs alpha-rad at flap=0, multiplies bi2vel*r_radps
CYr_alpha_bkpts_rad = [0.0000 0.0940];
CYr_data = [0.2140 0.2670];

%% LIFT axis
% aero/coefficient/CLwbh, table vs alpha-rad at stall-hyst-norm=0 (pre-stall column; hysteresis
% not modeled, see header note) -- this IS the full CL-alpha curve (includes CL0 intercept, no
% separate additive term needed)
CLwbh_alpha_bkpts_rad = [-0.0900 0.0000 0.0900 0.1000 0.1200 0.1400 0.1600 0.1700 0.1900 0.2100 ...
    0.2400 0.2600 0.2800 0.3000 0.3200 0.3400 0.3600];
CLwbh_data = [-0.2200 0.2500 0.7300 0.8300 0.9200 1.0200 1.0800 1.1300 1.1900 1.2500 1.3500 ...
    1.4400 1.4700 1.4300 1.3800 1.3000 1.1500];
CLde_coeff = 0.43;                 % aero/coefficient/CLDe, multiplies elevator_rad
CLq_coeff = 3.9;                   % aero/coefficient/CLq, multiplies ci2vel*q_radps

%% ROLL axis
ClBeta_coeff = -0.0322 / 0.3490;   % aero/coefficient/Clb, linearized from the 3-point beta table:
                                     % beta=[-0.349,0,0.349] -> Cl=[0.0322,0,-0.0322]
Clp_coeff = -0.484;                 % aero/coefficient/Clp, multiplies bi2vel*p_radps
% aero/coefficient/Clr, 2-pt table vs alpha-rad at flap=0, multiplies bi2vel*r_radps
Clr_alpha_bkpts_rad = [0.0000 0.0940];
Clr_data = [0.0798 0.1869];
ClDa_coeff = 0.229;                 % aero/coefficient/ClDa, multiplies aileron_rad
Cldr_coeff = 0.0147;                % aero/coefficient/Cldr, multiplies rudder_rad

%% PITCH axis
Cmo_coeff = 0.10;                   % aero/coefficient/Cmo
Cmalpha_coeff = -1.8;                % aero/coefficient/Cmalpha, multiplies alpha_rad
Cmq_coeff = -12.4;                   % aero/coefficient/Cmq, multiplies ci2vel*q_radps
Cmde_coeff = -1.122;                 % aero/coefficient/Cmde, multiplies elevator_rad

%% YAW axis
Cnbeta_coeff = 0.0205 / 0.3490;      % aero/coefficient/Cnb, linearized from the 3-point beta
                                       % table: beta=[-0.349,0,0.349] -> Cn=[-0.0205,0,0.0205]
Cnp_coeff = -0.0278;                 % aero/coefficient/Cnp, multiplies bi2vel*p_radps
Cnr_coeff = -0.0937;                 % aero/coefficient/Cnr, multiplies bi2vel*r_radps
Cnda_coeff = -0.0053;                % aero/coefficient/Cnda, multiplies aileron_rad
Cndr_coeff = -0.0430;                % aero/coefficient/Cndr, multiplies rudder_rad
