import numpy as np
import matplotlib.pyplot as plt
import scipy as sp

# Load data, skipping the header
trial_num = '19.1'
test_num = '8'
data = np.genfromtxt('src/FOC_position_control/test' + test_num + '/trial' + trial_num + '.csv', delimiter=',', skip_header=1, dtype=float)

# Extract columns
# timestamp = data[:, 0]        # Timestamp
time_sec = data[:,0]
sp_ang = data[:, 1]            # ADC1 current
ang = data[:, 2]            # ADC2 current
sp_vel = data[:, 3]            # ADC3 current
vel = data[:, 4] # motor electrical angle
sp_cur = data[:, 5]
d_cur_data = data[:, 6]            # d current
q_cur_data = data[:, 7]            # q current
# dc_cur_data = data[:, 8]            # dc current

plt.figure(figsize=(10, 6))
plt.plot(time_sec, sp_ang, label='set point angle', color='red',linewidth=.5,marker='')
plt.plot(time_sec, ang, label='angle', color='red',linewidth=.5,marker='')
plt.xlabel('Samples')
plt.ylabel('Angle (rad)')
plt.title('Angle Control Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Position_Test/test' + test_num + '/trial' + trial_num + '_angle.png')
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(time_sec, sp_vel, label='set point velocity', color='red',linewidth=.5,marker='')
plt.plot(time_sec, vel, label='velocity', color='blue',linewidth=.5,marker='')
plt.xlabel('Samples')
plt.ylabel('Veloicty (rad/s)')
plt.title('Velocity Control Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Position_Test/test' + test_num + '/trial' + trial_num + '_velocity.png')
plt.show()

plt.figure(figsize=(10, 6))
plt.plot(time_sec, q_cur_data, label='q current', color='blue',linewidth=.5,marker='')
plt.plot(time_sec, d_cur_data, label='d current', color='green',linewidth=.5,marker='')
plt.plot(time_sec, sp_cur, label='set point current', color='red',linewidth=.5,marker='')
plt.xlabel('Samples')
plt.ylabel('Current (A)')
plt.title('FOC Current Over Time')
plt.grid(True)
# plt.xlim([0,.1])
plt.legend()
plt.tight_layout()
plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Position_Test/test' + test_num + '/trial' + trial_num + '_FOCcurrents.png')
plt.show()

# plt.figure(figsize=(10, 6))
# plt.plot(time_sec, sp_cur, label='set point current', color='red',linewidth=.5,marker='')
# plt.plot(time_sec, dc_cur_data, label='dc current', color='blue',linewidth=.5,marker='')
# plt.xlabel('Time (s)')
# plt.ylabel('Current (A)')
# plt.title('DC Current Control Over Time')
# plt.grid(True)
# # plt.xlim([0,.1])
# plt.legend()
# plt.tight_layout()
# plt.savefig('/Users/michaeljenz/Documents/Research/MSR/Spring Quarter/FOC_Position_Test/test' + test_num + '/trial' + trial_num + '_DCcurrents.png')
# plt.show()
