function summary = verifyMissionRPY(sessionDir, opts)
%VERIFYMISSIONRPY Verify waypoint progression and roll/pitch/yaw behavior from a live SIL session.
%
% summary = verifyMissionRPY(sessionDir) reads <sessionDir>/px4.ulg (produced by
% saveLiveSILSession.m) and produces:
%   - a figure: ground track with waypoint-reach gates marked, plus roll/pitch/yaw vs time
%     with vertical markers at each waypoint-reach event
%   - a summary struct with per-waypoint reach time/position and pass/fail verdict
%
% Waypoint gate positions are derived directly from GPS position sampled at each
% mission_result.seq_reached transition -- no mission YAML parsing needed, so this works
% for any mission (zigzag, straight-line, racetrack, box, ...) without per-mission config.
%
% Example:
%   summary = verifyMissionRPY("HILDiagnostics/logs/session_c172js_20260729_box_ins2_pt_r1");

arguments
    sessionDir (1,1) string
    opts.savePng (1,1) logical = true
end

ulogPath = fullfile(sessionDir, "px4.ulg");
if ~isfile(ulogPath)
    error("verifyMissionRPY:NoUlog", "No px4.ulg found in %s", sessionDir);
end

ulog = ulogreader(ulogPath);

att = readTopicMsgs(ulog, "TopicNames", {'vehicle_attitude'}).TopicMessages{1};
gpos = readTopicMsgs(ulog, "TopicNames", {'vehicle_global_position'}).TopicMessages{1};
mres = readTopicMsgs(ulog, "TopicNames", {'mission_result'}).TopicMessages{1};
land = readTopicMsgs(ulog, "TopicNames", {'vehicle_land_detected'}).TopicMessages{1};
vstat = readTopicMsgs(ulog, "TopicNames", {'vehicle_status'}).TopicMessages{1};

%% Roll/pitch/yaw from quaternion (PX4 convention: q = [w x y z])
q = att.q;
[roll, pitch, yaw] = quat2rpy(q(:,1), q(:,2), q(:,3), q(:,4));
t_att_s = seconds(att.timestamp - ulog.StartTime);

%% Waypoint reach events: each time seq_reached increases, record the transition
seqReached = mres.seq_reached;
reachIdx = find(diff(seqReached) > 0) + 1;
reachSeq = seqReached(reachIdx);
reachTime = mres.timestamp(reachIdx);
[reachSeq, uidx] = unique(reachSeq, "stable"); % keep first crossing of each seq value
reachTime = reachTime(uidx);
reachTime_s = seconds(reachTime - ulog.StartTime);

nWp = numel(reachSeq);
wpLat = nan(nWp,1); wpLon = nan(nWp,1); wpAlt = nan(nWp,1);
for i = 1:nWp
    [~, gi] = min(abs(gpos.timestamp - reachTime(i)));
    wpLat(i) = gpos.lat(gi);
    wpLon(i) = gpos.lon(gi);
    wpAlt(i) = gpos.alt(gi);
end

%% Final vehicle state
finalLanded = land.landed(end);
finalArming = vstat.arming_state(end);
finalNavState = vstat.nav_state(end);
missionFinished = any(mres.finished == 1);
missionFailed = any(mres.failure == 1);

%% Figure
fig = figure("Position", [100 100 1200 800]);

subplot(2,2,1);
plot(gpos.lon, gpos.lat, "-", "Color", [0.3 0.5 0.9], "LineWidth", 1.2);
hold on;
for i = 1:nWp
    plot(wpLon(i), wpLat(i), "o", "MarkerSize", 12, "MarkerFaceColor", [0.2 0.75 0.3], ...
        "MarkerEdgeColor", "k", "LineWidth", 1.5);
    text(wpLon(i), wpLat(i), sprintf("  %s @ %.0fs", wpLabel(reachSeq(i)), reachTime_s(i)), ...
        "FontSize", 9, "VerticalAlignment", "bottom");
end
xlabel("Longitude"); ylabel("Latitude"); title("Ground track (green = waypoint reached)");
grid on; axis equal; hold off;

