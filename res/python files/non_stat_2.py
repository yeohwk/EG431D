import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, fftfreq

# Sampling parameters
fs = 1000  # Sampling frequency in Hz
duration = 2  # Duration in seconds
t = np.linspace(0, duration, fs * duration, endpoint=False)  # Time vector

# Generate a non-stationary signal
signal = (
    np.sin(2 * np.pi * 50 * t) * (t < 0.5) +  # 50 Hz in first 0.5s
    np.sin(2 * np.pi * 120 * t) * ((t >= 0.5) & (t < 1.5)) +  # 120 Hz in next segment
    np.sin(2 * np.pi * 200 * t) * (t >= 1.5)  # 200 Hz in the last segment
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
plt.figure(figsize=(14, 8))
plt.subplot(2, 1, 1)
plt.plot(t, noisy_signal, label='Noisy Non-Stationary Signal', color='blue', linewidth=1.2)
plt.title('Time Domain Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid()
plt.legend()

# Plot the frequency domain signal
plt.subplot(2, 1, 2)
plt.stem(positive_frequencies, positive_fft_result, basefmt=" ", label='FFT Spectrum', linefmt='orange')
plt.title('Frequency Domain Signal')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
