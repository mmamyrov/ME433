import matplotlib.pyplot as plt
import numpy as np

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


def plotFFTs(t, signal, filteredSignal, frqo, Yo, frqf, Yf, filename, X):
    fig = plt.figure(filename.split(',')[0] + ' X=' + str(X))
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
    fig = plt.figure(filename.split(',')[0])
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
