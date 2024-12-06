import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import firwin, freqz

# Specifications for the FIR filter
cutoff = 60     # Cutoff frequency
fs = 1000       # Sampling rate in Hz
num_taps = 101  # Number of filter coefficients (filter order + 1)

# Design the low-pass FIR filter using a Hamming window
fir_coefficients = firwin(num_taps, cutoff, fs = fs, window = "hamming")

'''
Calculate the frequency response of the filter
worN = 8000: The number of frequency points at which the response is calculated.
A higher value provides a smoother plot of the frequency response.
fs = fs: The sampling rate in Hz. Specifying the sampling rate ensures that the 
frequency values returned are in Hz instead of normalized units (0 to π radians/sample).
'''
w, h = freqz(fir_coefficients, worN = 8000, fs = fs)

# Plot the impulse response of the filter
plt.figure(figsize = (12, 6))
plt.subplot(2, 1, 1)
#plt.stem(fir_coefficients, use_line_collection=True)
plt.stem(fir_coefficients)
plt.title("Impulse Response of FIR Low-Pass Filter")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude")
plt.grid()

# Plot the magnitude response of the filter
plt.subplot(2, 1, 2)
plt.plot(w, 20 * np.log10(np.abs(h)), 'b')
plt.title("Magnitude Response of FIR Low-Pass Filter")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")
plt.axvline(cutoff, color='red', linestyle='--', label=f'Cutoff Frequency = {cutoff} Hz')
plt.legend()
plt.grid()

plt.tight_layout()
plt.show()
