import numpy as np
from scipy.signal import firwin, lfilter
import matplotlib.pyplot as plt

# Filter parameters
fs = 1000  # Sampling frequency (Hz)
cutoff = 100  # Cutoff frequency (Hz)
num_taps = 51  # Filter order + 1

# Generate filter coefficients
coefficients = firwin(num_taps, cutoff, fs=fs)

# Generate a noisy signal
t = np.linspace(0, 1, fs, endpoint=False)
signal = np.sin(2 * np.pi * 50 * t) + 0.5 * np.sin(2 * np.pi * 200 * t)  # 50 Hz + 200 Hz
noisy_signal = signal + 0.3 * np.random.normal(size=fs)

# Apply the filter
filtered_signal = lfilter(coefficients, 1.0, noisy_signal)

# Plot results
plt.figure(figsize=(10, 6))
plt.plot(t, noisy_signal, label="Noisy Signal", alpha=0.7)
plt.plot(t, filtered_signal, label="Filtered Signal", linewidth=2)
plt.title("Digital Low-Pass FIR Filter")
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.legend()
plt.grid()
plt.show()
