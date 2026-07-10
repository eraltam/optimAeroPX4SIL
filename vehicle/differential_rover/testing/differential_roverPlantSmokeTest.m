function result = differential_roverPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "differential_rover", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
