
# 📦 Smart Rocket System — Arav's Ground Tools

This folder contains two Python tools designed to help CS 321 students simulate and interact with the Smart Rocket System project **before actual hardware is available**.

---

## 🚀 Tool 1: Launch Controller Simulator (`launch_controller_sim.py`)

### 🔧 What It Does
Simulates the pre-launch process, including:
- Readiness checks (sensor status, telemetry link, FSM state)
- Countdown timer (T-5 to Launch)
- “Launch” button to trigger a simulated state transition
- FSM state visual feedback (colored indicator + GUI)
- **Logs all FSM transitions** to `fsm_log.txt` with timestamps

### ▶️ How to Run
```bash
python launch_controller_sim.py
```

### 🖥️ Requirements
- Python 3.6+
- No external packages (uses built-in `tkinter`)

### 📄 Outputs
- FSM transitions are logged in `fsm_log.txt`
- GUI shows current FSM state and system readiness

---

## 📡 Tool 2: Telemetry Logger (`telemetry_logger.py`)

### 🔧 What It Does
Simulates and logs incoming telemetry data at **~100 Hz**. Fields include:
- Altitude, acceleration, roll rate
- Temperature, humidity
- FSM state

### ▶️ How to Run
```bash
python telemetry_logger.py
```

### 🖥️ Requirements
- Python 3.6+
- No external packages

### 📄 Outputs
- A timestamped CSV file like `telemetry_log_1722268047.csv`
- Contains time-series sensor data and FSM state
- Press `Ctrl+C` to stop logging

---

## 🧪 How to Use These Together
You can run **both tools side-by-side** to simulate a full mission flow:
1. Start `telemetry_logger.py` to log data
2. Run `launch_controller_sim.py` to simulate readiness and launch
3. Review the outputs (`fsm_log.txt` and `.csv`) afterward

---

## 📚 Notes for Students
- These tools are **simulated** but designed to behave like real interfaces
- You’ll later replace the mock telemetry with live LoRa/ESP32 data
- FSM states and data formats may evolve — check team specs
