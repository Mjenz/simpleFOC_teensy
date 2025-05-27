import numpy as np
import matplotlib.pyplot as plt
import scipy as sp

# Load the data, skipping the first two header rows
data_scope = np.genfromtxt('src/FOCcurrent_control/test4/analog.csv', delimiter=',', skip_header=1)
data_adc = np.genfromtxt('src/FOCcurrent_control/test4/data_cropped.csv', delimiter=',', skip_header=1)

# Extract columns
time = data_scope[:, 0]         # Time in seconds
channel_1 = data_scope[:, 1]    # Voltage from channel 1
channel_2 = data_scope[:, 2]    # Voltage from channel 2
# trig = data_scope[:, 3]
time = time - time[0]

# Number of samplepoints
N1_scope = len(channel_1)
N2_scope = len(channel_2)
# sample spacing, 400 samples per second
T_scope = 1.0 / 30800.0
x1_scope = np.linspace(0.0, N1_scope*T_scope, N1_scope)
x2_scope = np.linspace(0.0, N2_scope*T_scope, N2_scope)
y1_scope = channel_1
y2_scope = channel_2
yf1_scope = sp.fftpack.fft(y1_scope)
yf2_scope = sp.fftpack.fft(y2_scope)
anglef1_scope = np.angle(yf1_scope)
anglef2_scope = np.angle(yf2_scope)
xf1_scope = np.linspace(0.0, 1.0/(2.0*T_scope), N1_scope//2)
xf2_scope = np.linspace(0.0, 1.0/(2.0*T_scope), N2_scope//2)

# fig, (ax1, ax2) = plt.subplots(1,2)
# ax1.plot(xf1_scope, 2.0/N1_scope * np.abs(yf1_scope[:N1_scope//2]))
# ax2.plot(xf2_scope, 2.0/N2_scope * np.abs(yf2_scope[:N2_scope//2]))
# plt.show()


# Extract columns
timestamp = data_adc[:, 0]        # Timestamp in milliseconds
adc1 = data_adc[:, 1]            # ADC1 voltage
adc2 = data_adc[:, 2]            # ADC2 voltage
adc3 = data_adc[:, 3]             # ADC3 voltage


# Number of samplepoints
N1_adc = len(adc1)
N2_adc = len(adc3)
# sample spacing, 400 samples per second
T_adc = 1.0 / 666.0
# x1_adc = np.linspace(0.0, N1_adc*T_scope, N1_adc)
# x2_adc = np.linspace(0.0, N2_adc*T_scope, N2_adc)



adc1i = np.arange(len(adc1))
adc2i = np.arange(len(adc2))
mask1 = np.isfinite(adc1)
mask2 = np.isfinite(adc2)
adc1filtered = np.interp(adc1i, adc1i[mask1], adc1[mask1])
adc2filtered = np.interp(adc2i, adc2i[mask2], adc2[mask2])
y1_adc = adc1filtered
y2_adc = adc2filtered
yf1_adc = sp.fftpack.fft(y1_adc)
yf2_adc = sp.fftpack.fft(y2_adc)
anglef1_adc = np.angle(yf1_adc)
anglef2_adc = np.angle(yf2_adc)
xf1_adc = np.linspace(0.0, 1.0/(2.0*T_adc), N1_adc//2)
xf2_adc = np.linspace(0.0, 1.0/(2.0*T_adc), N2_adc//2)

# fig, (ax1, ax2) = plt.subplots(1,2)
# ax1.plot(xf1_adc, 2.0/N1_adc * np.abs(yf1_adc[:N1_adc//2]))
# ax2.plot(xf2_adc, 2.0/N2_adc * np.abs(yf2_adc[:N2_adc//2]))
# plt.show()


fig, (ax1, ax2) = plt.subplots(1,2,figsize=(10, 6))
# Divide by N to normalize, multiply by 2 to preserve power, and display only positive values after calculating the magnitude of the FFT
ax1.plot(xf1_scope, 2.0/N1_scope * np.abs(yf1_scope[:N1_scope//2]))
ax2.plot(xf2_scope, 2.0/N2_scope * np.abs(yf2_scope[:N2_scope//2]))
ax1.plot(xf1_adc, 2.0/N1_adc * np.abs(yf1_adc[:N1_adc//2]))
ax2.plot(xf2_adc, 2.0/N2_adc * np.abs(yf2_adc[:N2_adc//2]))
ax1.legend(['scope1','adc1'])

ax1.set_title('Scope Signal 1 vs. ADC 1 FFT')
ax1.set_xlabel('freq [Hz]')
ax1.set_ylabel('mag [V]')
ax2.legend(['scope2','adc2'])
ax1.set_ylim(0,1)
ax1.set_xlim(0,50)
ax2.set_ylim(0,1)
ax2.set_xlim(0,50)
ax2.set_title('Scope Signal 2 vs. ADC 2 FFT')
ax2.set_xlabel('freq [Hz]')
ax2.set_ylabel('mag [V]')
plt.show()


fig2, (ax1, ax2) = plt.subplots(1,2,figsize=(10, 6))
# Divide by N to normalize, multiply by 2 to preserve power, and display only positive values after calculating the magnitude of the FFT
ax1.plot(xf1_scope, 2.0/N1_scope * (anglef1_scope[:N1_scope//2]))
ax2.plot(xf2_scope, 2.0/N2_scope * (anglef2_scope[:N2_scope//2]))
ax1.plot(xf1_adc, 2.0/N1_adc * (anglef1_adc[:N1_adc//2]))
ax2.plot(xf2_adc, 2.0/N2_adc * (anglef2_adc[:N2_adc//2]))
ax1.legend(['scope1','adc1'])
ax1.set_title('Scope Signal 1 vs. ADC 1 Phase of FFT')
ax1.set_xlabel('freq [Hz]')
ax1.set_ylabel('phase [rad]')
ax2.legend(['scope2','adc2'])
# ax1.set_ylim(0,1)
ax1.set_xlim(0,50)
# ax2.set_ylim(0,1)
ax2.set_xlim(0,50)
ax2.set_title('Scope Signal 2 vs. ADC 2 Phase of FFT')
ax2.set_xlabel('freq [Hz]')
ax2.set_ylabel('phase [rad]')
plt.show()

