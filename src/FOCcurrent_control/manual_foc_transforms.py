import numpy as np
import matplotlib.pyplot as plt
import scipy as sp

# Load data, skipping the header
trial_num = '11.1'
test_num = '7'
data = np.genfromtxt('src/FOCcurrent_control/test' + test_num + '/trial' + trial_num + '.csv', delimiter=',', skip_header=1, dtype=float)

# Extract columns
# timestamp = data[:, 0]        # Timestamp
adc1 = data[:, 1]            # ADC1 current
adc2 = data[:, 2]            # ADC2 current
adc3 = data[:, 3]            # ADC3 current
electrical_angle = data[:, 4] # motor electrical angle
d_cur_data = data[:, 5]            # d current
q_cur_data = data[:, 6]            # q current
time_sec = np.linspace(0,1,len(data))

plt.figure(figsize=(10, 6))
plt.plot(time_sec, electrical_angle, label='electrical_angle', color='red',linewidth=.5,marker='')
plt.xlabel('Time (s)')
plt.ylabel('Anlge (rad)')
plt.title('Motor Electrical Angle Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Control_Test/test' + test_num + '/trial' + trial_num + '_electrical_angle.png')
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(time_sec, adc1, label='phase_a', color='red',linewidth=.5,marker='')
plt.plot(time_sec, adc2, label='phase_b', color='blue',linewidth=.5,marker='')
plt.plot(time_sec, adc3, label='phase_c', color='green',linewidth=.5,marker='')
plt.xlabel('Time (s)')
plt.ylabel('Current (A)')
plt.title('Phase Current Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Control_Test/test' + test_num + '/trial' + trial_num + '_phase_currents.png')
plt.show()

# clarke transform
a = -adc3 - adc2
i_alpha = a
i_beta = 0.57735026919 * a + 1.15470053838 * adc2

plt.figure(figsize=(10, 6))
plt.plot(time_sec, i_alpha, label='i_alpha', color='red',linewidth=.5,marker='')
plt.plot(time_sec, i_beta, label='i_beta', color='blue',linewidth=.5,marker='')
plt.xlabel('Time (s)')
plt.ylabel('Current (A)')
plt.title('Alpha and Beta Current Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Control_Test/test' + test_num + '/trial' + trial_num + '_ab_currents.png')
plt.show()

# park transform
st = np.sin(electrical_angle)
ct = np.cos(electrical_angle)
d_current = i_alpha * ct + i_beta * st
q_current = i_beta * ct - i_alpha * st

plt.figure(figsize=(10, 6))
plt.plot(time_sec, d_current, label='d_current', color='red',linewidth=.5,marker='')
plt.plot(time_sec, q_current, label='q_current', color='blue',linewidth=.5,marker='')
plt.xlabel('Time (s)')
plt.ylabel('Current (A)')
plt.title('FOC Current Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Control_Test/test' + test_num + '/trial' + trial_num + '_foc_currents.png')
plt.show()

# compare calculated dq currents with the simpleFOC ouptut

plt.figure(figsize=(10, 6))
plt.plot(time_sec, d_current, label='d_current_calculated', color='red',linewidth=.5,marker='')
plt.plot(time_sec, q_current, label='q_current_calculated', color='blue',linewidth=.5,marker='')
plt.plot(time_sec, d_cur_data, label='d_current', color='green',linewidth=.5,marker='')
plt.plot(time_sec, q_cur_data, label='q_current', color='magenta',linewidth=.5,marker='')
plt.xlabel('Time (s)')
plt.ylabel('Current (A)')
plt.title('FOC Current Over Time Comparison')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Control_Test/test' + test_num + '/trial' + trial_num + '_comparison_foc_currents.png')
plt.show()

