function renderMissionVideo(sessionDir, opts)
%RENDERMISSIONVIDEO Render an MP4 (3D + 2D) of a live SIL session's flight from its px4.ulg.
%
% renderMissionVideo(sessionDir) reads <sessionDir>/px4.ulg and writes
% <sessionDir>/mission_video.mp4 showing, side by side:
%   - a 3D view and a 2D top-down view, both showing the TRUE simulated flight path
%     (vehicle_*_groundtruth topics -- the actual physics-simulated aircraft state, smooth by
%     construction, exactly what a live "Matlab" visualization window shows) with a proper
%     oriented airplane-shaped glyph (rotated by roll/pitch/yaw in 3D, by yaw alone in 2D so
%     heading is visible top-down), plus waypoint gates (grey = pending, green = reached)
%   - PX4's own ESTIMATED trajectory overlaid as a thin dashed orange line, for comparing what
%     the aircraft actually did against what PX4 believed it was doing. EKF horizontal position
%     resets (vehicle_global_position.lat_lon_reset_counter / alt_reset_counter jumps) are
%     marked on the ESTIMATE line with a red X and reported to the console -- this is the
%     "strange jump" detector. The true trajectory is never discontinuous by construction; only
%     the estimate can show resets.
%
% Name-value options:
%   outFile           - output path (default: <sessionDir>/mission_video.mp4)
%   videoDuration_s    - target playback length in seconds (default 45)
%   frameRate          - video frame rate (default 20)
%
% Example:
%   renderMissionVideo("HILDiagnostics/logs/session_c172js_20260729_box_ins2_pt_r1")

arguments
    sessionDir (1,1) string
    opts.outFile (1,1) string = ""
    opts.videoDuration_s (1,1) double = 45
    opts.frameRate (1,1) double = 20
end

if opts.outFile == ""
    opts.outFile = fullfile(sessionDir, "mission_video.mp4");
end

ulogPath = fullfile(sessionDir, "px4.ulg");
if ~isfile(ulogPath)
    error("renderMissionVideo:NoUlog", "No px4.ulg found in %s", sessionDir);
end

ulog = ulogreader(ulogPath);
gtPos = readTopicMsgs(ulog, "TopicNames", {'vehicle_local_position_groundtruth'}).TopicMessages{1};
gtAtt = readTopicMsgs(ulog, "TopicNames", {'vehicle_attitude_groundtruth'}).TopicMessages{1};
estPos = readTopicMsgs(ulog, "TopicNames", {'vehicle_global_position'}).TopicMessages{1};
mres = readTopicMsgs(ulog, "TopicNames", {'mission_result'}).TopicMessages{1};

%% TRUE trajectory: local NED groundtruth -> ENU (East, North, Up) meters
t_gt_s = seconds(gtPos.timestamp - ulog.StartTime);
east_true = gtPos.y;
north_true = gtPos.x;
up_true = -gtPos.z;

t_gtatt_s = seconds(gtAtt.timestamp - ulog.StartTime);
[rollAll, pitchAll, yawAll] = quat2rpyLocal(gtAtt.q(:,1), gtAtt.q(:,2), gtAtt.q(:,3), gtAtt.q(:,4));
roll_i = interp1(t_gtatt_s, rollAll, t_gt_s, "linear", "extrap");
pitch_i = interp1(t_gtatt_s, pitchAll, t_gt_s, "linear", "extrap");
yaw_i = interp1(t_gtatt_s, yawAll, t_gt_s, "linear", "extrap");

%% ESTIMATED trajectory: global lat/lon -> the same local ENU frame (relative to gtPos ref)
lat0 = gtPos.ref_lat(1); lon0 = gtPos.ref_lon(1);
Rearth = 6378137;
t_est_s = seconds(estPos.timestamp - ulog.StartTime);
north_est = (estPos.lat - lat0) * (pi/180) * Rearth;
east_est = (estPos.lon - lon0) * (pi/180) * Rearth * cosd(lat0);

