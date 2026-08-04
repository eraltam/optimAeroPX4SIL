function sync_jsbsim_unpack_chart(vehicleType)
%SYNC_JSBSIM_UNPACK_CHART Copy reviewed unpack source into all executable copies.
arguments
    vehicleType (1,1) string {mustBeMember(vehicleType, ...
        ["c172pJSBSim","c130JSBSim","f22JSBSim"])}
end

switch vehicleType
    case "c172pJSBSim"
        sourceNames = "unpackJSBSimOutputs.m";
        chartNames = "unpackJSBSimOutputs";
    case "c130JSBSim"
        sourceNames = ["unpackC130JSBSimOutputs.m","packC130JSBSimInputs.m"];
        chartNames = ["unpackC130JSBSimOutputs","packC130JSBSimInputs"];
    case "f22JSBSim"
        sourceNames = ["unpackF22JSBSimOutputs.m", "packF22JSBSimInputs.m"];
        chartNames = ["unpackF22JSBSimOutputs", "packF22JSBSimInputs"];
end

repoRoot = fileparts(fileparts(fileparts(mfilename("fullpath"))));
modelPaths = [ ...
    fullfile(repoRoot, "vehicle", vehicleType, vehicleType + ".slx"); ...
    fullfile(repoRoot, "VehiclePlant.slx"); ...
    fullfile(repoRoot, "VehicleSilSimulation.slx")];

for modelPath = modelPaths.'
    [~, modelName] = fileparts(modelPath);
    open_system(modelPath);
    machine = sfroot().find("-isa", "Stateflow.Machine", "Name", modelName);
    assert(isscalar(machine), "Expected one Stateflow machine for %s.", modelName);
    charts = machine.find("-isa", "Stateflow.EMChart");
    for sourceIndex = 1:numel(sourceNames)
        sourcePath = fullfile(repoRoot, "vehicle", vehicleType, "components", sourceNames(sourceIndex));
        sourceText = fileread(sourcePath);
        matches = charts(strcmp(string({charts.Name}), chartNames(sourceIndex)));
        assert(~isempty(matches), "No %s chart found in %s.", chartNames(sourceIndex), modelName);
        for chart = matches(:).'
            chart.Script = sourceText;
        end
        save_system(modelName);
        for chart = matches(:).'
            assert(strcmp(chart.Script, sourceText), ...
                "Chart %s does not match reviewed source after save.", chart.Path);
            fprintf("Synced and verified %s from %s.\n", chart.Path, sourcePath);
        end
    end
end
end
