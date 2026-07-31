import sys
import math
from pyulog import ULog

path = sys.argv[1]
t_stop = float(sys.argv[2])
window = float(sys.argv[3]) if len(sys.argv) > 3 else 5.0

u = ULog(path)


def get(name, m=0):
    for d in u.data_list:
        if d.name == name and d.multi_id == m:
            return d
    return None


avel = get("vehicle_angular_velocity")
att = get("vehicle_attitude")
lpos = get("vehicle_local_position")


def quat_to_euler_deg(q0, q1, q2, q3):
    roll = math.degrees(math.atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2)))
    sinp = max(-1.0, min(1.0, 2 * (q0 * q2 - q3 * q1)))
    pitch = math.degrees(math.asin(sinp))
    return roll, pitch


print("t\tbodyRateNorm(rad/s)\troll_deg\tpitch_deg")
ta = avel.data["timestamp"]
q0, q1, q2, q3 = att.data["q[0]"], att.data["q[1]"], att.data["q[2]"], att.data["q[3]"]
tatt = att.data["timestamp"]
aidx = 0
maxnorm = 0
maxnorm_t = None
for i in range(len(ta)):
    t = ta[i] / 1e6
    if t < t_stop - window or t > t_stop + 1.0:
        continue
    wx, wy, wz = avel.data["xyz[0]"][i], avel.data["xyz[1]"][i], avel.data["xyz[2]"][i]
    norm = math.sqrt(wx * wx + wy * wy + wz * wz)
    if norm > maxnorm:
        maxnorm = norm
        maxnorm_t = t
    while aidx + 1 < len(tatt) and tatt[aidx + 1] <= ta[i]:
        aidx += 1
    roll, pitch = quat_to_euler_deg(q0[aidx], q1[aidx], q2[aidx], q3[aidx])
    print(f"{t:.3f}\t{norm:.3f}\t{roll:.1f}\t{pitch:.1f}")

print(f"\nmax bodyRateNorm in window: {maxnorm:.3f} rad/s at t={maxnorm_t}")

if lpos is not None:
    tl = lpos.data["timestamp"]
    print("\n=== local position z (NED down) near stop ===")
    for i in range(len(tl)):
        t = tl[i] / 1e6
        if t < t_stop - window or t > t_stop + 1.0:
            continue
        print(f"{t:.3f}\tz={lpos.data['z'][i]:.2f}")
