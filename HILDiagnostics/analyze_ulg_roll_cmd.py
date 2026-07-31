import sys
import math
from pyulog import ULog

path = sys.argv[1]
t_start = float(sys.argv[2])
t_end = float(sys.argv[3])
u = ULog(path)


def get(name, m=0):
    for d in u.data_list:
        if d.name == name and d.multi_id == m:
            return d
    return None


def quat_to_roll(q0, q1, q2, q3):
    return math.degrees(math.atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)))


att = get("vehicle_attitude")
sp = get("vehicle_attitude_setpoint")
pcs = get("position_controller_status")
npfg = get("npfg_status")

print("t\troll_actual\troll_setpoint(roll_body)")
ta = att.data["timestamp"]
tsp = sp.data["timestamp"]
q0, q1, q2, q3 = att.data["q[0]"], att.data["q[1]"], att.data["q[2]"], att.data["q[3]"]
spidx = 0
for i in range(len(ta)):
    t = ta[i] / 1e6
    if t < t_start or t > t_end:
        continue
    roll_act = quat_to_roll(q0[i], q1[i], q2[i], q3[i])
    while spidx + 1 < len(tsp) and tsp[spidx + 1] <= ta[i]:
        spidx += 1
    roll_sp = math.degrees(sp.data["roll_body"][spidx]) if "roll_body" in sp.data else float("nan")
    print(f"{t:.2f}\t{roll_act:.1f}\t{roll_sp:.1f}")

if npfg is not None:
    print("\n=== npfg_status fields ===")
    print(list(npfg.data.keys()))
    tn = npfg.data["timestamp"]
    for i in range(len(tn)):
        t = tn[i] / 1e6
        if t < t_start or t > t_end:
            continue
        lat_dist = npfg.data.get("lat_accel", [None])[i] if "lat_accel" in npfg.data else None
        xtrack = npfg.data.get("xtrack_error", [None])[i] if "xtrack_error" in npfg.data else None
        bearing = npfg.data.get("bearing", [None])[i] if "bearing" in npfg.data else None
        print(f"{t:.2f} lat_accel={lat_dist} xtrack_error={xtrack} bearing={bearing}")
