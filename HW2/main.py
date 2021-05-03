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

        A = 0.9993
        B = 0.0007
        IIRFilteredSignal = python_fft.IIR(A, B, data)
        frqf, Yf = python_fft.transformTimeToFrequency(t, IIRFilteredSignal, sample_rate)
        python_fft.plotIIRFFTs(t, data, IIRFilteredSignal, frqo, Yo, frqf, Yf, filename, A, B)



if __name__ == "__main__":
    main()
