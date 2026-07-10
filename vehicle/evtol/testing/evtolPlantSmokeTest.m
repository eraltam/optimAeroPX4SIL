function result = evtolPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "evtol", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
