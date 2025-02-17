import numpy as np
import matplotlib.pyplot as plt
from scipy.fftpack import fft, fftfreq

# Parameters
fs = 2000  # Sampling frequency in Hz
t = np.arange(0, 2, 1/fs)  # Time vector for 2 seconds

# Generate a synthetic machine vibration signal
freq1 = 50   # Frequency component 1 in Hz (e.g., motor imbalance)
freq2 = 150  # Frequency component 2 in Hz (e.g., loose parts)
freq3 = 400  # Frequency component 3 in Hz (e.g., bearing fault)
amp1 = 1.0   # Amplitude of component 1
amp2 = 0.5   # Amplitude of component 2
amp3 = 0.3   # Amplitude of component 3

signal = (
    amp1 * np.sin(2 * np.pi * freq1 * t) +
    amp2 * np.sin(2 * np.pi * freq2 * t) +
    amp3 * np.sin(2 * np.pi * freq3 * t)
)

# Add noise to simulate real-world conditions
noise = 0.2 * np.random.normal(size=len(t))
noisy_signal = signal + noise

# Perform FFT
n = len(t)  # Number of samples
frequencies = fftfreq(n, d=1/fs)  # Frequency vector
fft_result = fft(noisy_signal)  # FFT computation

# Only take the positive frequencies and scale the amplitude
positive_frequencies = frequencies[:n//2]
positive_fft_result = np.abs(fft_result[:n//2]) / n

# Plot the time-domain signal
plt.figure(figsize=(12, 6))
plt.subplot(2, 1, 1)
plt.plot(t, noisy_signal, label='Machine Vibration Signal', color='blue', linewidth=0.8)
plt.title('Time Domain: Machine Vibration Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.grid(True)
plt.legend()

# Plot the frequency-domain signal
plt.subplot(2, 1, 2)
plt.stem(positive_frequencies, positive_fft_result, basefmt=" ")
plt.title('Frequency Domain: Machine Vibration Signal')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.xlim(0, 500)  # Focus on relevant frequencies
plt.grid(True)

plt.tight_layout()
plt.show()
