function result = unicycle_roverPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "unicycle_rover", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
