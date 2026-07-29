function run_c172p_sil()
% Headless launcher for the c172p SITL plant (connects to PX4 running in WSL).
% Logs progress to HILDiagnostics/logs/matlab_c172p_run.log and saves logsout at the end.
logdir = 'C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL\HILDiagnostics\logs';
if ~exist(logdir,'dir'); mkdir(logdir); end
diary(fullfile(logdir,'matlab_c172p_run.log')); diary on;
fprintf('[%s] === run_c172p_sil start ===\n', datestr(now));
try
    cd('C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL');
    initVehicleSIL("launchFullSIL", false, "vehicleType", "c172p", ...
        "controllerRuntime", "SITL", "visualizationType", "PassThrough", "clearSLCache", false);
    fprintf('[%s] initVehicleSIL done\n', datestr(now));
    scp = evalin('base','standardSILConfigurationParams');
    set_param(scp,'SignalLogging','on','SignalLoggingName','logsout');
    set_param('VehicleSilSimulation','StopTime','400');
    fprintf('[%s] starting sim -- connector now listening on 4560 for PX4\n', datestr(now));
    set_param('VehicleSilSimulation','SimulationCommand','start');
    t0=tic; laststat='';
    while true
        st = get_param('VehicleSilSimulation','SimulationStatus');
        if ~strcmp(st,laststat); fprintf('[%s] sim status: %s\n', datestr(now), st); laststat=st; end
        if strcmp(st,'stopped'); break; end
        if toc(t0) > 1500
            fprintf('[%s] TIMEOUT -> stopping sim\n', datestr(now));
            set_param('VehicleSilSimulation','SimulationCommand','stop'); pause(3); break;
        end
        pause(2);
    end
    fprintf('[%s] sim ended (status=%s)\n', datestr(now), get_param('VehicleSilSimulation','SimulationStatus'));
    try
        lo = evalin('base','logsout'); %#ok<NASGU>
        save(fullfile(logdir,'c172p_sil_logsout.mat'),'lo','-v7.3');
        fprintf('[%s] saved logsout -> c172p_sil_logsout.mat\n', datestr(now));
    catch e2
        fprintf('[%s] no logsout saved: %s\n', datestr(now), e2.message);
    end
catch e
    fprintf('[%s] ERROR: %s\n', datestr(now), getReport(e));
end
fprintf('[%s] === run_c172p_sil end ===\n', datestr(now));
diary off;
end
