import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Parameters
fs = 1000  # Sampling frequency in Hz
t = np.linspace(0, 1, fs, endpoint=False)  # Time vector for 1 second

# Generate original high-frequency signal (30 Hz)
original_signal = np.sin(2 * np.pi * 30 * t)

# Add low-frequency noise (5 Hz) to the signal
low_frequency_noise = 0.5 * np.sin(2 * np.pi * 5 * t)
noisy_signal = original_signal + low_frequency_noise

# Design a bandstop filter to remove the 5 Hz noise
lowcut = 4  # Lower cutoff frequency in Hz
highcut = 6  # Upper cutoff frequency in Hz
b, a = butter(4, [lowcut / (fs / 2), highcut / (fs / 2)], btype='bandstop')

# Apply the bandstop filter
bandstop_filtered_signal = filtfilt(b, a, noisy_signal)

# Plotting the signals
plt.figure(figsize=(10, 8))

# Plot the original signal
plt.subplot(3, 1, 1)
plt.plot(t, original_signal, label="Original Signal (30 Hz)", color="blue")
plt.title("Original High-Frequency Signal")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

# Plot the noisy signal
plt.subplot(3, 1, 2)
plt.plot(t, noisy_signal, label="Signal + Low-Frequency Noise (5 Hz)", color="orange")
plt.title("High-Frequency Signal with Low-Frequency Noise")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

# Plot the bandstop filtered signal
plt.subplot(3, 1, 3)
plt.plot(t, bandstop_filtered_signal, label="Bandstop Filtered Signal", color="green")
plt.title("Bandstop Filtered Signal (Removed 5 Hz Noise)")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
