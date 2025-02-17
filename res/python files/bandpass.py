import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Define parameters
fs = 100  # Sampling frequency in Hz
t = np.linspace(0, 1, fs, endpoint=False)  # 1-second time vector
lowcut = 4  # Lower cutoff frequency in Hz
highcut = 6  # Upper cutoff frequency in Hz

# Create a noisy signal: Combination of a 5 Hz signal (in-band) and 1 Hz (out-of-band) noise
in_band_signal = np.sin(2 * np.pi * 5 * t)  # Signal at 5 Hz
out_of_band_noise = np.sin(2 * np.pi * 1 * t)  # Noise at 1 Hz
noisy_signal = in_band_signal + out_of_band_noise + 0.5 * np.random.normal(size=len(t))

# Design a 4th-order Butterworth bandpass filter
b, a = butter(4, [lowcut / (fs / 2), highcut / (fs / 2)], btype='band')

# Apply the bandpass filter
bandpass_filtered_signal = filtfilt(b, a, noisy_signal)

# Plot the results
plt.figure(figsize=(10, 6))

# Plot the original noisy signal
plt.plot(t, noisy_signal, label="Original (Noisy)", color="blue", linewidth=0.8)

# Plot the bandpass filtered signal
plt.plot(t, bandpass_filtered_signal, label="Bandpass Filtered", color="green", linewidth=2)

# Adding titles, labels, and legend
plt.title("Original and Bandpass Filtered Signals")
plt.xlabel("Time in seconds")
plt.ylabel("Signal in volt")
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
