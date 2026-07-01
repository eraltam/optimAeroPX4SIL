%% wire_ins_variant.m
% Replaces sensors/sensors.slx's 'ins' Model Reference block with a selectable
% subsystem offering two choices, both computed every step, switched via a
% Multiport Switch driven by the base-workspace double INS_VARIANT:
%   INS_VARIANT==1 -> Generic  -> components/ins.slx        (existing baseline)
%   INS_VARIANT==2 -> AnelloX3 -> components/ins_anello.slx (ANELLO X3 datasheet model)
%
% A true "Variant Subsystem" library block was attempted first but this MATLAB
% release's variant-choice port-synchronization (children's port set must match
% the parent exactly, and there is no reliable programmatic API found to force
% that sync — see optimAeroPX4SIL/CLAUDE.md) made it unreliable to build
% headlessly. A Multiport Switch is simpler, fully supported for bus signals,
% and gives the same practical outcome (switch via one variable, defined in
% sensors/setUpSensors.m), at the cost of always computing both INS models
% every step (negligible — both are lightweight).
%
% Prerequisites: bus objects must be defined in the base workspace
% (BusDefinition) before running this script.

mdl = 'sensors';
if ~bdIsLoaded(mdl)
    load_system(fullfile(fileparts(fileparts(mfilename('fullpath'))), 'sensors.slx'));
end

oldBlk = [mdl '/ins'];
pos = get_param(oldBlk, 'Position');

% ---- Capture existing connections before deleting -----------------------
inLines  = get_param(oldBlk, 'LineHandles');
outLine  = inLines.Outport(1);
dstBlock = get_param(outLine, 'DstBlockHandle');
dstPort  = get_param(outLine, 'DstPortHandle');
dstPortNum = get_param(dstPort, 'PortNumber');
dstBlockPath = getfullname(dstBlock);

srcPorts = cell(2,1);
for i = 1:2
    lh = inLines.Inport(i);
    srcPortH = get_param(lh, 'SrcPortHandle');
    srcPortNum = get_param(srcPortH, 'PortNumber');
    srcBlockH = get_param(lh, 'SrcBlockHandle');
    srcBlockRel = strrep(getfullname(srcBlockH), [mdl '/'], '');
    srcPorts{i} = [srcBlockRel '/' num2str(srcPortNum)];
end
dstBlockRel = strrep(dstBlockPath, [mdl '/'], '');
dstPortSpec = [dstBlockRel '/' num2str(dstPortNum)];

delete_line(mdl, srcPorts{1}, 'ins/1');
delete_line(mdl, srcPorts{2}, 'ins/2');
delete_line(mdl, 'ins/1', dstPortSpec);
delete_block(oldBlk);

% ---- Build selector subsystem in its place -------------------------------
add_block('built-in/SubSystem', oldBlk, 'Position', pos);

add_block('simulink/Sources/In1',  [oldBlk '/EnvironmentBus'], 'Position', [40, 60, 70, 74], 'Port', '1');
add_block('simulink/Sources/In1',  [oldBlk '/BodyStatesBus'],  'Position', [40, 220, 70, 234], 'Port', '2');
add_block('simulink/Sources/Constant', [oldBlk '/INS_VARIANT'], 'Position', [40, 140, 70, 154], 'Value', 'INS_VARIANT');
add_block('simulink/Ports & Subsystems/Model', [oldBlk '/Generic'], ...
    'Position', [150, 40, 280, 100], 'ModelName', 'ins.slx');
add_block('simulink/Ports & Subsystems/Model', [oldBlk '/AnelloX3'], ...
    'Position', [150, 190, 280, 250], 'ModelName', 'ins_anello.slx');
add_block('simulink/Signal Routing/Multiport Switch', [oldBlk '/INS_Select'], 'Position', [330, 90, 380, 200]);
add_block('simulink/Sinks/Out1',   [oldBlk '/INSSensorBus'],   'Position', [430, 130, 460, 144], 'Port', '1');

add_line(oldBlk, 'EnvironmentBus/1', 'Generic/1', 'autorouting', 'on');
add_line(oldBlk, 'BodyStatesBus/1',  'Generic/2', 'autorouting', 'on');
add_line(oldBlk, 'EnvironmentBus/1', 'AnelloX3/1', 'autorouting', 'on');
add_line(oldBlk, 'BodyStatesBus/1',  'AnelloX3/2', 'autorouting', 'on');
add_line(oldBlk, 'INS_VARIANT/1', 'INS_Select/1', 'autorouting', 'on');
add_line(oldBlk, 'Generic/1',  'INS_Select/2', 'autorouting', 'on');
add_line(oldBlk, 'AnelloX3/1', 'INS_Select/3', 'autorouting', 'on');
add_line(oldBlk, 'INS_Select/1', 'INSSensorBus/1', 'autorouting', 'on');

% ---- Reconnect the selector subsystem into the parent sensors.slx -------
add_line(mdl, srcPorts{1}, 'ins/1', 'autorouting', 'on');
add_line(mdl, srcPorts{2}, 'ins/2', 'autorouting', 'on');
add_line(mdl, 'ins/1', dstPortSpec, 'autorouting', 'on');

Simulink.BlockDiagram.arrangeSystem(oldBlk);
save_system(mdl);
fprintf('sensors.slx updated: ins now selects Generic/AnelloX3 via INS_VARIANT.\n');
