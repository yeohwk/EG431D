# Generating a low-pass filter example using the uploaded data structure
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Parameters
fs = 1000  # Sampling frequency (Hz)
t = np.linspace(0, 1, fs, endpoint=False)  # Time vector (1 second)
low_freq = 10  # Low-frequency signal (Hz)
high_freq = 50  # High-frequency noise (Hz)

# Generating signals
low_freq_signal = np.sin(2 * np.pi * low_freq * t)  # Low-frequency sine wave
high_freq_noise = 0.5 * np.sin(2 * np.pi * high_freq * t)  # High-frequency noise
combined_signal = low_freq_signal + high_freq_noise  # Combined signal

# Low-pass filter design (cutoff frequency = 20 Hz)
cutoff_freq = 20  # Hz
nyquist = 0.5 * fs  # Nyquist frequency
normal_cutoff = cutoff_freq / nyquist  # Normalized cutoff frequency
b, a = butter(4, normal_cutoff, btype='low', analog=False)  # Low-pass Butterworth filter
filtered_signal = filtfilt(b, a, combined_signal)  # Apply the filter

# Plotting the results
plt.figure(figsize=(10, 8))

plt.subplot(3, 1, 1)
plt.plot(t, low_freq_signal, label='Low-Frequency Signal')
plt.title('Low-Frequency Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.subplot(3, 1, 2)
plt.plot(t, combined_signal, label='Low-Frequency Signal + High-Frequency Noise', color='orange')
plt.title('Low-Frequency Signal + High-Frequency Noise (50 Hz)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.subplot(3, 1, 3)
plt.plot(t, filtered_signal, label='Filtered Signal', color='green')
plt.title('Low-Pass Filtered Signal (Cutoff = 20 Hz)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')

plt.tight_layout()
plt.show()
