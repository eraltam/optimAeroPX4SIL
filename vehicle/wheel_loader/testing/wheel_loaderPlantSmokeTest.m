function result = wheel_loaderPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "wheel_loader", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
