function [] = testVehicleSIL()
% testVehicleSIL.m Conducts tests on VehicleSIL.slx
% Testing:
% 1. Compile testing on all components
% 2. Unit tests defined by test harnesses and mldatx test definitions
%
% TestResults.pdf summarizes results of unit testing 
%=================================
% Inputs
%=================================
% none
%=================================
% OUTPUTS
%=================================
% none
%=================================
% EXAMPLE USAGE
% =================================
% testVehicleSIL()
% testVehicleSIL('rebuildHarnesses', true)

disp('Test Vehicle SIL Model')

clear all
testTimer = tic;
bdclose('all')
hasSimulinkTest = license('test', 'Simulink_Test') && exist('sltest.testmanager.clear', 'file') == 2;
if hasSimulinkTest
    sltest.testmanager.clear
else
    warning('Simulink Test is not available. Compile and plant simulation will run; .mldatx unit tests will be skipped.')
end

%% Change the cache folders to non-default locations
cfg = Simulink.fileGenControl('getConfig');
currentCacheFolder = cfg.CacheFolder;
fullpathHere = mfilename('fullpath');
[pathHere, ~, ~] = fileparts(fullpathHere);
workFolder = fullfile(pathHere, 'work');
cfg.CacheFolder = workFolder;

if ~exist(workFolder, 'dir')
    mkdir(workFolder)
end
Simulink.fileGenControl('setConfig', 'config', cfg);

Simulink.fileGenControl('setConfig', 'config', cfg);

try
    %% Compile Models
    disp('Compiling models...')
    %vehicle
    try
        vehicleParams = evalin('base', 'vehicleParams');
        vehicleDefinition = resolveSelectedVehicle(vehicleParams);
        compileModel = erase(vehicleDefinition.CompileModel, '-');
        eval(strcat(compileModel, '([], [], [], ''compile'')'))
        eval(strcat(compileModel, '([], [], [], ''term'')'))
    catch ME
        warning('Error compiling vehicle model.')
        rethrow(ME)
    end
    
    %sensors
    try
        sensors([], [], [], 'compile')
        sensors([], [], [], 'term')
    catch ME
        warning('Error compiling sensors model.')
        rethrow(ME)
    end
    
    %environment
    try
        environment([], [], [], 'compile')
        environment([], [], [], 'term')
    catch ME
        warning('Error compiling environment model.')
        rethrow(ME)
    end
    
    %Vehicle Plant
    try
        VehiclePlant([], [], [], 'compile')
        VehiclePlant([], [], [], 'term')
    catch ME
        warning('Error compiling Vehicle Plant model.')
        rethrow(ME)
    end
    
    % sim Vehicle Plant
    try
        paramStruct.StopTime = "1";
        sim("VehiclePlant", paramStruct);
    catch ME
        warning('Error executing Vehicle Plant model.')
        rethrow(ME)
    end
    
    bdclose('all')
    
    disp('Finished compiling models...')
    

    %% Unit testing 
    vehicleKey = vehicleDefinition.VehicleKey;
    if hasSimulinkTest
        disp('Unit testing starting...')

        disp('Rebuilding test harnesses...')
        rebuildHarness('all', 'pathRootDir', pathHere)
        testDefinition = char(vehicleDefinition.TestDefinition);
        if exist(testDefinition, 'file')
            sltest.testmanager.load(testDefinition);
        else
            warning('Skipping vehicle-specific test definition because %s is not on the MATLAB path.', testDefinition)
        end
        sltest.testmanager.load('sensors.mldatx');
        sltest.testmanager.load('environment.mldatx');
        results = sltest.testmanager.run;
    else
        results = struct('NumPassed', 1, 'NumTotal', 1);
    end
    
    % reporting
    testResultsPath = strrep(mfilename("fullpath"), 'testVehicleSIL', 'testResults' + vehicleKey + '.pdf');
    if exist(testResultsPath, 'file')
        eval('delete ' + testResultsPath)
    end
    
    % get branch and current commit hash
    [~, gitBranchString] = system('git rev-parse --abbrev-ref HEAD');
    [~, gitHashString] = system('git rev-parse HEAD');
    titleString = string(['Branch: ', gitBranchString]);
    authorString = string(['Previous Commit Hash: ', gitHashString]);
    
    if results.NumPassed == results.NumTotal
        if hasSimulinkTest
            disp('Unit testing passed!')
            disp('Printing results....')
            sltest.testmanager.report(results, testResultsPath, 'LaunchReport', false, 'IncludeTestResults', 0, 'Title', titleString, 'Author', authorString);
            sltest.testmanager.clearResults
            errorText = "";
        else
            errorText = "Simulink Test unavailable; skipped .mldatx unit tests after successful compile and plant simulation.";
        end
        writeVehicleTestResult(vehicleDefinition, toc(testTimer), NaN, true, errorText)
    else
        sltest.testmanager.report(results, testResultsPath, 'LaunchReport', false, 'Title', titleString, 'Author', authorString);
        error(['Unit testing failed! ' '<a href="matlab: sltest.testmanager.view">Click here to view results</a>' ' or view them in testResults.pdf'])
    end

    %% Change the cache folders to original locations
    cfg.CacheFolder = currentCacheFolder;
    Simulink.fileGenControl('setConfig', 'config', cfg);

    %% Clean up and initialize before exiting
    bdclose('all')
    evalin('base', sprintf('initVehicleSIL("vehicleType", "%s")', vehicleKey))
    