%% EKF reset detection (on the ESTIMATE only -- ground truth is never discontinuous)
resetMask = [false; (diff(estPos.lat_lon_reset_counter) ~= 0) | (diff(estPos.alt_reset_counter) ~= 0)];
resetIdx = find(resetMask);
fprintf("Detected %d EKF reset event(s) on the estimate", numel(resetIdx));
if ~isempty(resetIdx)
    fprintf(" at t = %s s", mat2str(round(t_est_s(resetIdx)', 1)));
end
fprintf("\n");

%% Waypoint reach events, position sampled from TRUE trajectory at the reach timestamp
seqReached = mres.seq_reached;
reachRel = find(diff(seqReached) > 0) + 1;
reachSeq = seqReached(reachRel);
reachTime = mres.timestamp(reachRel);
[reachSeq, uidx] = unique(reachSeq, "stable");
reachTime = reachTime(uidx);
reachTime_s = seconds(reachTime - ulog.StartTime);
nWp = numel(reachSeq);
wpEast = nan(nWp,1); wpNorth = nan(nWp,1); wpUp = nan(nWp,1);
for i = 1:nWp
    [~, gi] = min(abs(t_gt_s - reachTime_s(i)));
    wpEast(i) = east_true(gi); wpNorth(i) = north_true(gi); wpUp(i) = up_true(gi);
end

%% Downsample to target video frame count (drive frames off the TRUE trajectory timeline)
totalDuration_s = t_gt_s(end);
nFrames = max(2, round(opts.videoDuration_s * opts.frameRate));
frameTimes = linspace(0, totalDuration_s, nFrames);
frameIdx = round(interp1(t_gt_s, 1:numel(t_gt_s), frameTimes, "nearest", "extrap"));
playbackSpeedup = totalDuration_s / opts.videoDuration_s;

%% Airplane glyph: simple recognizable "paper airplane" silhouette, body axes (X-fwd,Y-right,Z-down)
wingSpan = max(15, 0.025 * max(range(east_true), range(north_true)));
nose        = [ 1.00,  0.00,  0.00];
wingtipR    = [-0.05,  1.00,  0.00];
wingtipL    = [-0.05, -1.00,  0.00];
tailCenter  = [-1.00,  0.00,  0.00];
tailWingR   = [-0.80,  0.30,  0.00];
tailWingL   = [-0.80, -0.30,  0.00];
finTop      = [-0.80,  0.00, -0.30];
glyphBody = wingSpan * [nose; wingtipR; wingtipL; tailCenter; tailWingR; tailWingL; finTop];
% faces reference rows of glyphBody: 1=nose 2=wingtipR 3=wingtipL 4=tailCenter 5=tailWingR 6=tailWingL 7=finTop
glyphFaces3D = [1 2 4; 1 3 4; 4 5 7; 4 6 7; 1 4 7];
glyphFaces2D = [1 2 4 3]; % flat planform outline (nose-wingtipR-tailCenter-wingtipL) for the top-down view

%% Figure / video setup
fig = figure("Position", [50 50 1500 750], "Color", "w");
vw = VideoWriter(opts.outFile, "MPEG-4");
vw.FrameRate = opts.frameRate;
vw.Quality = 90;
open(vw);

ax3 = subplot(1,2,1);
hold(ax3, "on"); grid(ax3, "on"); axis(ax3, "equal"); view(ax3, -35, 25);
xlabel(ax3, "East (m)"); ylabel(ax3, "North (m)"); zlabel(ax3, "Up (m)");
title(ax3, "3D trajectory (true flight path)");
plot3(ax3, east_true, north_true, up_true, "-", "Color", [0.75 0.85 0.75], "LineWidth", 1);
upAtEst = interp1(t_gt_s, up_true, min(t_est_s, t_gt_s(end)), "linear", "extrap");
plot3(ax3, east_est, north_est, upAtEst, "--", "Color", [0.95 0.55 0.15], "LineWidth", 0.75);
trail3 = plot3(ax3, nan, nan, nan, "-", "Color", [0.1 0.3 0.9], "LineWidth", 2);
ac3 = patch(ax3, "Faces", glyphFaces3D, "Vertices", nan(7,3), "FaceColor", [0.85 0.2 0.2], "EdgeColor", "k");
for i = 1:nWp
    plot3(ax3, wpEast(i), wpNorth(i), wpUp(i), "o", "MarkerSize", 10, ...
        "MarkerFaceColor", [0.6 0.6 0.6], "MarkerEdgeColor", "k");
end
if ~isempty(resetIdx)
    upAtReset = interp1(t_gt_s, up_true, t_est_s(resetIdx), "linear", "extrap");
    plot3(ax3, east_est(resetIdx), north_est(resetIdx), upAtReset, "x", "MarkerSize", 12, ...
        "Color", [0.9 0 0], "LineWidth", 2.5);
end
xlim(ax3, [min(east_true)-50, max(east_true)+50]);
ylim(ax3, [min(north_true)-50, max(north_true)+50]);
zlim(ax3, [min(up_true)-20, max(up_true)+50]);
legend(ax3, {"true path", "PX4 estimate", "", "", "", "reset"}, "Location", "best", "AutoUpdate", "off");

ax2 = subplot(1,2,2);
hold(ax2, "on"); grid(ax2, "on"); axis(ax2, "equal");
xlabel(ax2, "East (m)"); ylabel(ax2, "North (m)");
title(ax2, "2D ground track (top-down, true flight path)");
plot(ax2, east_true, north_true, "-", "Color", [0.75 0.85 0.75], "LineWidth", 1);
plot(ax2, east_est, north_est, "--", "Color", [0.95 0.55 0.15], "LineWidth", 0.75);
trail2 = plot(ax2, nan, nan, "-", "Color", [0.1 0.3 0.9], "LineWidth", 2);
ac2 = patch(ax2, "Faces", glyphFaces2D, "Vertices", nan(4,2), "FaceColor", [0.85 0.2 0.2], "EdgeColor", "k");
for i = 1:nWp
    plot(ax2, wpEast(i), wpNorth(i), "o", "MarkerSize", 10, "MarkerFaceColor", [0.6 0.6 0.6], "MarkerEdgeColor", "k");
    text(ax2, wpEast(i), wpNorth(i), sprintf(" %s", wpLabelLocal(reachSeq(i))), "FontSize", 8);
end
if ~isempty(resetIdx)
    plot(ax2, east_est(resetIdx), north_est(resetIdx), "x", "MarkerSize", 12, "Color", [0.9 0 0], "LineWidth", 2.5);
end
xlim(ax2, [min(east_true)-50, max(east_true)+50]);
ylim(ax2, [min(north_true)-50, max(north_true)+50]);

[~, sessionName] = fileparts(sessionDir);
txt = sgtitle("", "Interpreter", "none");

fprintf("Rendering %d frames (%.0fx real-time playback) to %s...\n", nFrames, playbackSpeedup, opts.outFile);
for f = 1:nFrames
    idx = frameIdx(f);
    pos = [east_true(idx), north_true(idx), up_true(idx)];

    Rnb = eulToDcmLocal(roll_i(idx), pitch_i(idx), yaw_i(idx));
    vertsNed = (Rnb * glyphBody')'; % [N E D] deltas in NED
    vertsEnu = [vertsNed(:,2), vertsNed(:,1), -vertsNed(:,3)] + pos;
    set(ac3, "Vertices", vertsEnu);
    set(trail3, "XData", east_true(1:idx), "YData", north_true(1:idx), "ZData", up_true(1:idx));

    % 2D glyph: rotate the flat planform by yaw only, project to East-North
    Rz2 = [cos(deg2rad(yaw_i(idx))), -sin(deg2rad(yaw_i(idx))); sin(deg2rad(yaw_i(idx))), cos(deg2rad(yaw_i(idx)))];
    bodyXY = glyphBody(:, [2 1]); % [East-ish, North-ish] = [y, x] in body axes
    vertsXY = (Rz2 * bodyXY')' + [pos(1), pos(2)];
    set(ac2, "Vertices", vertsXY);
    set(trail2, "XData", east_true(1:idx), "YData", north_true(1:idx));

    set(txt, "String", sprintf("%s  |  t=%.1fs  alt=%.1fm  roll=%.0f\\circ  pitch=%.0f\\circ  yaw=%.0f\\circ", ...
        strrep(sessionName, "_", "\_"), t_gt_s(idx), up_true(idx), roll_i(idx), pitch_i(idx), yaw_i(idx)));

    drawnow limitrate;
    writeVideo(vw, getframe(fig));
end

close(vw);
close(fig);
fprintf("Done: %s\n", opts.outFile);
end

function [roll, pitch, yaw] = quat2rpyLocal(w, x, y, z)
roll = atan2d(2*(w.*x + y.*z), 1 - 2*(x.^2 + y.^2));
sinp = max(-1, min(1, 2*(w.*y - z.*x)));
pitch = asind(sinp);
yaw = atan2d(2*(w.*z + x.*y), 1 - 2*(y.^2 + z.^2));
end

function R = eulToDcmLocal(rollDeg, pitchDeg, yawDeg)
r = deg2rad(rollDeg); p = deg2rad(pitchDeg); y = deg2rad(yawDeg);
Rx = [1 0 0; 0 cos(r) -sin(r); 0 sin(r) cos(r)];
Ry = [cos(p) 0 sin(p); 0 1 0; -sin(p) 0 cos(p)];
Rz = [cos(y) -sin(y) 0; sin(y) cos(y) 0; 0 0 1];
R = Rz*Ry*Rx; % body -> NED
end

function lbl = wpLabelLocal(seq)
if seq == 0
    lbl = "Takeoff";
else
    lbl = sprintf("WP%d", seq);
end
end
