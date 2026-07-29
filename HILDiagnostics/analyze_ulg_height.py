"""Summarize PX4 ULog height sources for SIL estimator debugging."""

from __future__ import annotations

import argparse

import numpy as np
from pyulog import ULog


TOPICS = (
    "sensor_baro",
    "sensor_gps",
    "vehicle_global_position",
    "vehicle_local_position",
    "estimator_status",
    "estimator_status_flags",
    "estimator_innovations",
    "estimator_sensor_bias",
    "sensor_mag",
    "vehicle_magnetometer",
)


def summarize(values: np.ndarray) -> str:
    finite = np.asarray(values)[np.isfinite(values)]
    if not finite.size:
        return "no finite samples"
    return (
        f"n={finite.size} first={finite[0]:.6g} last={finite[-1]:.6g} "
        f"min={finite.min():.6g} median={np.median(finite):.6g} max={finite.max():.6g}"
    )


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("ulog")
    args = parser.parse_args()

    ulog = ULog(args.ulog)
    for dataset in ulog.data_list:
        if dataset.name not in TOPICS:
            continue
        print(f"[{dataset.name}:{dataset.multi_id}]")
        for name, values in dataset.data.items():
            lowered = name.lower()
            is_mag_topic = dataset.name in ("sensor_mag", "vehicle_magnetometer")
            is_estimator_flag = dataset.name == "estimator_status_flags" and "mag" in lowered
            if is_mag_topic or is_estimator_flag or any(
                token in lowered
                for token in (
                    "alt",
                    "height",
                    "baro",
                    "press",
                    "gps",
                    "mag",
                    "heading",
                    "yaw",
                    "z_",
                    "dist_bottom",
                )
            ):
                print(f"  {name}: {summarize(values)}")


if __name__ == "__main__":
    main()
