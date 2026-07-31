% otto_diag_build.m -- diagnose why PX4OutputMappingC172pJSBSim_msf.mexw64 fails to build.
repoRoot = "C:\Users\Edison Altamirano\SITLV2\AnelloSummer\optimAeroPX4SIL";
cd(repoRoot); addpath(fullfile(repoRoot, "HILDiagnostics"));

try
    ccC = mex.getCompilerConfigurations('C','Selected');
    if isempty(ccC), fprintf('C_COMPILER=NONE\n'); else, fprintf('C_COMPILER=%s | %s\n', ccC(1).Name, ccC(1).Version); end
catch e, fprintf('C_COMPILER_ERR=%s\n', e.message); end
try
    ccX = mex.getCompilerConfigurations('C++','Selected');
    if isempty(ccX), fprintf('CPP_COMPILER=NONE\n'); else, fprintf('CPP_COMPILER=%s | %s\n', ccX(1).Name, ccX(1).Version); end
catch e, fprintf('CPP_COMPILER_ERR=%s\n', e.message); end

initVehicleSIL("launchFullSIL", false, "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", "visualizationType", "PassThrough");
assignin("base","INS_VARIANT",2);

fprintf('=== attempting slbuild PX4OutputMappingC172pJSBSim (ModelReferenceSimTarget) ===\n');
try
    slbuild('PX4OutputMappingC172pJSBSim','ModelReferenceSimTarget');
    fprintf('SLBUILD_OK\n');
catch e
    fprintf(2,'SLBUILD_FAIL: %s\n', e.message);
    fprintf(2,'%s\n', e.getReport('extended','hyperlinks','off'));
end
