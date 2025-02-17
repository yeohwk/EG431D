import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import lfilter

# Sampling parameters
fs = 1000  # Sampling frequency in Hz
t = np.linspace(0, 0.7, int(0.7 * fs), endpoint=False)  # Time vector for 0.7 seconds

# Original signal: low-frequency sine wave
original_signal = 10 * np.sin(2 * np.pi * 5 * t)  # 5 Hz sine wave with amplitude 10

# Noise: high-frequency random noise
noise = 30 * np.random.randn(len(t))  # Gaussian noise with amplitude 30

# Combined signal: original signal + noise
noisy_signal = original_signal + noise

# Adaptive filtering: low-pass filter (moving average filter as an example)
filter_order = 50
b = np.ones(filter_order) / filter_order  # Moving average filter coefficients
adaptive_filtered_signal = lfilter(b, 1, noisy_signal)

# Plotting the signals
plt.figure(figsize=(10, 6))

plt.plot(t, noisy_signal, label="Original (Noisy)", color="blue", linewidth=0.8)
plt.plot(t, adaptive_filtered_signal, label="Adaptive Filtered", color="magenta", linewidth=2)
plt.title("Original and Adaptive Filtered Signals")
plt.xlabel("Time in seconds")
plt.ylabel("Signal in volt")
plt.legend()
plt.grid()

plt.tight_layout()
plt.show()
