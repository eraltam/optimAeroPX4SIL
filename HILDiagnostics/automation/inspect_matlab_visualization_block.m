%INSPECT_MATLAB_VISUALIZATION_BLOCK Print tunable UAV Animation parameters.
repoRoot = fileparts(fileparts(fileparts(mfilename("fullpath"))));
cd(repoRoot);
load_system("VehicleSilSimulation");
block = "VehicleSilSimulation/visualizationVariant/MatlabVisualization/UAV Animation";
fprintf("BLOCK=%s\n", block);
disp(get_param(block, "DialogParameters"));
fprintf("SampleTime=%s\n", string(get_param(block, "SampleTime")));
fprintf("MaskType=%s\n", string(get_param(block, "MaskType")));
close_system("VehicleSilSimulation", 0);
