import time

def telemetry_generator_with_curves(file_path="data.txt", interval=0.1, loop=False):
    """
    Telemetry generator with velocity and acceleration curves.

    Args:
        file_path (str): Input file with telemetry data (10 columns).
        interval (float): Output rate (seconds). Default = 0.1s (10Hz).
        loop (bool): Replay indefinitely if True.
    """
    while True:
        with open(file_path, 'r') as f:
            lines = [line.strip() for line in f if line.strip()]

        prev_alt = None
        prev_vel = None
        elapsed_time = 0.0

        for line in lines:
            parts = line.split(',')
            if len(parts) < 10:
                continue  # skip malformed rows

            altitude = float(parts[9])

            # Compute velocity (Δh/Δt)
            if prev_alt is not None:
                velocity = (altitude - prev_alt) / interval
            else:
                velocity = 0.0

            # Compute acceleration (Δv/Δt)
            if prev_vel is not None:
                acceleration = (velocity - prev_vel) / interval
            else:
                acceleration = 0.0

            # Build extended telemetry line (with time, velocity, acceleration)
            extended_line = f"{elapsed_time:.2f}," + line + f",{velocity:.3f},{acceleration:.3f}"

            # Print as simulated telemetry
            print(extended_line)

            # Update for next iteration
            prev_alt = altitude
            prev_vel = velocity
            elapsed_time += interval

            time.sleep(interval)

        if not loop:
            break


if __name__ == "__main__":
    telemetry_generator_with_curves(
        file_path="data.txt",
        interval=0.1,  # 100 ms (10 Hz)
        loop=False
    )