catch ME
    try
        vehicleParamsForLog = evalin('base', 'vehicleParams');
        vehicleDefinitionForLog = resolveSelectedVehicle(vehicleParamsForLog);
        writeVehicleTestResult(vehicleDefinitionForLog, toc(testTimer), NaN, false, string(getReport(ME, 'extended', 'hyperlinks', 'off')))
    catch
    end

    %% Change the cache folders to original locations
    cfg.CacheFolder = currentCacheFolder;
    Simulink.fileGenControl('setConfig', 'config', cfg);

    %% Clean up and initialize before exiting
    bdclose('all')
    evalin('base', 'initVehicleSIL')

    %let user know of orginal error
    rethrow(ME)
end

end

function vehicleDefinition = resolveSelectedVehicle(vehicleParams)
if isfield(vehicleParams, 'registryEntry')
    vehicleDefinition = vehicleParams.registryEntry;
elseif isfield(vehicleParams, 'vehicleKey')
    vehicleDefinition = vehicleRegistry(vehicleParams.vehicleKey);
else
    vehicleDefinition = vehicleRegistry(vehicleParams.type);
end
end

function writeVehicleTestResult(vehicleDefinition, simulationDuration, finalWaypointError, passed, errorText)
resultsFolder = fullfile(fileparts(mfilename('fullpath')), 'work', 'vehicle_test_results');
if ~exist(resultsFolder, 'dir')
    mkdir(resultsFolder)
end

[~, gitHash] = system('git rev-parse HEAD');
result = struct();
result.vehicleKey = char(vehicleDefinition.VehicleKey);
result.timestamp = char(datetime('now', 'Format', 'yyyy-MM-dd HH:mm:ss'));
result.gitCommitOrTimestamp = strtrim(gitHash);
if strlength(string(result.gitCommitOrTimestamp)) == 0
    result.gitCommitOrTimestamp = result.timestamp;
end
result.matlabRelease = version('-release');
result.px4Target = char(vehicleDefinition.PX4Target);
result.buildCommand = char("make px4_sitl_default " + vehicleDefinition.PX4Target);
result.buildExitCode = NaN;
result.simulationDuration = simulationDuration;
result.finalWaypointError = finalWaypointError;
result.passed = logical(passed);
result.errorText = char(errorText);

fileName = sprintf('%s_%s.json', result.vehicleKey, char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
fid = fopen(fullfile(resultsFolder, fileName), 'w');
cleanup = onCleanup(@() fclose(fid));
fprintf(fid, '%s', jsonencode(result, 'PrettyPrint', true));
end
