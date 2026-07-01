% setUpHITLConnection: Locates the UAV Toolbox Support Package for PX4 Autopilots'
% px4MAVLinkBridgelib (MAVLink Bridge Source/Sink blocks, serial connection to real PX4 hardware)
% and pushes the HITL link parameters into the base workspace for the Simulink HITL connector
% (PX4HITLConnector/PX4HITLConnector.slx) to read.
%
% Only called from initVehicleSIL.m when controllerRuntime="HITL". See CLAUDE_HITL.md for the full
% design and why this path-lookup is needed: px4MAVLinkBridgelib does not reliably resolve just
% because the support package shows as installed in matlab.addons.installedAddons -- its actual
% library/class files live under MATLAB's SupportPackages folder, which is not automatically on
% path in every session (and initVehicleSIL.m's own restoredefaultpath wipes it if it was).

% Find the px4 support package's "core" folder (contains the +px4 namespace package with the
% MAVLinkSource/MAVLinkSink class implementations) and "core/blocks" folder (contains
% px4MAVLinkBridgelib.slx itself). Search all installed MATLAB releases under SupportPackages
% rather than hardcoding a release, since the release folder name changes every year.
% NOTE: dir() on a path with no wildcard in its last segment lists that folder's CONTENTS, not the
% folder itself -- so we glob one level up (the release folder, e.g. "R2026a") and then probe each
% candidate's full px4/core path with isfolder, rather than trying to glob "core" directly.
spRoot = fullfile(getenv('PROGRAMDATA'), 'MATLAB', 'SupportPackages');
releaseDirs = dir(spRoot);
releaseDirs = releaseDirs([releaseDirs.isdir] & ~ismember({releaseDirs.name}, {'.', '..'}));

px4CoreDir = '';
for k = 1:numel(releaseDirs)
    candidate = fullfile(spRoot, releaseDirs(k).name, 'toolbox', 'target', 'supportpackages', 'px4', 'core');
    if isfolder(candidate)
        px4CoreDir = candidate;
        break;
    end
end

if isempty(px4CoreDir)
    error("Could not find the UAV Toolbox Support Package for PX4 Autopilots' px4 support files " + ...
        "under '%s'. Install/repair it via the Add-On Manager (Home tab > Add-Ons > Get " + ...
        "Hardware Support Packages > 'UAV Toolbox Support Package for PX4 Autopilots'), then retry.", ...
        spRoot);
end

addpath(px4CoreDir);
addpath(fullfile(px4CoreDir, 'blocks'));

if exist('px4MAVLinkBridgelib', 'file') ~= 4
    error("px4MAVLinkBridgelib still did not resolve after adding '%s' to the path. The support " + ...
        "package install may be corrupt -- try reinstalling it via the Add-On Manager.", px4CoreDir);
end

% HITL link parameters. NOTE: hitlParams.serialPort is NOT actually read live by the MAVLink
% Bridge Source/Sink blocks' PixhawkSerialPortManually mask parameter, despite that parameter
% being set to the string 'hitlParams.serialPort' when the model is built -- MaskVariables shows
% that parameter as "&2" (Simulink's notation for "take this string literally"), not "@2"
% ("evaluate as an expression"). Pointing it at a workspace variable NAME silently does nothing
% useful: it was confirmed (by actually running the model) to try opening a serial port literally
% named '\\.\hitlParams.serialPort'. The real fix: initVehicleSIL.m calls set_param(...,
% 'PixhawkSerialPortManually', char(opts.hitlSerialPort)) directly on both blocks, every run, after
% loading VehicleSilSimulation.slx. hitlParams.serialPort is kept here for logging/display only.
hitlParams.serialPort = char(opts.hitlSerialPort);
hitlParams.baudRate    = opts.hitlBaudRate;     % informational only -- the block hardcodes 921600
                                                  % (230400 on Linux) regardless of this value; kept
                                                  % here so it's visible/tunable if MathWorks changes
                                                  % that in a future release.
hitlParams.qgcRemotePortType   = 'UDP';
hitlParams.qgcRemoteIpAddress  = '127.0.0.1';
hitlParams.qgcRemotePortNumber = num2str(opts.hitlQGCPort);

% PX4HITLConnector.slx reuses the "HIL Sensor"/"HIL_GPS"/"Send Heartbeat" subsystems copied
% verbatim from uav_HITL_sample (see build_PX4HITLConnector.m), which reference these workspace
% variables internally (same names/roles as that project's setupHITLConfiguration.m). Defined here
% rather than depending on that sibling project's data files at runtime.
SampleTime    = 0.008;  % [s] HIL link base rate (125 Hz). Must be an integer multiple of the main
                          % plant's fixed step (stepSize_s = 0.004s / 250Hz) -- Simulink's fixed-step
                          % solver requires every sample time in the model to divide evenly into the
                          % base step once this connector is inlined into VehicleSilSimulation.slx
                          % (see wire_px4_hitl_selector.m / fix_inline_hitl_connector.m). The
                          % reference project's own default was 0.01s/100Hz, which is NOT a multiple
                          % of 0.004s -- 0.008s is the closest compatible rate.
gpsFreq       = 25;     % [Hz] HIL_GPS message rate
heartbeatFreq = 1;      % [Hz] HEARTBEAT message rate
MAVLink_Input_Read_Size = 1024;   % [bytes] matches MAVLinkSource.m's own dataLength default;
                                    % referenced both by the MAVLink Bridge Source block's mask and
                                    % internally by the copied "Read actuator output" subsystem

fprintf('HITL connection configured: serial port %s, baud %d (QGC UDP relay on port %d).\n', ...
    hitlParams.serialPort, hitlParams.baudRate, opts.hitlQGCPort);
