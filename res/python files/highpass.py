import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

# Sampling parameters
fs = 1000  # Sampling frequency in Hz
t = np.linspace(0, 1, fs, endpoint=False)  # 1-second duration

# Signals
high_freq_signal = np.sin(2 * np.pi * 50 * t)  # 50 Hz high-frequency signal
low_freq_noise = 0.5 * np.sin(2 * np.pi * 5 * t)  # 5 Hz low-frequency noise
combined_signal = high_freq_signal + low_freq_noise

# High-pass filter design
cutoff = 30  # High-pass filter cutoff frequency in Hz
b, a = butter(4, cutoff / (fs / 2), btype='high')  # 4th-order Butterworth filter
filtered_signal = filtfilt(b, a, combined_signal)

# Plotting
plt.figure(figsize=(10, 8))

# High-frequency signal
plt.subplot(3, 1, 1)
plt.plot(t, high_freq_signal, label='High-Frequency Signal', color='b')
plt.title('High-Frequency Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid()

# Combined signal
plt.subplot(3, 1, 2)
plt.plot(t, combined_signal, label='High-Frequency Signal + Low-Frequency Noise', color='orange')
plt.title('High-Frequency Signal + Low-Frequency Noise')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid()

# High-pass filtered signal
plt.subplot(3, 1, 3)
plt.plot(t, filtered_signal, label='High-Pass Filtered Signal (Cutoff = 30 Hz)', color='green')
plt.title('High-Pass Filtered Signal (Cutoff = 30 Hz)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid()

plt.tight_layout()
plt.show()
