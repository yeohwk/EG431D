import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, fftfreq

# Sampling parameters
fs = 1000  # Sampling frequency in Hz
duration = 3  # Signal duration in seconds
t = np.linspace(0, duration, fs * duration, endpoint=False)  # Time vector

# Generate a complex non-periodic signal
np.random.seed(42)  # For reproducibility
signal = (
    0.8 * np.exp(-t) * np.sin(2 * np.pi * 10 * t) +   # Decaying sine wave
    0.5 * np.log(1 + t) * np.sin(2 * np.pi * 60 * t) +  # Logarithmic modulation
    0.3 * np.sin(2 * np.pi * t**2) +                 # Quadratic chirp
    0.4 * np.random.normal(size=len(t))             # Random noise
)

# Perform FFT
n = len(t)  # Number of samples
fft_result = fft(signal)
frequencies = fftfreq(n, d=1/fs)  # Frequency vector

# Only use the positive half of the spectrum
positive_frequencies = frequencies[:n//2]
positive_fft_result = np.abs(fft_result[:n//2]) / n

# Plot the time-domain signal
plt.figure(figsize=(14, 8))
plt.subplot(2, 1, 1)
plt.plot(t, signal, label='Complex Non-Periodic Signal', color='blue', linewidth=1.2)
plt.title('Time Domain Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid()
plt.legend()

# Plot the frequency-domain spectrum
plt.subplot(2, 1, 2)
plt.stem(positive_frequencies, positive_fft_result, basefmt=" ", linefmt='orange')
plt.title('Frequency Domain Spectrum')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.grid()

plt.tight_layout()
plt.show()
