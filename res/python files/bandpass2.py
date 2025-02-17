import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Parameters
fs = 1000  # Sampling frequency in Hz
t = np.linspace(0, 1, fs, endpoint=False)  # 1-second time vector
lowcut = 8  # Lower cutoff frequency in Hz
highcut = 12  # Upper cutoff frequency in Hz

# Generate the original signal (10 Hz sine wave)
original_signal = np.sin(2 * np.pi * 10 * t)

# Add noise to the signal: low-frequency noise (3 Hz) and high-frequency noise (50 Hz)
low_freq_noise = 0.5 * np.sin(2 * np.pi * 3 * t)
high_freq_noise = 0.5 * np.sin(2 * np.pi * 50 * t)
noisy_signal = original_signal + low_freq_noise + high_freq_noise + 0.2 * np.random.normal(size=len(t))

# Design a bandpass filter
b, a = butter(4, [lowcut / (fs / 2), highcut / (fs / 2)], btype='band')

# Apply the bandpass filter
bandpass_filtered_signal = filtfilt(b, a, noisy_signal)

# Plot the results
plt.figure(figsize=(10, 8))

# Original signal
plt.subplot(3, 1, 1)
plt.plot(t, original_signal, label="Original Signal (10 Hz)", color="blue")
plt.title("Original Signal")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

# Noisy signal
plt.subplot(3, 1, 2)
plt.plot(t, noisy_signal, label="Signal + Noise", color="orange")
plt.title("Signal Contaminated with Noise")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

# Bandpass filtered signal
plt.subplot(3, 1, 3)
plt.plot(t, bandpass_filtered_signal, label="Bandpass Filtered Signal", color="green")
plt.title("Bandpass Filtered Signal (8-12 Hz)")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
