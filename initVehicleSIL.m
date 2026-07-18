% initVehicleSIL: This function checks to make sure the matlab environment is set up properly. This file checks if the
% user has the required toolboxes, support packages, and loads variable and bus definitions that are used in the
% F-16.slx model.
% ======================================================================================================================
%                                                       INPUTS
% ======================================================================================================================
% opts.launchFullSIL:            Flag used to launch the full SIL, set to False if desire is to only to load workspace.
%                                NOTE: should not be used on first attempt of using SIL.
% opts.visualizationType:        The method that will be used to visualize the aircraft while SIL is running
% opts.simHostIP:                IP Address of users computer
% opts.vehicleType:              The plant model that will be used during the SIL simulation: F-16, hexarotor, etc
% opts.controllerType:           The flight controller used in the SIL simulation (currently can only be PX4)
% opts.failureType               A control surfance, engine, or motor failure. Must be element of  EnumHexFailureType.m or EnumF16FailureType.m
% opts.PX4RepoPath               Path relative to optimAeroPX4SIL if PX4 repo is on windows side, or path relative to root wsl directory
% opts.PX4InWSL                  If attempting to use the PX4 repo cloned into the WSL root directory, set this variable to true
% opts.wslDistro                WSL distribution used to build/run PX4. "auto" prefers PX4Simulink,
%                                then Ubuntu-22.04, instead of accidentally using an incompatible default distro.
% opts.makeClean                 Removes all the compiled build files and intermediate artifacts. This may need to be 
%                                set to true when making changes to the configuration file. 
% opts.clearSLCache              Clear simulink cache. This deletes the work folder and will force all slx models 
%                                to be recompiled. This can fix some Simulink errors
% opts.flightGearFreq_Hz         Frequency of the TCP send block is sending data to flightGear
% opts.flightGearHost            IP address used by the Simulink TCP send blocks for FlightGear.
%                                Defaults to 127.0.0.1 for local preflight visualization.
% opts.flightGearTelnetPort      TCP telnet port FlightGear opens for hexarotor rotor commands.
% opts.launchPreflightVisualization
%                                When launchFullSIL=false and visualizationType="FlightGear",
%                                start FlightGear and wait for the TCP endpoint before returning.
% opts.assignFailureButton:      Set to true if you want to map a vehicle failrue to a joystick button
% opts.controllerRuntime:        "SITL" (default, unchanged behavior) or "HITL". HITL keeps the plant,
%                                environment, and sensors in Simulink but replaces the simulated PX4
%                                SITL controller with a real PX4 autopilot connected over serial. See
%                                CLAUDE_HITL.md for the full design. HITL does NOT launch WSL/PX4 SITL.
% opts.hardwareTarget:           Flight controller board used when controllerRuntime="HITL". Currently
%                                only "CubeOrangePlus" is supported (build target
%                                cubepilot_cubeorangeplus_default -- a hardware flash target, distinct
%                                from the px4_sitl_default targets used for SITL. See CLAUDE_HITL.md
%                                section 6 and README.md for the flash flow).
% opts.hitlSerialPort:           Windows COM port the PX4 hardware is connected on (e.g. "COM5"),
%                                required when controllerRuntime="HITL". No default -- must be set
%                                explicitly per machine/cable.
% opts.hitlBaudRate:             Serial baud rate for the HITL MAVLink link. Defaults to 921600, the
%                                same rate jMAVSim uses for PX4 HITL mode (see
%                                px4.internal.block.MAVLinkBridgeBase in the UAV Toolbox Support
%                                Package for PX4 Autopilots).
% opts.hitlQGCPort:              Local UDP port the HITL connector relays the MAVLink stream to so
%                                QGroundControl can connect concurrently with the Simulink HITL link.
%                                Defaults to 14550 (QGC's standard MAVLink UDP port).
% ======================================================================================================================
%                                                    EXAMPLE USAGE
% ======================================================================================================================
% initVehicleSIL("launchFullSIL", false):
%   Run the intialization file only, this should be used before changing any models

% initVehicleSIL("launchFullSIL", true, "vehicleType", "F-16", "visualizationType", "FlightGear", "simHostIP", "10.0.0.200","PX4InWSL",true):
%   Launch full SIL sim of F-16, visualize vehicle using FlightGear, use the PX4 repo cloned into the WSL root directory
%   and set IP address for PX4 connection.

% initVehicleSIL("launchFullSIL", false, "vehicleType", "hexarotor", "visualizationType", "FlightGear", "simHostIP", "10.0.0.200","PX4InWSL", false)
%   Load sim as "hexarotor"

% initVehicleSIL("launchFullSIL", true, "vehicleType", "hexarotor", "visualizationType", "FlightGear", "simHostIP",
% "192.168.12.145", "PX4InWSL", true, "makeClean", true, "clearSLCache", false)
%   Launch full sim for hexarotor in FlightGear using the PX4 repo in the Linux partition for faster compilation, 
%   make clean on PX4 compile and  NOT clearing the Simulink clache

% initVehicleSIL("launchFullSIL", true, "vehicleType", "hexarotor", "visualizationType", "FlightGear","PX4InWSL",
% true,"failureType","motor1")
%  Full launch of sim using hexarotor with the failure injection set to motor1.

% initVehicleSIL("launchFullSIL", false, "vehicleType", "hexarotor", "controllerRuntime", "HITL", ...
%     "hardwareTarget", "CubeOrangePlus", "hitlSerialPort", "COM5")
%   Load the hexarotor sim wired for HITL against a Cube Orange Plus connected on COM5. Does not
%   launch WSL/PX4 SITL. Flash the board separately first -- see CLAUDE_HITL.md section 6.
function initVehicleSIL(opts)

arguments
    opts.launchFullSIL        (1,1) logical = false
    opts.vehicleType          (1,1) string  = "hexarotor"       % "F-16", "hexarotor"
    opts.visualizationType    (1,1) string  = "Matlab"          % "PassThrough", "FlightGear", or "Matlab"
    opts.simHostIP            (1,1) string  = "auto"            % Replace with your IP address (not WSL's IP)
    opts.controllerType       (1,1) string  = "PX4"             % Currently PX4 is the only controller that can be used
    opts.failureType          (1,1) string  = "none"            % Must be a F-16 or hexartor failure type listed in EnumHexFailureType.m or EnumF16FailureType.m
    opts.PX4RepoPath          (1,1) string  = "PX4-Autopilot"   % PX4 repository path
    opts.PX4InWSL             (1,1) logical = false             % Is PX4 repository stored in Linux partition
    opts.wslDistro            (1,1) string  = "auto"            % PX4-capable WSL distro, or "auto"
    opts.makeClean            (1,1) logical = false             % Run "make clean" before "make" - if in doubt, use if PX4 config changes made
    opts.clearSLCache         (1,1) logical = false             % Clear Simulink cache
    opts.flightGearFreq_Hz    (1,1) double  = 0.5               % Frequency of the TCP send block is sending data to flightGear
    opts.flightGearHost       (1,1) string  = "127.0.0.1"       % Local FlightGear TCP endpoint used by preflight/SIL visualization
    opts.flightGearTelnetPort (1,1) double  = 5400              % FlightGear telnet port used by hexarotor TCP send blocks
    opts.launchPreflightVisualization (1,1) logical = true      % Start FlightGear for launchFullSIL=false preflight runs
    opts.assignFailureButton  (1,1) logical = false             % Set to true if you want to map a vehicle failrue to a joystick button
    opts.controllerRuntime    (1,1) string  {mustBeMember(opts.controllerRuntime, ["SITL","HITL"])} = "SITL"
    opts.hardwareTarget       (1,1) string  {mustBeMember(opts.hardwareTarget, ["CubeOrangePlus"])} = "CubeOrangePlus"
    opts.hitlSerialPort       (1,1) string  = "auto"            % e.g. "COM5". Required (non-"auto") when controllerRuntime="HITL"
    opts.hitlBaudRate         (1,1) double  = 921600            % matches jMAVSim's PX4 HITL baud rate
    opts.hitlQGCPort          (1,1) double  = 14550             % QGroundControl UDP relay port
end
restoredefaultpath
% Note: In future versions these will be arguments
addpath('vehicle');
vehicleDefinition                    = vehicleRegistry(opts.vehicleType);
vehicleParams.requestedType          = opts.vehicleType;
vehicleParams.vehicleKey             = vehicleDefinition.VehicleKey;
vehicleParams.displayName            = vehicleDefinition.DisplayName;
vehicleParams.type                   = vehicleDefinition.PlantModel;
vehicleParams.baseVehicleType        = vehicleDefinition.BaseVehicleType;
vehicleParams.controllerType         = opts.controllerType;
vehicleParams.silControllerType      = vehicleDefinition.ControllerType;
vehicleParams.actuatorInterface      = vehicleDefinition.ActuatorInterface;
vehicleParams.failureType            = opts.failureType;
vehicleParams.PX4Target              = vehicleDefinition.PX4Target;
vehicleParams.registryEntry          = vehicleDefinition;
visualizationParams.flightGearFreq_Hz      = opts.flightGearFreq_Hz;
visualizationParams.flightGearHost         = opts.flightGearHost;
visualizationParams.flightGearTelnetPort   = opts.flightGearTelnetPort;

% HITL requires an explicit serial port -- "auto" detection isn't implemented (Cube Orange Plus
% enumerates as a generic USB-serial device; which COM port it lands on depends on what else is
% plugged in, so guessing it is unsafe). Fail fast here rather than deep inside model setup.
if strcmpi(opts.controllerRuntime, "HITL") && strcmpi(opts.hitlSerialPort, "auto")
    error("controllerRuntime=""HITL"" requires ""hitlSerialPort"" to be set explicitly (e.g. " + ...
        """COM5""). Check Windows Device Manager > Ports (COM & LPT) with the Cube Orange Plus " + ...
        "plugged in to find it.")
end

% check for required toolboxes, support packages, and MATLAB version
% list is here: (https://www.mathworks.com/matlabcentral/answers/377731-how-do-features-from-license-correspond-to-names
% -from-ver#answer_300675)
requiredTools ={'Aerospace_Blockset'
    'Aerospace_Toolbox'
    'SIMULINK'
    'Simulink_Test'
    'Instrument Control Toolbox'
    'UAV_Toolbox'};

% NOTE: required unconditionally, not just for controllerRuntime="HITL". VehicleSilSimulation.slx's
% "PX4 HITL Interface" subsystem (built from px4MAVLinkBridgelib) is always present in the model --
% it's chosen at runtime via a Multiport Switch on CONTROLLER_RUNTIME, not a true Variant Subsystem
% (px4MAVLinkBridgelib's blocks only support Interpreted execution, which ruled out a Model
% Reference-based variant -- see CLAUDE_HITL.md). That means Simulink needs to resolve
% px4MAVLinkBridgelib to compile the model AT ALL, even for a pure-SITL run.
requiredSupportPackages = {'UAV Toolbox Support Package for PX4 Autopilots'};

toolboxList = matlab.addons.installedAddons;
for ii = 1:length(requiredTools)
    % Check if toolbox is installed & if license exists
    toolboxName = requiredTools{ii};
    toolboxName = replace(toolboxName, '_', ' '); % replace underscore with space to get names to match matlab.addons.installedAddons names
    if ~any(strcmpi(toolboxName, toolboxList.Name))
        if strcmpi(toolboxName, 'Simulink Test')
            warning(['Please install ' toolboxName])
        else
            error(['Please install the required toolbox:' toolboxName])
        end
        % Check license
        if ~license('test', requiredTools{ii})
            warning(['No license for ' toolboxName])
        end
    end
end

for ii = 1:length(requiredSupportPackages)
    if ~any(strcmpi(requiredSupportPackages{ii}, toolboxList.Name))
        error(['Please install the required support package: ' requiredSupportPackages{ii} ...
            ' (Add-On Manager, or matlab.addons.install). Required for controllerRuntime="HITL".'])
    end
end

try
    mex -setup cpp
catch 
    error("Currently, there's no C++ compiler installed. MinGW-w64 C/C++/Fortran Compiler can be installed from" + ...
        " using Matlab Add-Ons" )
end

% Initializes path, load bus definitions, and sets model
% parameters for vehicleSIL.slx
addpath(genpath('PX4-Autopilot'));
addpath(genpath('environment'));
addpath(genpath('sensors'));
addpath(genpath('data'));
addpath(genpath('signals'));
addpath(genpath('utilities'));
addpath(genpath('visualization'));
addpath(genpath('PX4SILConnector'));
addpath(genpath('PX4HITLConnector'));
addpath(genpath('vehicle/common'));
addpath(genpath('vehicle/hexarotor'));
addpath(genpath(vehicleBasePath(vehicleDefinition.BaseVehicleType)));
addpath(genpath(vehicleBasePath(vehicleDefinition.PlantModel)));
addpath(genpath(vehicleDefinition.VehiclePath));
compilerVehicleName = vehicleDefinition.PX4Target;

Simulink.fileGenControl('set', ...
    'CacheFolder', 'work', ...
    'CodeGenFolder', 'work', ...
    'createDir', true)

load('standardSILConfigurationParams.mat')
assignin('base', 'standardSILConfigurationParams', standardSILConfigurationParams);
stepSize_s = 0.004; % step size used in standardSILConfugrationParams.mat file

% Conversions
constants

% load bus definitions
% NOTE: dispatches on vehicleParams.type (the specific vehicle key), not baseVehicleType -- see
% PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/2. Every non-F16/hexarotor vehicle previously got
% the hexarotor's ServosBus/ServosCommandBus shape (6-motor mixer) via the baseVehicleType
% fallback, regardless of actuator type.
BusDefinition(vehicleParams.type)

% load vehicle specific data and initial conditions
setUpVehicle

% load sensor data
setUpSensors

%actuatorInterface
setUpActuators

% set up winds
setUpEnvironment

% Controller runtime selector read by VehicleSilSimulation's PX4 connector selector (mirrors the
% INS_VARIANT pattern in sensors/setUpSensors.m -- see CLAUDE.md/CLAUDE_HITL.md). 1 = SITL
% (existing pixhawk_sil_connector path, default), 2 = HITL (real PX4 hardware over serial). The
% connector outputs feed a Variant Source, so the inactive connector branch is not compiled during
% diagram update. The Enable ports are retained as a runtime guard, but they are not enough by
% themselves for the HITL MAVLink MATLAB System blocks because setupImpl can open serial at
% simulation start.
%
% setUpHITLConnection still runs unconditionally, because it puts px4MAVLinkBridgelib on the path
% so the saved model can resolve the HITL library blocks even when SITL is the active runtime.
if strcmpi(opts.controllerRuntime, "HITL")
    CONTROLLER_RUNTIME = 2;
else
    CONTROLLER_RUNTIME = 1;
end
setUpHITLConnection

% If no failureType selected, disable joystick failure injection.
% If failureType selected, set Joystick1 so failure can be injected into plant.
if strcmpi(opts.failureType,"none")
    modelName = 'VehicleSilSimulation';
    jsBlockPath = [modelName, '/Failure Injection/PilotJoystick'];
    warning('off', 'Simulink:ConfigSet:CannotFindConfigSet');
    load_system(modelName);
    set_param(jsBlockPath, 'JoystickID', 'None');
else
    modelName = 'VehicleSilSimulation';
    jsBlockPath = [modelName, '/Failure Injection/PilotJoystick'];
    load_system(modelName);
    set_param(jsBlockPath, 'JoystickID', 'Joystick1');
end
configureVehicleInterfaceVariants(modelName);
configureHitlRuntime(modelName, opts.controllerRuntime);

% Set the HITL serial port directly on both MAVLink Bridge blocks. Their PixhawkSerialPortManually
% mask parameter is a literal-string field (MaskVariables shows "&2", not "@2" -- Simulink's
% notation for "take as-is", not "evaluate as an expression"), so pointing it at a workspace
% variable name like 'hitlParams.serialPort' does NOT work -- confirmed by it literally trying to
% open a port named '\\.\hitlParams.serialPort'. Must set() the actual string here instead.
if strcmpi(opts.controllerRuntime, "HITL")
    hitlConnectorBlocks = {'PX4 HITL Interface/MAVLink Bridge Sink', 'PX4 HITL Interface/MAVLink Bridge Source'};
    for ii = 1:numel(hitlConnectorBlocks)
        set_param([modelName '/' hitlConnectorBlocks{ii}], 'PixhawkSerialPortManually', char(opts.hitlSerialPort));
    end
end

% Check failure type
%
% NOTE: this only had F-16/hexarotor branches, matched via baseVehicleType. Every other
% registered vehicle (ackermann_rover, differential_rover, etc.) has baseVehicleType forced to
% "hexarotor" (see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/F1), so it accidentally matched
% the hexarotor branch instead of erroring. fixedwing_plane uses its own name as
% baseVehicleType (same convention as F16), which fell through both branches silently, leaving
% vehicleParams.failureType as an unconverted string -- and later failed downstream at
% "VehicleSilSimulation/Failure Injection/Constant" ("Invalid setting ... for parameter
% 'Value'") instead of here, since neither branch here raises when nothing matches. Reuses
% EnumF16FailureType (not a new EnumFixedwingPlaneFailureType) because
% vehicle/fixedwing_plane/components/failureInputReadFixedwingPlane.slx is a verbatim clone of
% failureInputReadF16.slx and still compares against EnumF16FailureType internally -- see
% PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
try
    if strcmpi(vehicleParams.baseVehicleType,"F-16")
        vehicleParams.failureType = EnumF16FailureType(vehicleParams.failureType);
    elseif strcmpi(vehicleParams.baseVehicleType,"hexarotor")
        vehicleParams.failureType = EnumHexFailureType(vehicleParams.failureType);
    elseif strcmpi(vehicleParams.baseVehicleType,"fixedwing_plane")
        vehicleParams.failureType = EnumF16FailureType(vehicleParams.failureType);
    end
catch
    error("The selected failure type does not match the selected vehicle. Failure type must be an enum from " + ...
        "EnumHexFailureType.m or EnumF16FailureType.m")
end

% Prompt the user to assign a joystick button to the failure injection.
if ~strcmpi(opts.failureType,"none") &&  opts.assignFailureButton
    assignFailureInjectionButton
    opts.failureInjectionButton = failureInjectionButton;
else
    % Default failure injection button assignment.
    opts.failureInjectionButton = uint32(64);
end

% Save all workspace variables and push them to base workspace
save('workspace')
evalin("base", 'load workspace.mat')
delete workspace.mat

% Variant Models
if strcmpi(opts.visualizationType, 'Matlab')
    load_system('VehicleSilSimulation.slx')
    warning("When using Matlab visualization the SIL simulator runs slower than FlightGear. Recommend setting simulink model to" + ...
        " accelerator mode.")
    % 'UAV Animation' (Aerospace Blockset) only supports UAVType 'Multirotor' or
    % 'FixedWing' -- there is no third option to pick from, so ControllerType
    % "rover"/"boat"/"sub" vehicles are structurally unable to get a correct 3D
    % shape out of this block (see PLAN_CORRECCION_MULTIVEHICULO_SITL.md F5/Fase 4).
    % A real per-class visualization (Simscape Mechanics Explorer for
    % rover/tracked vehicles, MSS-style trajectory plots for boat/sub) is future
    % work -- not implemented here. Until then, keep the closest available
    % silhouette but make the mismatch loud instead of silent.
    switch lower(vehicleDefinition.ControllerType)
        case "multirotor"
            set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
                'Multirotor');
        case "fixedwing"
            set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
                'FixedWing');
        case "rover"
            set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
                'Multirotor');
            warning("initVehicleSIL:NoRoverVisualization", "'%s' is a ground vehicle " + ...
                "(ControllerType=rover) but 'Matlab' visualization is showing it as a " + ...
                "generic multirotor icon -- this is a KNOWN INCORRECT placeholder, not a " + ...
                "real representation of its shape (wheels/tracks/steering are not drawn). " + ...
                "See PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 4.", opts.vehicleType)
        case {"boat", "sub"}
            set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
                'FixedWing');
            warning("initVehicleSIL:NoMarineVisualization", "'%s' is a marine vehicle " + ...
                "(ControllerType=%s) but 'Matlab' visualization is showing it as a fixed-wing " + ...
                "aircraft icon -- this is a KNOWN INCORRECT placeholder, not a real " + ...
                "representation of its shape. See PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 4.", ...
                opts.vehicleType, vehicleDefinition.ControllerType)
        otherwise
            set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
                'FixedWing');
            warning("initVehicleSIL:UnknownControllerTypeVisualization", "'%s' has an " + ...
                "unrecognized ControllerType='%s' for 'Matlab' visualization purposes; " + ...
                "defaulting to a FixedWing icon as a KNOWN INCORRECT placeholder.", ...
                opts.vehicleType, string(vehicleDefinition.ControllerType))
    end
elseif strcmpi(opts.visualizationType, 'FlightGear')
    load_system('VehicleSilSimulation.slx')
    configureFlightGearTcpEndpoint('VehicleSilSimulation', opts.flightGearHost, opts.flightGearTelnetPort, opts.vehicleType);
    if ~opts.launchFullSIL && opts.launchPreflightVisualization
        if ~isTcpEndpointOpen(opts.flightGearHost, opts.flightGearTelnetPort)
            evalin("base", 'runFlightGear')
        end
        waitForFlightGearTcpEndpoint(opts.flightGearHost, opts.flightGearTelnetPort, 20, opts.vehicleType);
    end
end
% Launch full SIL/HITL if requested
if opts.launchFullSIL

    % Run flightgear if requested
    currLoc = pwd;
    if strcmpi(opts.visualizationType, "FlightGear")
        evalin("base", 'runFlightGear')
    end

    % Open QGC
    disp('Launch QGroundControl manually.');

    if strcmpi(opts.controllerRuntime, "SITL")
        wslDistro = resolveWSLDistro(opts.wslDistro);
        if strlength(wslDistro) > 0
            wslCommand = sprintf('wsl -d "%s"', wslDistro);
        else
            wslCommand = 'wsl';
        end

        % Get WSL IP address if simHostIP is set to auto
        if (opts.simHostIP) == "auto"
            getWSLIP
            opts.simHostIP = wslVEip;
        end
        opts.simHostIPVal = double(split(string(opts.simHostIP), '.'));

        % Launch PX4
        if opts.clearSLCache
            cacheFolderPath = 'work';

            if exist(cacheFolderPath, 'dir')
                fileTypes = {'*.slxc', '*.mexw64'};

                for k = 1:length(fileTypes)
                    files = dir(fullfile(cacheFolderPath, '**', fileTypes{k}));

                    for i = 1:length(files)
                        filePath = fullfile(files(i).folder, files(i).name);
                        try
                            delete(filePath);
                            fprintf('Deleted: %s\n', filePath);
                        catch ME
                            fprintf('Error deleting file: %s\nReason: %s\n', filePath, ME.message);
                        end
                    end
                end
            else
                fprintf('Folder "%s" does not exist.\n', cacheFolderPath);
            end
        end

        if ~opts.PX4InWSL
            % Launch PX4-Autopilot that is checked out on the Windows side
            eval(strcat("cd ", opts.PX4RepoPath))
            if opts.makeClean
                system(sprintf(['start "" %s bash -lc "export PX4_SIM_HOSTNAME=%d.%d.%d.%d && make clean && ' ...
                    'make px4_sitl_default %s"'],...
                    wslCommand, opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), opts.simHostIPVal(4), ...
                    compilerVehicleName ));
            else
                system(sprintf('start "" %s bash -lc "export PX4_SIM_HOSTNAME=%d.%d.%d.%d && make px4_sitl_default %s"',...
                    wslCommand, opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), opts.simHostIPVal(4), ...
                    compilerVehicleName ));
            end
        else
            if opts.makeClean
                % Launch PX4-Autopilot that is cloned into the WSL root directory
                system(sprintf(['start "" %s bash -lc "cd ~/%s && export PX4_SIM_HOSTNAME=%d.%d.%d.%d && ' ...
                    'make clean && make px4_sitl_default %s"'],...
                    wslCommand, opts.PX4RepoPath, opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), ...
                    opts.simHostIPVal(4), compilerVehicleName ));
            else
                % Launch PX4-Autopilot that is cloned into the WSL root directory
                system(sprintf(['start "" %s bash -lc "cd ~/%s && export PX4_SIM_HOSTNAME=%d.%d.%d.%d &&' ...
                    ' make px4_sitl_default %s"'],...
                    wslCommand, opts.PX4RepoPath, opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), ...
                    opts.simHostIPVal(4), compilerVehicleName ));
            end
        end

        % Check to make sure WSL is running. Pause is needed to provide time for system command to fail.
        pause(3)
        [~, result] = system('tasklist /FI "IMAGENAME eq wsl.exe"');
        if ~contains(result, 'wsl.exe')
            error('WSL/PX4 did not successfully launch, try launching PX4 manually and starting the VehicleSilSimulation.slx manually.')
        end
    else
        % HITL: do not launch WSL/PX4 SITL -- a real PX4 board does the controller/estimator work.
        % It must already be flashed and configured before this point; this just reminds the user
        % and lets the model run against PX4HITLConnector instead of PX4SILConnector.
        fprintf([...
            'HITL mode: not launching PX4 SITL in WSL.\n' ...
            '  1. Confirm %s is flashed with cubepilot_cubeorangeplus_default firmware (README.md).\n' ...
            '  2. Confirm it is connected on %s, powered, and SYS_HITL=1 is set in QGroundControl > Parameters.\n' ...
            '  3. Point QGroundControl at UDP port %d once the model is running to see telemetry.\n'], ...
            opts.hardwareTarget, opts.hitlSerialPort, opts.hitlQGCPort);
    end

    % Open simulink model
    cd(currLoc)
    open VehicleSilSimulation.slx

    if strcmpi(opts.visualizationType,"FlightGear")
        waitForFlightGearTcpEndpoint(opts.flightGearHost, opts.flightGearTelnetPort, 15, opts.vehicleType);
    end
    % Run simulink model
    sim VehicleSilSimulation
end

end

function distro = resolveWSLDistro(requestedDistro)
distro = strtrim(string(requestedDistro));
if ~strcmpi(distro, "auto")
    return
end

[status, output] = system('wsl --list --quiet');
if status ~= 0
    distro = "";
    return
end

% Windows PowerShell/MATLAB can expose wsl.exe's UTF-16 NUL bytes in the
% captured text. Remove them before comparing distribution names.
output = erase(string(output), char(0));
installed = strip(splitlines(output));
installed(installed == "") = [];
preferred = ["PX4Simulink", "Ubuntu-22.04"];
for ii = 1:numel(preferred)
    if any(strcmpi(installed, preferred(ii)))
        distro = preferred(ii);
        fprintf('Using WSL distribution %s for PX4 SITL.\n', distro);
        return
    end
end

warning('No PX4-specific WSL distribution found; using the default WSL distribution.')
distro = "";
end

function configureVehicleInterfaceVariants(modelName)
% NOTE (Fase 2, see PLAN_CORRECCION_MULTIVEHICULO_SITL.md): this now dispatches on
% vehicleParams.type (the specific vehicle key), not baseVehicleType. Previously every
% non-F16/hexarotor vehicle was forced through the Hex or F16 branches here via the
% baseVehicleType fallback -- this function runs on every initVehicleSIL() call and
% unconditionally overwrites each variant choice's VariantControl, so whatever is hardcoded
% here is authoritative regardless of what happens to be saved in the .slx file.
%
% This also fixes a latent bug: vehicleParams.type for F16 is "F16" (no hyphen, from
% vehicleRegistry.m's PlantModel field), not "F-16" -- the F-16-with-hyphen form only matches
% vehicleParams.baseVehicleType. Using baseVehicleType previously masked this because it was
% "F-16" (with hyphen) for the F16 vehicle itself; switching to .type without also fixing the
% string would have silently broken F16's own variant selection.
variantControls = {
    'Failure Injection/Variant Model/F16', ...
        'strcmpi(vehicleParams.type, "F16")'
    'Failure Injection/Variant Model/hexarotor', ...
        'strcmpi(vehicleParams.type, "hexarotor")'
    'Failure Injection/Variant Model/ackermann_rover', ...
        'strcmpi(vehicleParams.type, "ackermann_rover")'
    'Failure Injection/Variant Model/ackermann_simscape', ...
        'strcmpi(vehicleParams.type, "ackermann_simscape")'
    'Failure Injection/Variant Model/differential_rover', ...
        'strcmpi(vehicleParams.type, "differential_rover")'
    'Failure Injection/Variant Model/tracked_vehicle', ...
        'strcmpi(vehicleParams.type, "tracked_vehicle")'
    'Failure Injection/Variant Model/tracked_vehicle_simscape', ...
        'strcmpi(vehicleParams.type, "tracked_vehicle_simscape")'
    'Failure Injection/Variant Model/usv_surface', ...
        'strcmpi(vehicleParams.type, "usv_surface")'
    'Failure Injection/Variant Model/uuv_subsea', ...
        'strcmpi(vehicleParams.type, "uuv_subsea")'
    'Failure Injection/Variant Model/bicycle_rover', ...
        'strcmpi(vehicleParams.type, "bicycle_rover")'
    'Failure Injection/Variant Model/wheel_loader', ...
        'strcmpi(vehicleParams.type, "wheel_loader")'
    'Failure Injection/Variant Model/wheel_loader_simscape', ...
        'strcmpi(vehicleParams.type, "wheel_loader_simscape")'
    'Failure Injection/Variant Model/unicycle_rover', ...
        'strcmpi(vehicleParams.type, "unicycle_rover")'
    'Failure Injection/Variant Model/quadrotor', ...
        'strcmpi(vehicleParams.type, "quadrotor")'
    'Failure Injection/Variant Model/octarotor', ...
        'strcmpi(vehicleParams.type, "octarotor")'
    'Failure Injection/Variant Model/evtol', ...
        'strcmpi(vehicleParams.type, "evtol")'
    'Failure Injection/Variant Model/evtol_simscape', ...
        'strcmpi(vehicleParams.type, "evtol_simscape")'
    'PX4 Interface/Command Output Variaint/F16 Output Mapping', ...
        'strcmpi(vehicleParams.type, "F16") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Hex Output Mapping', ...
        'strcmpi(vehicleParams.type, "hexarotor") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Ackermann Rover Output Mapping', ...
        'strcmpi(vehicleParams.type, "ackermann_rover") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/AckermannSimscape Output Mapping', ...
        'strcmpi(vehicleParams.type, "ackermann_simscape") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Differential Rover Output Mapping', ...
        'strcmpi(vehicleParams.type, "differential_rover") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Tracked Vehicle Output Mapping', ...
        'strcmpi(vehicleParams.type, "tracked_vehicle") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/TrackedVehicleSimscape Output Mapping', ...
        'strcmpi(vehicleParams.type, "tracked_vehicle_simscape") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Usv Surface Output Mapping', ...
        'strcmpi(vehicleParams.type, "usv_surface") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Uuv Subsea Output Mapping', ...
        'strcmpi(vehicleParams.type, "uuv_subsea") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Bicycle Rover Output Mapping', ...
        'strcmpi(vehicleParams.type, "bicycle_rover") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Wheel Loader Output Mapping', ...
        'strcmpi(vehicleParams.type, "wheel_loader") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/WheelLoaderSimscape Output Mapping', ...
        'strcmpi(vehicleParams.type, "wheel_loader_simscape") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Unicycle Rover Output Mapping', ...
        'strcmpi(vehicleParams.type, "unicycle_rover") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Quadrotor Output Mapping', ...
        'strcmpi(vehicleParams.type, "quadrotor") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Octarotor Output Mapping', ...
        'strcmpi(vehicleParams.type, "octarotor") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/Evtol Output Mapping', ...
        'strcmpi(vehicleParams.type, "evtol") & strcmpi(vehicleParams.controllerType, "PX4")'
    'PX4 Interface/Command Output Variaint/EvtolSimscape Output Mapping', ...
        'strcmpi(vehicleParams.type, "evtol_simscape") & strcmpi(vehicleParams.controllerType, "PX4")'
    'Failure Injection/Variant Model/fixedwing_plane', ...
        'strcmpi(vehicleParams.type, "fixedwing_plane")'
    'PX4 Interface/Command Output Variaint/Fixedwing Plane Output Mapping', ...
        'strcmpi(vehicleParams.type, "fixedwing_plane") & strcmpi(vehicleParams.controllerType, "PX4")'
    };

for ii = 1:size(variantControls, 1)
    blockPath = [modelName '/' variantControls{ii, 1}];
    if getSimulinkBlockHandle(blockPath) ~= -1
        set_param(blockPath, 'VariantControl', variantControls{ii, 2});
    end
end
end

function configureHitlRuntime(modelName, controllerRuntime)
if ~bdIsLoaded(modelName)
    load_system(modelName);
end

hitlSubsystem = [modelName '/PX4 HITL Interface'];
if ~strcmp(get_param(hitlSubsystem, 'Type'), 'block')
    return
end

if strcmpi(controllerRuntime, "HITL")
    set_param(hitlSubsystem, 'Commented', 'off');
else
    set_param(hitlSubsystem, 'Commented', 'on');
end
end

function configureFlightGearTcpEndpoint(modelName, host, port, vehicleType)
if ~bdIsLoaded(modelName)
    load_system(modelName);
end

if ~strcmpi(vehicleType, "hexarotor")
    return
end

fgSubsystem = [modelName '/visualizationVariant/FlightGearVisualizationHexarotor'];
tcpBlocks = find_system(fgSubsystem, ...
    'LookUnderMasks', 'all', ...
    'FollowLinks', 'on', ...
    'BlockType', 'Reference');

for ii = 1:numel(tcpBlocks)
    try
        if strcmp(get_param(tcpBlocks{ii}, 'SourceType'), 'instrument.system.TCPIPSend')
            set_param(tcpBlocks{ii}, 'Host', char(host), 'Port', num2str(port));
        end
    catch
    end
end
end

function waitForFlightGearTcpEndpoint(host, port, timeout_s, vehicleType)
if ~strcmpi(vehicleType, "hexarotor")
    return
end

tStart = tic;
isTCPOpen = false;
while toc(tStart) < timeout_s && ~isTCPOpen
    isTCPOpen = isTcpEndpointOpen(host, port);
    if ~isTCPOpen
        pause(2);
    end
end

if ~isTCPOpen
    error("FlightGear hexarotor TCP endpoint is not open at %s:%d. Check that FlightGear launched successfully.", ...
        char(host), port)
end
end

function isOpen = isTcpEndpointOpen(host, port)
isOpen = false;
try
    client = tcpclient(char(host), port); %#ok<NASGU>
    clear client
    isOpen = true;
catch
end
end

function pathName = vehicleBasePath(baseVehicleType)
switch lower(string(baseVehicleType))
    case "f-16"
        pathName = "vehicle/F16";
    case "hexarotor"
        pathName = "vehicle/hexarotor";
    case "evtol_simscape"
        pathName = "vehicle/evtol_simscape";
    case "tracked_vehicle_simscape"
        pathName = "vehicle/tracked_vehicle_simscape";
    case "wheel_loader_simscape"
        pathName = "vehicle/wheel_loader_simscape";
    case "ackermann_simscape"
        pathName = "vehicle/ackermann_simscape";
    otherwise
        pathName = "vehicle/" + string(baseVehicleType);
end
end
