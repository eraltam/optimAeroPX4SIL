%% wire_px4_hitl_selector.m
% Adds PX4HITLConnector.slx as a selectable alternative to VehicleSilSimulation's existing
% "PX4 Interface" (SITL) subsystem, switched by the base-workspace double CONTROLLER_RUNTIME
% (1=SITL, default; 2=HITL) set by initVehicleSIL.m when controllerRuntime="HITL".
%
% Architecture notes (see CLAUDE_HITL.md for the full story):
%   - Uses a Variant Source at the connector output, not a Multiport Switch. This is critical:
%     a Multiport Switch keeps both connectors in the compiled graph, so side-effecting connector
%     blocks can initialize even when their output is not selected. The Variant Source makes the
%     inactive connector branch inactive during diagram update/compile.
%   - PX4HITLConnector is INLINED as a plain Subsystem, not referenced via a Model block:
%     px4MAVLinkBridgelib's MAVLink Bridge Source/Sink blocks declare
%     getSimulateUsingImpl = "Interpreted execution" only, which is incompatible with Model
%     Reference blocks entirely (even SimulationMode="Normal" still triggers a sim-target build
%     attempt) -- confirmed by direct compile error ("class does not support code generation").
%     Inlining avoids that; it's also how the existing "PX4 Interface" (pixhawk_sil_connector) is
%     already built, so this matches established precedent in this repo.
%   - BOTH "PX4 HITL Interface" and "PX4 Interface" still have Enable ports, gated on
%     CONTROLLER_RUNTIME==2 and ==1 respectively, as a second runtime guard. The compile-time
%     selection comes from the Variant Source; the Enable ports alone are not sufficient for the
%     HITL MATLABSystem blocks, whose setupImpl can open the serial channel at simulation start.
%   - The SITL side needed a source change to work the same way: pixhawk_sil_connector.cpp is a
%     raw S-Function, not a MATLABSystem -- its blocking TCP accept() originally happened in
%     mdlStart, which (unlike MATLABSystem's setupImpl) is NOT skipped by Enabled Subsystem
%     semantics (mdlStart runs once for every block instance in the compiled diagram, regardless
%     of enable state). Fixed by moving the connect logic from mdlStart to the first mdlOutputs
%     call (see PX4SILConnector/pixhawk_sil_connector.cpp) -- mdlOutputs of a disabled Enabled
%     Subsystem's content IS skipped, so this now correctly never connects during a HITL run.
%     Confirmed empirically (not just by compiling): ran the model with CONTROLLER_RUNTIME=2 and
%     it failed after ~20s trying to open the (nonexistent, no real hardware) COM5 serial port --
%     no "Waiting for PX4 to connect on TCP port 4560..." ever printed.
%
% Prerequisites: same as build_PX4HITLConnector.m (bus objects, PX4OutputMappingHex.slx on path),
% plus PX4HITLConnector.slx must already be built (run build_PX4HITLConnector.m first).

mdl = 'VehicleSilSimulation';
if ~bdIsLoaded(mdl)
    load_system(fullfile(fileparts(fileparts(mfilename('fullpath'))), 'VehicleSilSimulation.slx'));
end

sitlBlk = [mdl '/PX4 Interface'];
pos = get_param(sitlBlk, 'Position');
w = pos(3) - pos(1);
h = pos(4) - pos(2);

% ---- Build "PX4 HITL Interface" as an inlined, Enable-gated Subsystem -----------------------
hitlBlk = [mdl '/PX4 HITL Interface'];
add_block('built-in/SubSystem', hitlBlk, 'Position', [pos(1), pos(2)+200, pos(1)+w, pos(2)+200+h]);
Simulink.BlockDiagram.copyContentsToSubSystem('PX4HITLConnector', hitlBlk);

add_block('simulink/Ports & Subsystems/Enable', [hitlBlk '/Enable']);
% Variable-size signals inside (the MAVLink byte streams) need their size resolved at every step,
% not just when the subsystem transitions from disabled to enabled -- see the explicit error
% message Simulink gives without this ("output signal sizes ... depend on input signal values").
set_param([hitlBlk '/Enable'], 'PropagateVarSize', 'During execution');

% Branch additional lines from the same sources feeding "PX4 Interface" -- Simulink allows multiple
% destinations from one source port, so the existing SITL wiring is untouched.
add_line(mdl, 'Rate Transition1/1', 'PX4 HITL Interface/1', 'autorouting', 'on');
add_line(mdl, 'Rate Transition2/1', 'PX4 HITL Interface/2', 'autorouting', 'on');
add_line(mdl, 'Rate Transition3/1', 'PX4 HITL Interface/3', 'autorouting', 'on');

% ---- Selector: CONTROLLER_RUNTIME picks which connector's output reaches the plant -----------
add_block('simulink/Sources/Constant', [mdl '/CONTROLLER_RUNTIME_sel'], ...
    'Position', [pos(1)+200, pos(2)+60, pos(1)+240, pos(2)+80], 'Value', 'CONTROLLER_RUNTIME');
add_block('simulink/Signal Routing/Variant Source', [mdl '/PX4_Connector_Select'], ...
    'Position', [pos(1)+260, pos(2), pos(1)+320, pos(2)+150]);
set_param([mdl '/PX4_Connector_Select'], ...
    'VariantControlMode', 'expression', ...
    'VariantActivationTime', 'update diagram', ...
    'VariantControls', {'CONTROLLER_RUNTIME == 1', 'CONTROLLER_RUNTIME == 2'});

% Rewire: PX4 Interface -> Memory used to be direct; now goes through the selector.
delete_line(mdl, 'PX4 Interface/1', 'Memory/1');
add_line(mdl, 'PX4 Interface/1', 'PX4_Connector_Select/1', 'autorouting', 'on');
add_line(mdl, 'PX4 HITL Interface/1', 'PX4_Connector_Select/2', 'autorouting', 'on');
add_line(mdl, 'PX4_Connector_Select/1', 'Memory/1', 'autorouting', 'on');

% ---- Enable gate: HITL side runs only when CONTROLLER_RUNTIME==2 -----------------------------
add_block('simulink/Logic and Bit Operations/Relational Operator', [mdl '/HITL_Enable_cond'], ...
    'Position', [pos(1)-80, pos(2)+200, pos(1)-40, pos(2)+220], 'Operator', '==');
add_block('simulink/Sources/Constant', [mdl '/HITL_Enable_const2'], ...
    'Position', [pos(1)-140, pos(2)+220, pos(1)-100, pos(2)+240], 'Value', '2');
add_line(mdl, 'CONTROLLER_RUNTIME_sel/1', 'HITL_Enable_cond/1', 'autorouting', 'on');
add_line(mdl, 'HITL_Enable_const2/1', 'HITL_Enable_cond/2', 'autorouting', 'on');
add_line(mdl, 'HITL_Enable_cond/1', 'PX4 HITL Interface/Enable', 'autorouting', 'on');

% ---- Enable gate: SITL side runs only when CONTROLLER_RUNTIME==1 ------------------------------
% Requires the pixhawk_sil_connector.cpp fix (lazy connect in mdlOutputs, not mdlStart) to
% actually take effect -- an Enable port alone does nothing for a raw S-Function whose blocking
% call is in mdlStart. See the header note above.
add_block('simulink/Ports & Subsystems/Enable', [sitlBlk '/Enable']);
set_param([sitlBlk '/Enable'], 'PropagateVarSize', 'During execution');

add_block('simulink/Logic and Bit Operations/Relational Operator', [mdl '/SITL_Enable_cond'], ...
    'Position', [pos(1)-80, pos(2)+250, pos(1)-40, pos(2)+270], 'Operator', '==');
add_block('simulink/Sources/Constant', [mdl '/SITL_Enable_const1'], ...
    'Position', [pos(1)-140, pos(2)+270, pos(1)-100, pos(2)+290], 'Value', '1');
add_line(mdl, 'CONTROLLER_RUNTIME_sel/1', 'SITL_Enable_cond/1', 'autorouting', 'on');
add_line(mdl, 'SITL_Enable_const1/1', 'SITL_Enable_cond/2', 'autorouting', 'on');
add_line(mdl, 'SITL_Enable_cond/1', 'PX4 Interface/Enable', 'autorouting', 'on');

save_system(mdl);
fprintf('VehicleSilSimulation.slx updated: PX4 connector selects SITL/HITL via CONTROLLER_RUNTIME Variant Source.\n');
