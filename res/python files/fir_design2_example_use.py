import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import firwin, lfilter

# Specifications for the FIR filter
cutoff = 0.2  # Normalized cutoff frequency (as a fraction of Nyquist rate)
fs = 1000  # Sampling rate in Hz
num_taps = 51  # Number of filter coefficients (filter order + 1)

# Design the low-pass FIR filter using a Hamming window
fir_coefficients = firwin(num_taps, cutoff, window="hamming", fs=fs)

# Generate a test signal: sum of a low-frequency signal (desired) and high-frequency noise
t = np.linspace(0, 1, fs, endpoint=False)  # 1 second of data
desired_signal = np.sin(2 * np.pi * 50 * t)  # Low-frequency component (50 Hz)
noise = 0.5 * np.sin(2 * np.pi * 300 * t)  # High-frequency noise (300 Hz)
noisy_signal = desired_signal + noise  # Combined signal

# Apply the FIR filter to the noisy signal
filtered_signal = lfilter(fir_coefficients, 1.0, noisy_signal)

# Plot the original, noisy, and filtered signals
plt.figure(figsize=(12, 8))

# Plot the desired signal
plt.subplot(3, 1, 1)
plt.plot(t, desired_signal, label="Desired Signal (50 Hz)", color="green")
plt.title("Desired Signal")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

# Plot the noisy signal
plt.subplot(3, 1, 2)
plt.plot(t, noisy_signal, label="Noisy Signal", color="red")
plt.title("Noisy Signal (50 Hz + 300 Hz)")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

# Plot the filtered signal
plt.subplot(3, 1, 3)
plt.plot(t, filtered_signal, label="Filtered Signal", color="blue")
plt.title("Filtered Signal (After Low-Pass FIR Filter)")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
