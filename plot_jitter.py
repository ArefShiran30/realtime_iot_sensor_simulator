import matplotlib
matplotlib.use("TkAgg")

import matplotlib.pyplot as plt
import re

sensor1 = []
sensor2 = []

with open("out.log", "r") as f:
    for line in f:
        # Match "Sensor 1", "Sensor +1", "Sensor +2", "Sensor 2", etc.
        m = re.search(r"Sensor\s*\+?(\d+).*Jitter:\s*([+-]?\d+\.\d+)", line)
        if m:
            sensor_id = int(m.group(1))
            jitter = float(m.group(2))

            if sensor_id == 1:
                sensor1.append(jitter)
            elif sensor_id == 2:
                sensor2.append(jitter)

# Remove first jitter spike (large negative)
if sensor1:
    sensor1 = sensor1[1:]
if sensor2:
    sensor2 = sensor2[1:]

# Plot
plt.figure(figsize=(12, 6))
plt.plot(sensor1, marker="o", label="Sensor 1 Jitter (ms)")
plt.plot(sensor2, marker="x", label="Sensor 2 Jitter (ms)")
plt.axhline(0, color="gray", linestyle="--", linewidth=1)

plt.title("Jitter Analysis of Real-Time IoT Sensor Simulator")
plt.xlabel("Sample Index")
plt.ylabel("Jitter (ms)")
plt.grid(True)
plt.legend()
plt.tight_layout()

# Save PNG
output_path = "jitter_plot.png"
plt.savefig(output_path, dpi=300)

print(f"Plot saved as: {output_path}")