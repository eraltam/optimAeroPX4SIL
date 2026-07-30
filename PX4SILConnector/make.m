clear all;
close all;

mingwLib = fullfile(getenv("ProgramData"), "MATLAB", "SupportPackages", ...
    "R2026a", "3P.instrset", "mingw_w64.instrset", ...
    "x86_64-w64-mingw32", "lib");
mex("-I.", "pixhawk_sil_connector.cpp", "-L" + mingwLib, "-lws2_32")
