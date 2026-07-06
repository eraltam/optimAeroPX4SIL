"""End-to-end HIL health check: run preflight_check, then live_monitor +
loop_closure_test while VehicleSilSimulation is running in MATLAB, then print
one consolidated report. Saves a timestamped log under HILDiagnostics/logs/.

Right now (micro-USB only, section 1a) preflight_check must run before this
script's live-monitor/loop-closure steps can grab the port - the prompt below
gives you the chance to stop Simulink and hand the port back if needed.
"""
import sys
import time
import subprocess
from pathlib import Path

LOG_DIR = Path(__file__).parent / "logs"
SCRIPT_DIR = Path(__file__).parent


def run(cmd):
    print(f"\n$ {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=SCRIPT_DIR, capture_output=True, text=True)
    print(result.stdout)
    if result.returncode != 0:
        print(result.stderr, file=sys.stderr)
    return result.returncode, result.stdout


def main(device, baud, live_duration_s):
    LOG_DIR.mkdir(exist_ok=True)
    log_path = LOG_DIR / f"hil_report_{time.strftime('%Y%m%d_%H%M%S')}.log"

    results = {}
    with open(log_path, "w") as log:
        rc, out = run([sys.executable, str(SCRIPT_DIR / "preflight_check.py"), device, str(baud)])
        results["preflight_check"] = rc == 0
        log.write(out)

        print("\nStart the Simulink HIL run now (sim(\"VehicleSilSimulation\")), "
              "then press Enter to begin live monitoring.")
        input()

        rc, out = run([sys.executable, str(SCRIPT_DIR / "live_monitor.py"), device, str(baud), str(live_duration_s)])
        results["live_monitor"] = rc == 0
        log.write(out)

        rc, out = run([sys.executable, str(SCRIPT_DIR / "loop_closure_test.py"), device, str(baud)])
        results["loop_closure_test"] = rc == 0
        log.write(out)

    print("\n===== HIL Diagnostic Report =====")
    for name, ok in results.items():
        print(f"  {name:20s} {'PASS' if ok else 'FAIL'}")
    overall = all(results.values())
    print("OVERALL:", "PASS" if overall else "FAIL")
    print(f"Log saved to {log_path}")
    sys.exit(0 if overall else 1)


if __name__ == "__main__":
    device = sys.argv[1] if len(sys.argv) > 1 else "COM7"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 57600
    live_duration = float(sys.argv[3]) if len(sys.argv) > 3 else 30.0
    main(device, baud, live_duration)
