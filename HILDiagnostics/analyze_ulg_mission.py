import sys
from pyulog import ULog

path = sys.argv[1]
u = ULog(path)


def get(name, m=0):
    for d in u.data_list:
        if d.name == name and d.multi_id == m:
            return d
    return None


mr = get("mission_result")
print("=== mission_result ===")
for i in range(len(mr.data["timestamp"])):
    t = mr.data["timestamp"][i] / 1e6
    print(
        t,
        "seq_current=", mr.data["seq_current"][i],
        "seq_reached=", mr.data["seq_reached"][i],
        "finished=", mr.data["finished"][i],
        "failure=", mr.data["failure"][i],
    )

print()
print("=== vehicle_land_detected (last 10) ===")
ld = get("vehicle_land_detected")
n = len(ld.data["timestamp"])
for i in range(max(0, n - 10), n):
    print(ld.data["timestamp"][i] / 1e6, "landed=", ld.data["landed"][i])

print()
print("=== vehicle_status (last 10) ===")
vs = get("vehicle_status")
print(list(vs.data.keys()))
n = len(vs.data["timestamp"])
for i in range(max(0, n - 10), n):
    print(
        vs.data["timestamp"][i] / 1e6,
        "arming_state=", vs.data.get("arming_state", [None] * n)[i],
        "nav_state=", vs.data.get("nav_state", [None] * n)[i],
        "failsafe=", vs.data.get("failsafe", [None] * n)[i],
    )

print()
print("=== failsafe_flags (last 5) ===")
ff = get("failsafe_flags")
n = len(ff.data["timestamp"])
keys = [k for k in ff.data.keys() if k != "timestamp"]
for i in range(max(0, n - 5), n):
    active = [k for k in keys if ff.data[k][i]]
    print(ff.data["timestamp"][i] / 1e6, active)
