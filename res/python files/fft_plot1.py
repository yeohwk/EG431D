import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, fftfreq

# Parameters for the signal
fs = 1000  # Sampling frequency in Hz
t = np.arange(0, 1, 1/fs)  # Time vector for 1 second

# Generate a complex periodic signal
freq1 = 50  # Frequency of the first component in Hz
freq2 = 120  # Frequency of the second component in Hz
freq3 = 200  # Frequency of the third component in Hz
amplitude1 = 1.0  # Amplitude of the first component
amplitude2 = 0.5  # Amplitude of the second component
amplitude3 = 0.2  # Amplitude of the third component

signal = (
    amplitude1 * np.sin(2 * np.pi * freq1 * t) +
    amplitude2 * np.sin(2 * np.pi * freq2 * t) +
    amplitude3 * np.sin(2 * np.pi * freq3 * t)
)

# Add noise to the signal
noise = 0.3 * np.random.normal(size=len(t))
noisy_signal = signal + noise

# Perform FFT
fft_result = fft(noisy_signal)
n = len(t)  # Number of samples
frequencies = fftfreq(n, d=1/fs)  # Frequency vector

# Only use the positive half of the frequencies and FFT results
positive_frequencies = frequencies[:n//2]
positive_fft_result = np.abs(fft_result[:n//2]) / n

# Plot the time domain signal
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(t, noisy_signal, label='x(t)')
plt.title('Time Domain Signal x(t)')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid()
plt.legend()

# Plot the frequency domain signal
plt.subplot(2, 1, 2)
plt.stem(positive_frequencies, positive_fft_result, basefmt=" ", label='FFT')
plt.title('Frequency Domain Signal X(f)')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
