import numpy as np
from scipy.signal import butter, freqz
import matplotlib.pyplot as plt

# Bandstop filter parameters
fs = 1000  # Sampling frequency in Hz
lowcut = 4  # Lower cutoff frequency in Hz
highcut = 6  # Upper cutoff frequency in Hz

# Design the bandstop filter using Butterworth
b, a = butter(4, [lowcut / (fs / 2), highcut / (fs / 2)], btype='bandstop')

# Compute the frequency response
w, h = freqz(b, a, worN=8000, fs=fs)

# Plot the frequency response
plt.figure(figsize=(8, 5))
plt.plot(w, 20 * np.log10(abs(h)), color='blue', label="Frequency Response")
plt.title("Bandstop Filter Frequency Response")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.axvline(lowcut, color='red', linestyle='--', label=f"Lower Cutoff: {lowcut} Hz")
plt.axvline(highcut, color='green', linestyle='--', label=f"Upper Cutoff: {highcut} Hz")
plt.grid(which='both', linestyle='--', linewidth=0.5)
plt.legend(loc="best")
plt.tight_layout()
plt.show()

