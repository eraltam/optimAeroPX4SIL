function result = octarotorPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "octarotor", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
