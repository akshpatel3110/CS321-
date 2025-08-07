🚀 Rocket Flight Simulator & Telemetry Streamer

This project simulates the behavior of a rocket flight and provides real-time-like telemetry data that mimics actual sensor readings. It is designed to help flight segment students develop and test software using simulated data that is later compatible with real hardware. It also enables ground segment students to consume sample telemetry streams for processing and analysis.

🧠 Project Purpose

- For Flight Segment (CS 321 Students):
  - Build software logic using simulated data.
  - Transition seamlessly to real hardware later in the semester.

- For Ground Segment:
  - Consume and parse realistic sensor telemetry data.
  - Test ground systems under near-real conditions.

📂 Project Structure

.
├── flight_simulator.py        # Simulates full rocket flight profile and generates realistic sensor data
├── telemetry_streamer.py      # Streams telemetry data in real-time-like format from simulator or file
├── simulated_flight.csv       # Output file containing selected flight sensor readings
├── simulated_telemetry.json   # Output file containing full sensor telemetry packets
├── streamed_telemetry.jsonl   # Output file storing streamed telemetry packets
└── README.txt                 # You're reading it!

🛠 Features

- Flight Simulator (flight_simulator.py)
  - Simulates realistic flight phases: boost, coast, and descent.
  - Generates sensor data for:
    - Altimeter
    - IMU (accelerometer + gyroscope)
    - Temperature and humidity
  - Exports telemetry to:
    - .csv for analysis
    - .json for structured telemetry

- Telemetry Streamer (telemetry_streamer.py)
  - Reads JSON telemetry and emits packets at simulated real-time intervals.
  - Mocks GPS data with location drift.
  - Stores streamed data in .jsonl format (newline-delimited JSON).

▶️ Getting Started

Prerequisites:
- Python 3.8+
- matplotlib (for plotting)

Install dependencies:
pip install matplotlib

Run the Flight Simulator:
python flight_simulator.py

Optional CLI arguments:
--duration     # Total duration of flight (default: 30s)
--timestep     # Simulation timestep (default: 0.1s)
--burn         # Rocket motor burn time (default: 3s)
--coast        # Coasting time before falling (default: 2s)
--noise        # Noise standard deviation (default: 0.5)
--no-plot      # Disable graph plots

Example:
python flight_simulator.py --duration 45 --burn 5 --noise 0.3

Run the Telemetry Streamer:
python telemetry_streamer.py --input simulated_telemetry.json

Output:
- Real-time stream of telemetry packets with added GPS data
- Output saved to streamed_telemetry.jsonl

📊 Output Examples

CSV (simulated_flight.csv):
| time | altitude | velocity | acceleration | temperature | humidity | accel_x | gyro_z |
|------|----------|----------|--------------|-------------|----------|---------|--------|
| 0.0  | 0.5      | 0.3      | 30.0         | 25.1        | 54.9     | 30.3    | 0.3    |

JSON (simulated_telemetry.json):
{
  "time": 2.5,
  "altimeter": {"altitude": 12.45},
  "imu": {
    "accel_x": 1.3,
    "gyro_z": 0.15
  },
  "environment": {
    "temperature": 24.9,
    "humidity": 56.1
  },
  "raw": {
    "altitude": 12.5,
    "velocity": 15.2,
    "acceleration": -5.0
  }
}

Streamed JSONL (streamed_telemetry.jsonl):
{"time": 2.5, "altitude": 12.45, "gps": {"lat": 38.93, "lon": -77.04, "alt": 12.45}}
{"time": 2.6, "altitude": 13.20, "gps": {"lat": 38.9302, "lon": -77.0401, "alt": 13.20}}



