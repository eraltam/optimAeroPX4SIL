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
vehicleParams.type                   = opts.vehicleType;
vehicleParams.controllerType         = opts.controllerType;
vehicleParams.failureType            = opts.failureType;
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

if strcmpi(vehicleParams.type, "F-16")
    addpath(genpath('vehicle/F16'));
    compilerVehicleName = "optimAeroF16";
elseif strcmpi(vehicleParams.type, "hexarotor")
    addpath(genpath('vehicle/hexarotor'))
    compilerVehicleName = "optimAeroHex";
end

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
try
    if strcmpi(vehicleParams.type,"F-16")
        vehicleParams.failureType = EnumF16FailureType(vehicleParams.failureType);
    elseif strcmpi(vehicleParams.type,"hexarotor")
        vehicleParams.failureType = EnumHexFailureType(vehicleParams.failureType);
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
    if strcmpi(vehicleParams.type,'Hexarotor')
        set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
            'Multirotor');
    else
        set_param('VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation', 'UAVType', ...
            'FixedWing');
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
                system(sprintf(['start wsl bash -c "export PX4_SIM_HOSTNAME=%d.%d.%d.%d && make clean && ' ...
                    'make px4_sitl_default %s"'],...
                    opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), opts.simHostIPVal(4), ...
                    compilerVehicleName ));
            else
                system(sprintf('start wsl bash -c "export PX4_SIM_HOSTNAME=%d.%d.%d.%d && make px4_sitl_default %s"',...
                    opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), opts.simHostIPVal(4), ...
                    compilerVehicleName ));
            end
        else
            if opts.makeClean
                % Launch PX4-Autopilot that is cloned into the WSL root directory
                system(sprintf(['start wsl bash -c "cd ~/%s && export PX4_SIM_HOSTNAME=%d.%d.%d.%d && ' ...
                    'make clean && make px4_sitl_default %s"'],...
                    opts.PX4RepoPath, opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), ...
                    opts.simHostIPVal(4), compilerVehicleName ));
            else
                % Launch PX4-Autopilot that is cloned into the WSL root directory
                system(sprintf(['start wsl bash -c "cd ~/%s && export PX4_SIM_HOSTNAME=%d.%d.%d.%d &&' ...
                    ' make px4_sitl_default %s"'],...
                    opts.PX4RepoPath, opts.simHostIPVal(1), opts.simHostIPVal(2), opts.simHostIPVal(3), ...
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
