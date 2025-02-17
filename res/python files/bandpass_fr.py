import numpy as np
from scipy.signal import butter, freqz
import matplotlib.pyplot as plt

# Bandpass filter parameters
fs = 1000  # Sampling frequency in Hz
lowcut = 8  # Lower cutoff frequency in Hz
highcut = 12  # Upper cutoff frequency in Hz

# Design a bandpass filter using a Butterworth filter
b, a = butter(4, [lowcut / (fs / 2), highcut / (fs / 2)], btype='band')

# Compute the frequency response
w, h = freqz(b, a, worN=8000, fs=fs)

# Plot the frequency response
plt.figure(figsize=(8, 5))
plt.plot(w, 20 * np.log10(abs(h)), color='blue', label="Frequency Response")
plt.title("Bandpass Filter Frequency Response (8-12 Hz)")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Amplitude (dB)")
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.axvline(lowcut, color='green', linestyle='--', label=f"Lower Cutoff: {lowcut} Hz")
plt.axvline(highcut, color='red', linestyle='--', label=f"Upper Cutoff: {highcut} Hz")
plt.legend(loc="best")
plt.tight_layout()
plt.show()
