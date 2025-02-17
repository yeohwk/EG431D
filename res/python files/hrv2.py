import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import welch

# Simulate RR intervals in seconds
np.random.seed(42)  # Seed for reproducibility
duration = 300  # Duration of simulation in seconds (5 minutes)
fs = 4  # Sampling frequency (4 Hz is standard for HRV analysis)
t = np.arange(0, duration, 1/fs)  # Time vector

# Generate HRV components
baseline_rr = 0.8  # Baseline RR interval (seconds)
lf_power = 0.05 * np.sin(2 * np.pi * 0.1 * t)  # Low-frequency component (~0.1 Hz)
hf_power = 0.02 * np.sin(2 * np.pi * 0.25 * t)  # High-frequency component (~0.25 Hz)
random_noise = 0.01 * np.random.normal(size=len(t))  # Random noise for variability

# Combine to simulate RR intervals
rr_intervals = baseline_rr + lf_power + hf_power + random_noise

# Calculate instantaneous heart rate (HR = 60 / RR)
instantaneous_hr = 60 / rr_intervals  # Heart rate in beats per minute (BPM)

# Frequency-domain analysis using Welch's method
f, pxx = welch(rr_intervals, fs=fs, nperseg=1024)

# Plotting the HRV time-domain and frequency-domain signals
plt.figure(figsize=(14, 8))

# Time-domain plot (RR intervals)
plt.subplot(2, 1, 1)
plt.plot(t, rr_intervals, label="RR Intervals", color="blue", linewidth=1)
plt.title("Heart Rate Variability (Time Domain)")
plt.xlabel("Time (s)")
plt.ylabel("RR Interval (s)")
plt.grid()
plt.legend()

# Frequency-domain plot (Power Spectral Density)
plt.subplot(2, 1, 2)
plt.plot(f, pxx, label="Power Spectral Density", color="orange", linewidth=1.5)
plt.title("Heart Rate Variability (Frequency Domain)")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Power Spectral Density")
plt.axvspan(0.04, 0.15, color='blue', alpha=0.1, label="LF Band (0.04-0.15 Hz)")
plt.axvspan(0.15, 0.4, color='green', alpha=0.1, label="HF Band (0.15-0.4 Hz)")
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
