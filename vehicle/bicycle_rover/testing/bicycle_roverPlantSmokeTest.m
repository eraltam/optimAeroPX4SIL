function result = bicycle_roverPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "bicycle_rover", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
