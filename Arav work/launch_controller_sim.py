import tkinter as tk
from tkinter import messagebox
import time
import threading

class LaunchSimulator:
    def __init__(self, root):
        self.root = root
        self.root.title("Smart Rocket Launch Controller")
        self.ready = False

        # Readiness states
        self.sensor_status = tk.StringVar(value="Not Connected")
        self.telemetry_status = tk.StringVar(value="Link Down")
        self.fsm_state = tk.StringVar(value="Idle")
        self.countdown_text = tk.StringVar(value="")

        # GUI layout
        tk.Label(root, text="Sensor Status:").grid(row=0, column=0, sticky="e")
        tk.Label(root, textvariable=self.sensor_status).grid(row=0, column=1)

        tk.Label(root, text="Telemetry Link:").grid(row=1, column=0, sticky="e")
        tk.Label(root, textvariable=self.telemetry_status).grid(row=1, column=1)

        tk.Label(root, text="FSM State:").grid(row=2, column=0, sticky="e")
        tk.Label(root, textvariable=self.fsm_state).grid(row=2, column=1)

        self.status_light = tk.Label(root, text="●", font=("Arial", 30), fg="red")
        self.status_light.grid(row=0, column=2, rowspan=3, padx=20)

        self.check_btn = tk.Button(root, text="Run Readiness Check", command=self.run_checks)
        self.check_btn.grid(row=3, column=0, columnspan=2, pady=10)

        self.launch_btn = tk.Button(root, text="Launch", state="disabled", command=self.start_countdown)
        self.launch_btn.grid(row=4, column=0, columnspan=2, pady=10)

        self.countdown_label = tk.Label(root, textvariable=self.countdown_text, font=("Arial", 20))
        self.countdown_label.grid(row=5, column=0, columnspan=3, pady=5)

    def log_fsm_state_change(self, new_state):
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        log_entry = f"[{timestamp}] FSM State changed to: {new_state}\n"
        with open("fsm_log.txt", "a") as f:
            f.write(log_entry)

    def run_checks(self):
        # Simulated readiness checks
        self.sensor_status.set("Connected")
        self.telemetry_status.set("Link Established")
        self.fsm_state.set("Ready for Launch")
        self.log_fsm_state_change("Ready for Launch")

        self.ready = True
        self.status_light.config(fg="green")
        self.launch_btn.config(state="normal")

    def start_countdown(self):
        # Disable UI during countdown
        self.launch_btn.config(state="disabled")
        countdown_thread = threading.Thread(target=self.countdown)
        countdown_thread.start()

    def countdown(self):
        for i in range(5, 0, -1):
            self.countdown_text.set(f"T-{i} seconds")
            time.sleep(1)
        self.countdown_text.set("LAUNCH!")
        self.trigger_launch()

    def trigger_launch(self):
        self.fsm_state.set("Launched")
        self.log_fsm_state_change("Launched")
        self.status_light.config(fg="blue")
        messagebox.showinfo("Launch", "Launch Command Sent!")
        print("Launch signal sent at", time.strftime("%H:%M:%S"))

if __name__ == "__main__":
    root = tk.Tk()
    app = LaunchSimulator(root)
    root.mainloop()
