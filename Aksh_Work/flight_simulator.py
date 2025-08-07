import math
import csv
import json
import random
import argparse
import matplotlib.pyplot as plt

# Utility function to add Gaussian noise to a signal
def add_noise(value, noise_std):
    return round(random.gauss(value, noise_std), 3)

# Simulate altimeter (barometric altitude)
def simulate_altimeter(altitude, noise_std=0.5):
    return {"altitude": add_noise(altitude, noise_std)}

# Simulate IMU (accelerometer + gyroscope)
def simulate_imu(acceleration, velocity, noise_std=0.2):
    return {
        "accel_x": add_noise(acceleration, noise_std),
        "accel_y": add_noise(0.0, noise_std),
        "accel_z": add_noise(9.81, noise_std),  # Static gravity
        "gyro_x": add_noise(0.01 * velocity, noise_std),
        "gyro_y": add_noise(0.0, noise_std),
        "gyro_z": add_noise(0.02 * velocity, noise_std)
    }

# Simulate temperature and humidity sensor
def simulate_temp_humidity(noise_std=0.2):
    return {
        "temperature": add_noise(25.0, noise_std),
        "humidity": add_noise(55.0, noise_std)
    }

# Simulate GPS data with latitude, longitude, altitude, and timestamp
def simulate_gps(altitude, time_s, lat_base=37.7749, lon_base=-122.4194):
    # Simulate slight drift around a base coordinate (e.g., San Francisco)
    drift = 0.0001
    latitude = round(lat_base + random.uniform(-drift, drift), 6)
    longitude = round(lon_base + random.uniform(-drift, drift), 6)
    return {
        "latitude": latitude,
        "longitude": longitude,
        "altitude": round(altitude, 2),
        "timestamp": round(time_s, 2)
    }

# Main flight profile simulator: generates per-timestep sensor and telemetry data
def simulate_flight_profile(duration=30.0, time_step=0.1, burn_time=3.0, coast_time=2.0, noise_std=0.5):
    data = []
    velocity = 0
    altitude = 0
    steps = int(duration / time_step)

    for step in range(steps):
        t = round(step * time_step, 2)

        # Simple finite state flight logic
        if t < burn_time:
            acceleration = 30.0  # Rocket burn phase
        elif t < burn_time + coast_time:
            acceleration = -5.0  # Coasting phase with drag
        else:
            acceleration = -9.81  # Falling phase (gravity)

        # Integrate acceleration to get velocity and altitude
        velocity += acceleration * time_step
        altitude += velocity * time_step

        # Clamp altitude and velocity to zero if below ground level
        if altitude < 0:
            altitude = 0
            velocity = 0

        # Generate simulated sensor readings
        altimeter = simulate_altimeter(altitude, noise_std)
        imu = simulate_imu(acceleration, velocity, noise_std)
        env = simulate_temp_humidity(noise_std)
        gps = simulate_gps(altitude, t)

        # Bundle telemetry packet
        packet = {
            "time": t,
            "altimeter": altimeter,
            "imu": imu,
            "environment": env,
            "gps": gps,
            "raw": {
                "altitude": round(altitude, 2),
                "velocity": round(velocity, 2),
                "acceleration": round(acceleration, 2)
            }
        }

        data.append(packet)

    return data

# Save selected data fields to CSV for easy analysis
def save_to_csv(data, filename="simulated_flight.csv"):
    with open(filename, mode='w', newline='') as file:
        writer = csv.DictWriter(file, fieldnames=[
            "time", "altitude", "velocity", "acceleration",
            "temperature", "humidity", "accel_x", "gyro_z",
            "gps_latitude", "gps_longitude", "gps_altitude", "gps_timestamp"
        ])
        writer.writeheader()
        for row in data:
            writer.writerow({
                "time": row["time"],
                "altitude": row["altimeter"]["altitude"],
                "velocity": row["raw"]["velocity"],
                "acceleration": row["raw"]["acceleration"],
                "temperature": row["environment"]["temperature"],
                "humidity": row["environment"]["humidity"],
                "accel_x": row["imu"]["accel_x"],
                "gyro_z": row["imu"]["gyro_z"],
                "gps_latitude": row["gps"]["latitude"],
                "gps_longitude": row["gps"]["longitude"],
                "gps_altitude": row["gps"]["altitude"],
                "gps_timestamp": row["gps"]["timestamp"]
            })

# Save full telemetry packets to JSON for streaming or further processing
def save_to_json(data, filename="simulated_telemetry.json"):
    with open(filename, "w") as f:
        json.dump(data, f, indent=2)

# Plot graphs of altitude, velocity, and acceleration over time
def plot_flight(data):
    times = [row["time"] for row in data]
    altitudes = [row["altimeter"]["altitude"] for row in data]
    velocities = [row["raw"]["velocity"] for row in data]
    accelerations = [row["raw"]["acceleration"] for row in data]

    plt.figure(figsize=(12, 8))

    plt.subplot(3, 1, 1)
    plt.plot(times, altitudes)
    plt.title("Altitude vs Time")
    plt.ylabel("Altitude (m)")
    plt.grid(True)

    plt.subplot(3, 1, 2)
    plt.plot(times, velocities)
    plt.title("Velocity vs Time")
    plt.ylabel("Velocity (m/s)")
    plt.grid(True)

    plt.subplot(3, 1, 3)
    plt.plot(times, accelerations)
    plt.title("Acceleration vs Time")
    plt.xlabel("Time (s)")
    plt.ylabel("Acceleration (m/s²)")
    plt.grid(True)

    plt.tight_layout()
    plt.show()

# Command-line interface entry point
def main():
    parser = argparse.ArgumentParser(description="Simulate rocket flight sensor and telemetry data.")
    parser.add_argument("--duration", type=float, default=30.0, help="Total flight duration in seconds")
    parser.add_argument("--timestep", type=float, default=0.1, help="Time step in seconds")
    parser.add_argument("--burn", type=float, default=3.0, help="Burn time in seconds")
    parser.add_argument("--coast", type=float, default=2.0, help="Coast time after burn before fall")
    parser.add_argument("--noise", type=float, default=0.5, help="Sensor noise standard deviation")
    parser.add_argument("--no-plot", action="store_true", help="Disable plotting of data")
    args = parser.parse_args()

    print("[*] Simulating flight...")
    flight_data = simulate_flight_profile(
        duration=args.duration,
        time_step=args.timestep,
        burn_time=args.burn,
        coast_time=args.coast,
        noise_std=args.noise
    )

    save_to_csv(flight_data)
    save_to_json(flight_data)
    print("[+] Flight simulation complete. Data saved to CSV and JSON.")

    if not args.no_plot:
        plot_flight(flight_data)

if __name__ == "__main__":
    main()
