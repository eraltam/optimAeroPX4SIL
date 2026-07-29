"""Check whether logged body-frame magnetometer data stays fixed in NED."""

from __future__ import annotations

import argparse

import numpy as np
from pyulog import ULog
from scipy.spatial.transform import Rotation


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("ulog")
    args = parser.parse_args()

    ulog = ULog(args.ulog)
    mag = next(d.data for d in ulog.data_list if d.name == "sensor_mag" and d.multi_id == 0)
    attitude_topic = "vehicle_attitude"
    att = next(d.data for d in ulog.data_list if d.name == attitude_topic and d.multi_id == 0)

    flags = next(
        (d.data for d in ulog.data_list if d.name == "estimator_status_flags" and d.multi_id == 0),
        None,
    )
    if flags is not None and "cs_mag_fault" in flags:
        fault_indices = np.flatnonzero(flags["cs_mag_fault"])
        if fault_indices.size:
            fault_time = flags["timestamp"][fault_indices[0]]
            keep = mag["timestamp"] < fault_time
            mag = {name: values[keep] for name, values in mag.items()}
            print(f"limited analysis to pre-mag-fault data (< {fault_time} us)")

    tm = mag["timestamp"]
    ta = att["timestamp"]
    idx = np.searchsorted(ta, tm).clip(1, len(ta) - 1)
    choose_previous = (tm - ta[idx - 1]) < (ta[idx] - tm)
    idx[choose_previous] -= 1

    body = np.column_stack((mag["x"], mag["y"], mag["z"]))
    # PX4 quaternion is [w, x, y, z], body FRD to earth NED.
    q_xyzw = np.column_stack(
        (att["q[1]"][idx], att["q[2]"][idx], att["q[3]"][idx], att["q[0]"][idx])
    )
    earth = Rotation.from_quat(q_xyzw).apply(body)
    earth_heading = np.unwrap(np.arctan2(earth[:, 1], earth[:, 0]))

    for label, values in (("body", body), ("earth", earth)):
        magnitude = np.linalg.norm(values, axis=1)
        print(
            f"{label}: median={np.median(values, axis=0)}, "
            f"axis_std={np.std(values, axis=0)}, "
            f"|B| median/std={np.median(magnitude):.6f}/{np.std(magnitude):.6f} G"
        )
    print(
        "earth magnetic heading median/std: "
        f"{np.degrees(np.median(earth_heading)):.3f}/"
        f"{np.degrees(np.std(earth_heading)):.3f} deg"
    )
    print(f"attitude quaternion axis std: {np.std(q_xyzw, axis=0)}")
    print(f"attitude source: {attitude_topic}")


if __name__ == "__main__":
    main()
