import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import welch

# Generate simulated RR intervals
np.random.seed(42)  # For reproducibility
fs = 4  # Sampling frequency (in Hz, corresponds to HRV sampling rate)
duration = 300  # Duration of simulation in seconds (5 minutes)
t = np.linspace(0, duration, duration * fs, endpoint=False)

# Simulate HRV with low and high frequency components
low_freq = 0.1  # Low-frequency component (Hz)
high_freq = 0.25  # High-frequency component (Hz)
rr_intervals = (
    0.8 +  # Baseline RR interval (in seconds)
    0.05 * np.sin(2 * np.pi * low_freq * t) +  # Low-frequency component
    0.03 * np.sin(2 * np.pi * high_freq * t) +  # High-frequency component
    0.02 * np.random.normal(size=len(t))  # Random noise
)

# Convert RR intervals to instantaneous heart rate
heart_rate = 60 / rr_intervals  # HR in beats per minute (BPM)

# Welch's method for frequency-domain analysis of HRV
f, pxx = welch(rr_intervals, fs=fs, nperseg=256)

# Plot the time-domain HRV signal
plt.figure(figsize=(12, 8))

plt.subplot(2, 1, 1)
plt.plot(t, rr_intervals, label="RR Intervals", color="blue")
plt.title("Heart Rate Variability (Time Domain)")
plt.xlabel("Time (s)")
plt.ylabel("RR Interval (s)")
plt.grid()
plt.legend()

# Plot the frequency-domain HRV signal
plt.subplot(2, 1, 2)
plt.semilogy(f, pxx, label="Power Spectral Density", color="orange")
plt.title("Heart Rate Variability (Frequency Domain)")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Power Spectral Density")
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
