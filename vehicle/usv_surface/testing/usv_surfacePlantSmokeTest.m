function result = usv_surfacePlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "usv_surface", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
