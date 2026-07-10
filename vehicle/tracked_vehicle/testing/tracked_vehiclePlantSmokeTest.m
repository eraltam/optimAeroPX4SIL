function result = tracked_vehiclePlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "tracked_vehicle", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
