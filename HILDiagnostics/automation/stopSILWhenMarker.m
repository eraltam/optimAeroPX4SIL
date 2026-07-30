function stopSILWhenMarker(modelName, markerPath)
%STOPSILWHENMARKER Stop a running SIL model after the flight harness succeeds.
%
% The Python mission harness writes markerPath only after the explicit landing
% item completes and the vehicle reports landed. Stopping through Simulink's
% normal SimulationCommand path returns a valid SimulationOutput, allowing the
% paired .mat capture to contain the full logged signals.

if ~isfile(markerPath)
    return;
end

try
    if bdIsLoaded(modelName) && ...
            strcmp(get_param(modelName, "SimulationStatus"), "running")
        set_param(modelName, "SimulationCommand", "stop");
    end
catch stopError
    warning("HILDiagnostics:StopAfterMissionFailed", ...
        "Could not stop %s after mission success: %s", ...
        modelName, stopError.message);
end
end