subplot(2,2,2);
plot(t_att_s, roll, "LineWidth", 1);
hold on;
markWaypoints(reachTime_s, reachSeq);
yline(45,"r--"); yline(-45,"r--");
xlabel("t (s)"); ylabel("roll (deg)"); title("Roll"); grid on; hold off;

subplot(2,2,3);
plot(t_att_s, pitch, "LineWidth", 1);
hold on;
markWaypoints(reachTime_s, reachSeq);
xlabel("t (s)"); ylabel("pitch (deg)"); title("Pitch"); grid on; hold off;

subplot(2,2,4);
plot(t_att_s, yaw, "LineWidth", 1);
hold on;
markWaypoints(reachTime_s, reachSeq);
xlabel("t (s)"); ylabel("yaw (deg)"); title("Yaw"); grid on; hold off;

[~, sessionName] = fileparts(sessionDir);
nRealWp = sum(reachSeq >= 1); % exclude seq=0 (takeoff item, not a navigation waypoint)
maxSeq = max(reachSeq, [], "omitnan");
sgtitle(sprintf("%s -- %d waypoint(s) reached (up to seq=%d), finished=%d, failure=%d, landed=%d", ...
    strrep(sessionName, "_", "\_"), nRealWp, maxSeq, ...
    missionFinished, missionFailed, finalLanded), "Interpreter", "tex");

if opts.savePng
    pngPath = fullfile(sessionDir, "rpy_verification.png");
    exportgraphics(fig, pngPath, "Resolution", 150);
    fprintf("Saved plot: %s\n", pngPath);
end

%% Summary struct + console report
summary = struct();
summary.sessionDir = sessionDir;
summary.waypointsReached = reachSeq;
summary.waypointReachTime_s = reachTime_s;
summary.waypointLat = wpLat;
summary.waypointLon = wpLon;
summary.waypointAlt_m = wpAlt;
summary.maxRoll_deg = max(abs(roll));
summary.maxPitch_deg = max(abs(pitch));
summary.missionFinished = missionFinished;
summary.missionFailed = missionFailed;
summary.finalLanded = finalLanded;
summary.finalArmingState = finalArming;
summary.finalNavState = finalNavState;
summary.durationLogged_s = seconds(ulog.EndTime - ulog.StartTime);

fprintf("\n=== %s ===\n", sessionName);
fprintf("Waypoints reached: %s\n", mat2str(reachSeq'));
for i = 1:nWp
    fprintf("  seq=%d reached at t=%.1fs  (lat=%.6f lon=%.6f alt=%.1fm)\n", ...
        reachSeq(i), reachTime_s(i), wpLat(i), wpLon(i), wpAlt(i));
end
fprintf("Max |roll|=%.1f deg, max |pitch|=%.1f deg\n", summary.maxRoll_deg, summary.maxPitch_deg);
fprintf("mission finished=%d failure=%d | final landed=%d arming_state=%d nav_state=%d\n", ...
    missionFinished, missionFailed, finalLanded, finalArming, finalNavState);
end

function [roll, pitch, yaw] = quat2rpy(w, x, y, z)
roll = atan2d(2*(w.*x + y.*z), 1 - 2*(x.^2 + y.^2));
sinp = max(-1, min(1, 2*(w.*y - z.*x)));
pitch = asind(sinp);
yaw = atan2d(2*(w.*z + x.*y), 1 - 2*(y.^2 + z.^2));
end

function markWaypoints(reachTime_s, reachSeq)
yl = ylim;
for i = 1:numel(reachTime_s)
    xline(reachTime_s(i), "Color", [0.2 0.75 0.3], "LineWidth", 1.2, "LineStyle", "--");
    text(reachTime_s(i), yl(2), wpLabel(reachSeq(i)), "FontSize", 8, ...
        "Color", [0.1 0.5 0.2], "VerticalAlignment", "top", "HorizontalAlignment", "left");
end
end

function lbl = wpLabel(seq)
% seq=0 is the takeoff item (mission_result convention), not a navigation waypoint.
if seq == 0
    lbl = "Takeoff";
else
    lbl = sprintf("WP%d", seq);
end
end
