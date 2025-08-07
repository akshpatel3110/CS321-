import csv
import time
import random

def generate_mock_telemetry():
    """Simulates incoming telemetry data."""
    return {
        "timestamp": time.strftime("%H:%M:%S"),
        "altitude": round(random.uniform(100, 500), 2),
        "acceleration": round(random.uniform(-2, 10), 2),
        "roll_rate": round(random.uniform(-180, 180), 2),
        "temperature": round(random.uniform(10, 35), 1),
        "humidity": round(random.uniform(20, 80), 1),
        "fsm_state": random.choice(["Idle", "Armed", "Ready", "Launch", "Coast", "Deploy", "Landed"])
    }

def main():
    filename = f"telemetry_log_{int(time.time())}.csv"
    fields = ["timestamp", "altitude", "acceleration", "roll_rate", "temperature", "humidity", "fsm_state"]

    print(f"[INFO] Logging telemetry data to: {filename}")

    with open(filename, mode="w", newline="") as file:
        writer = csv.DictWriter(file, fieldnames=fields)
        writer.writeheader()

        try:
            while True:
                data = generate_mock_telemetry()
                writer.writerow(data)
                print(data)
                time.sleep(0.01)  # 100 Hz

        except KeyboardInterrupt:
            print("\n[INFO] Logging stopped by user.")

if __name__ == "__main__":
    main()
