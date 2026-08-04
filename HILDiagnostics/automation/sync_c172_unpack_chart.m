function sync_c172_unpack_chart()
%SYNC_C172_UNPACK_CHART Copy the reviewed source into every executable chart copy.
%
% The vehicle plant is copied into VehiclePlant and VehicleSilSimulation as a
% variant subsystem.  Updating only vehicle/c172pJSBSim/c172pJSBSim.slx leaves
% those embedded copies stale even though the standalone source model verifies
% successfully.  Keep all three copies byte-for-byte aligned before compiling.
% This updates existing Stateflow.EMChart scripts only; it does not add, delete,
% reconnect, or reposition Simulink blocks.

sync_jsbsim_unpack_chart("c172pJSBSim");
end
