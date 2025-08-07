import json
import time
import argparse
import socket

def stream_telemetry(data, delay=0.1, output='console', host='localhost', port=9000):
    """
    Stream telemetry data packets in real-time.

    Args:
        data (list): List of telemetry packets (dicts).
        delay (float): Delay between packets in seconds.
        output (str): 'console' to print or 'socket' to send UDP.
        host (str): Host address for socket streaming.
        port (int): Port number for socket streaming.
    """
    if output == 'socket':
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        print(f"Streaming telemetry to UDP {host}:{port} ...")
    else:
        print("Streaming telemetry to console...")

    for packet in data:
        json_packet = json.dumps(packet)

        if output == 'console':
            print(json_packet)
        elif output == 'socket':
            sock.sendto(json_packet.encode('utf-8'), (host, port))

        time.sleep(delay)  # simulate real-time delay

    print(" Streaming completed.")

def main():
    parser = argparse.ArgumentParser(description="Stream simulated telemetry data in real-time.")
    parser.add_argument("--input", type=str, default="simulated_telemetry.json", help="Path to input JSON file")
    parser.add_argument("--delay", type=float, default=0.1, help="Delay between packets in seconds")
    parser.add_argument("--output", choices=["console", "socket"], default="console", help="Output mode")
    parser.add_argument("--host", type=str, default="localhost", help="Host for UDP output")
    parser.add_argument("--port", type=int, default=9000, help="Port for UDP output")
    args = parser.parse_args()

    # Load telemetry data from JSON file
    try:
        with open(args.input, "r") as f:
            telemetry_data = json.load(f)
    except FileNotFoundError:
        print(f"Error: File '{args.input}' not found.")
        return

    # Stream telemetry packets
    stream_telemetry(
        telemetry_data,
        delay=args.delay,
        output=args.output,
        host=args.host,
        port=args.port
    )

if __name__ == "__main__":
    main()
