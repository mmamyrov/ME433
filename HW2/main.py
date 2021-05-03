import matplotlib.pyplot as plt
import python_plot
import python_csv
import python_fft

def main():
    filenames = ['sigA.csv', 'sigB.csv', 'sigC.csv', 'sigD.csv']

    for _, filename in enumerate(filenames):
        t, data = python_csv.getSigData(filename)
        # python_plot.sigPlot(t, data, filename)

        sample_rate = len(data) / (t[-1] - t[0])
        print(filename + " sample rate:" + str(sample_rate))

        frqo, Yo = python_fft.transformTimeToFrequency(t, data, sample_rate)
        # python_fft.plotSingleFFT(t, data, frqo, Yo, filename)
        # X = 220
        # MAFFilteredSignal = python_fft.movingAvgList(X, data)
        # frqf, Yf = python_fft.transformTimeToFrequency(t, MAFFilteredSignal, sample_rate)
        # python_fft.plotMAFFFTs(t, data, MAFFilteredSignal, frqo, Yo, frqf, Yf, filename, X)

        # A = 0.9993
        # B = 0.0007
        # IIRFilteredSignal = python_fft.IIR(A, B, data)
        # frqf, Yf = python_fft.transformTimeToFrequency(t, IIRFilteredSignal, sample_rate)
        # python_fft.plotIIRFFTs(t, data, IIRFilteredSignal, frqo, Yo, frqf, Yf, filename, A, B)

        if filename == 'sigC.csv':
            fL = 25
            bN = 115
            FIRBlackmanSignal = python_fft.blackmanFIR(data, sample_rate, fL, bN)
            frqf, Yf = python_fft.transformTimeToFrequency(t, FIRBlackmanSignal, sample_rate)
            blackmanDetails = f'fL={fL} N={bN} window=blackman'
            python_fft.plotFIRFFTs(t, data, FIRBlackmanSignal, frqo, Yo, frqf, Yf, filename, blackmanDetails)

            kN = 93
            beta = 2.655
            FIRKaiserSignal = python_fft.kaiserFIR(data, sample_rate, fL, kN, beta)
            frqf, Yf = python_fft.transformTimeToFrequency(t, FIRKaiserSignal, sample_rate)
            kaiserDetails = f'fL={fL} N={kN} beta={beta} window=kaiser'
            python_fft.plotFIRFFTs(t, data, FIRKaiserSignal, frqo, Yo, frqf, Yf, filename, kaiserDetails)

            hN = 79
            FIRHammingSignal = python_fft.hammingFIR(data, sample_rate, fL, hN)
            frqf, Yf = python_fft.transformTimeToFrequency(t, FIRHammingSignal, sample_rate)
            hammingDetails = f'fL={fL} N={hN} window=hamming'
            python_fft.plotFIRFFTs(t, data, FIRHammingSignal, frqo, Yo, frqf, Yf, filename, hammingDetails)

            rN = 91
            FIRRectSignal = python_fft.rectangularFIR(data, sample_rate, fL, rN)
            frqf, Yf = python_fft.transformTimeToFrequency(t, FIRRectSignal, sample_rate)
            hammingDetails = f'fL={fL} N={hN} window=hamming'
            rectDetails = f'fL={fL} n={rN} window=rect'
            python_fft.plotFIRFFTs(t, data, FIRRectSignal, frqo, Yo, frqf, Yf, filename, rectDetails)


if __name__ == "__main__":
    main()
