import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

# Given specifications
cutoff = 0.2  # Normalized cutoff frequency (Nyquist = 1)
fs = 1000  # Sampling rate in Hz
order = 50  # Filter order
num_taps = order + 1  # Number of filter coefficients

# Design the filter using firwin with a Hamming window
fir_coefficients = signal.firwin(num_taps, cutoff, window="hamming", pass_zero="lowpass", fs=fs)

# Frequency response of the filter
w, h = signal.freqz(fir_coefficients, worN=8000, fs=fs)

# Plot the filter response
plt.figure(figsize=(10, 6))
plt.title("Frequency Response of Low-Pass FIR Filter")
plt.plot(w, 20 * np.log10(np.abs(h)), label="Magnitude Response")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.grid()
plt.axvline(cutoff * fs / 2, color='red', linestyle='--', label="Cutoff Frequency")
plt.legend()
plt.show()

# Plot the filter coefficients
plt.figure(figsize=(10, 4))
plt.stem(fir_coefficients, use_line_collection=True)
plt.title("Filter Coefficients (Impulse Response)")
plt.xlabel("Coefficient Index")
plt.ylabel("Amplitude")
plt.grid()
plt.show()
