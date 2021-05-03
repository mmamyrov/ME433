import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

def sigPlot(t, data, filename):
    plt.figure(filename.split('.')[0])
    plt.plot(t,data,'b-*')
    plt.xlabel('Time [s]')
    plt.ylabel('Signal')
    plt.title('Signal vs Time')
