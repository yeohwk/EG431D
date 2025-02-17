import numpy as np
from scipy.signal import butter, freqz
import matplotlib.pyplot as plt

# High-pass filter parameters
fs = 1000  # Sampling frequency in Hz
cutoff = 30  # Cutoff frequency in Hz

# Design the high-pass filter using Butterworth
b, a = butter(4, cutoff / (fs / 2), btype='high')

# Compute the frequency response
w, h = freqz(b, a, worN=8000, fs=fs)

# Plot the frequency response
plt.figure(figsize=(8, 5))
plt.plot(w, 20 * np.log10(abs(h)), color='blue', label="Frequency Response")
plt.title("High-Pass Filter Frequency Response")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.axvline(cutoff, color='red', linestyle='--', label=f"Cutoff Frequency: {cutoff} Hz")
plt.grid(which='both', linestyle='--', linewidth=0.5)
plt.legend(loc="best")
plt.tight_layout()
plt.show()
