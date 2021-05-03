from __future__ import print_function
from __future__ import division
import matplotlib.pyplot as plt
import numpy as np

def blackmanFIR(s, fS, fL, N):
    # Compute sinc filter.
    h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))

    # Apply window.
    h *= np.blackman(N)

    # Normalize to get unity gain.
    h /= np.sum(h)

    return list(np.convolve(s, h, 'same'))

def kaiserFIR(s, fS, fL, N, beta):
    # Compute sinc filter.
    h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))

    # Apply window.
    h *= np.kaiser(N, beta)

    # Normalize to get unity gain.
    h /= np.sum(h)

    # Applying the filter to a signal s can be as simple as writing
    return list(np.convolve(s, h, 'same'))

def hammingFIR(s, fS, fL, N):
    # Compute sinc filter.
    h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))

    # Apply window.
    h *= np.hamming(N)

    # Normalize to get unity gain.
    h /= np.sum(h)

    # Applying the filter to a signal s can be as simple as writing
    return list(np.convolve(s, h, 'same'))

def rectangularFIR(s, fS, fL, N):
    # Compute sinc filter.
    h = np.sinc(2 * fL / fS * (np.arange(N) - (N - 1) / 2))

    # Normalize to get unity gain.
    h /= np.sum(h)

    # Applying the filter to a signal s can be as simple as writing
    return list(np.convolve(s, h, 'same'))

def transformTimeToFrequency(t, signal, sample_rate):
    n = len(signal) # length of the signal
    k = np.arange(n)
    T = n/sample_rate
    frq = k/T # two sides frequency range
    half = range(int(n/2))
    frq = frq[half]
    Y = np.fft.fft(signal)/n #fft computing and normalization
    Y = Y[half]

    return frq, Y

def movingAvgList(X, signal):
    if len(signal) == 0 or X == 0:
        return []
    if X == 1:
        return signal

    start = X - 1
    avgList = np.zeros(start)
    for i in range(start, len(signal)):
        avg = sum(signal[i-start:i]) / X
        avgList = np.append(avgList, avg)

    print('movingAvgList: ', avgList)
    return list(avgList)

def IIR(A, B, signal):
    new_average = signal.copy()
    for i in range(1, len(signal)):
        new_average[i] = new_average[i-1] * A + signal[i] * B

    return new_average

def plotMAFFFTs(t, signal, filteredSignal, frqo, Yo, frqf, Yf, filename, X):
    fig = plt.figure(filename.split('.')[0] + 'MAF X=' + str(X))
    ax1, ax2, ax3 = fig.subplots(3, 1)
    ax1.plot(t, signal, 'k')
    ax1.plot(t, filteredSignal, 'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.plot(t, filteredSignal, 'r')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Amplitude')
    ax3.loglog(frqo, abs(Yo), 'k')
    ax3.loglog(frqf, abs(Yf), 'r')
    ax3.set_xlabel('Freq (Hz)')
    ax3.set_ylabel('|Y(freq)|')
    plt.show()

def plotIIRFFTs(t, signal, filteredSignal, frqo, Yo, frqf, Yf, filename, A, B):
    fig = plt.figure(filename.split('.')[0] + 'IIR A=' + str(A) + ' B=' + str(B))
    ax1, ax2, ax3 = fig.subplots(3, 1)
    ax1.plot(t, signal, 'k')
    ax1.plot(t, filteredSignal, 'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.plot(t, filteredSignal, 'r')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Amplitude')
    ax3.loglog(frqo, abs(Yo), 'k')
    ax3.loglog(frqf, abs(Yf), 'r')
    ax3.set_xlabel('Freq (Hz)')
    ax3.set_ylabel('|Y(freq)|')
    plt.show()

def plotFIRFFTs(t, signal, filteredSignal, frqo, Yo, frqf, Yf, filename, details):
    fig = plt.figure(filename.split('.')[0] + 'FIR ' + details)
    ax1, ax2, ax3 = fig.subplots(3, 1)
    ax1.plot(t, signal, 'k')
    ax1.plot(t, filteredSignal, 'r')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.plot(t, filteredSignal, 'r')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Amplitude')
    ax3.loglog(frqo, abs(Yo), 'k')
    ax3.loglog(frqf, abs(Yf), 'r')
    ax3.set_xlabel('Freq (Hz)')
    ax3.set_ylabel('|Y(freq)|')
    plt.show()

def plotSingleFFT(t, signal, frq, Y, filename):
    fig = plt.figure(filename.split('.')[0])
    ax1, ax2 = fig.subplots(2, 1)
    ax1.plot(t, signal, 'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq, abs(Y), 'b')
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()

def exampleFFT():
    dt = 1.0/10000.0 # 10kHz
    t = np.arange(0.0, 1.0, dt) # 10s
    # a constant plus 100Hz and 1000Hz
    s = 4.0 * np.sin(2 * np.pi * 100 * t) + 0.25 * np.sin(2 * np.pi * 1000 * t) + 25

    Fs = 10000 # sample rate
    y = s # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))]

    _, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t,y,'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq,abs(Y),'b') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()
