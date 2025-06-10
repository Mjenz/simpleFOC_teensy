import numpy as np
import matplotlib.pyplot as plt
import scipy as sp

# Load the data, skipping the first two header rows
# data = np.genfromtxt('/Users/michaeljenz/Documents/PlatformIO/Projects/ME_590/src/current sense debugging/data/raw data/test3/digital_pos_cntrl_interferencePWM.csv', delimiter=',', skip_header=1)
analog_data = np.genfromtxt('src/FOCcurrent_control/test5/trial5_analog.csv', delimiter=',', skip_header=1)



time_a = np.linspace(0,1,len(analog_data)) # normalized time
channel_1_a = analog_data[:, 1]    # Voltage from channel 1
channel_2_a = analog_data[:, 2]    # Voltage from channel 2# Plotting

# plt.plot(time_a, channel_1_a, label='Channel 1', color='magenta',linewidth=1)
# plt.plot(time_a, channel_2_a, label='Channel 2', color='purple',linewidth=1)
# plt.xlabel('Time (s)')
# plt.ylabel('Voltage (V)')
# plt.title('Oscilloscope Output')
# plt.grid(True)
# # plt.xlim([0,.1])
# plt.legend()
# plt.tight_layout()
# plt.show()

# Load data, skipping the header
data = np.genfromtxt('src/FOCcurrent_control/test9/trial.csv', delimiter=',', skip_header=1, dtype=float)

# Extract columns
# timestamp = data[:, 0]        # Timestamp
adc1 = data[:, 1]            # ADC1 voltage
adc2 = data[:, 2]            # ADC2 voltage
time_sec = np.linspace(0,1,len(data))

plt.figure(figsize=(10, 6))
plt.plot(time_sec, adc1, label='Q_current', color='red',linewidth=.5,marker='')
plt.plot(time_sec, adc2, label='D_current', color='blue',linewidth=.5,marker='')
plt.xlabel('Time (s)')
plt.ylabel('Current (A)')
plt.title('FOC Current Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.show()

# # Channel 1
# plt.figure(figsize=(10, 6))
# plt.plot(time_a, channel_1_a/50/0.01, label='Channel 1', color='red',linewidth='.5')
# plt.plot(time_sec, adc1, label='ADC1', color='blue',linewidth='.5')
# plt.xlabel('Time (normalized)')
# plt.ylabel('Current (A)')
# plt.title('ADC Readings Over Time')
# plt.grid(True)
# plt.legend()
# plt.tight_layout()
# plt.xlim([0,.1])
# plt.show()

# # Channel 2
# plt.figure(figsize=(10, 6))
# plt.plot(time_a, channel_2_a/50/0.01, label='Channel 2', color='red',linewidth='.5')
# plt.plot(time_sec, adc2, label='ADC2', color='blue',linestyle='solid',linewidth='.5')
# plt.xlabel('Time (normalized)')
# plt.ylabel('Current (A)')
# plt.title('ADC Readings Over Time')
# plt.grid(True)
# plt.legend()
# plt.tight_layout()
# plt.xlim([0,.1])
# plt.show()
