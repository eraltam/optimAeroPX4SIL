clear all;
close all;

% -lws2_32 -lmswsock: required on Windows for Asio's socket calls (WSAStartup, closesocket,
% etc.) -- without these the compile succeeds but linking fails with "undefined reference to
% __imp_WSAStartup" and similar. Not needed on the original build environment this repo shipped
% from (evidently already had them somewhere on the link path), but required when rebuilding on
% this machine (2026-07-29).
mex -I./includes pixhawk_sil_connector.cpp -lws2_32 -lmswsock