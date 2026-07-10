function result = uuv_subseaPlantSmokeTest()
initVehicleSIL("launchFullSIL", false, "vehicleType", "uuv_subsea", "launchPreflightVisualization", false);
testVehicleSIL();
result = true;
end
