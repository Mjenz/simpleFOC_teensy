import serial
import numpy as np

# Open serial port (make sure this matches your device)
s = serial.Serial('/dev/ttyACM0', baudrate=115200, timeout=1)

data = []

while True:
    # try:
        # Read a line from serial
        line = s.readline().decode('utf-8').strip()  # decode bytes to string and remove newline
        if line:
            split = line.split(',')
            if len(split) == 7:
                numbers = list(map(float, split))  # convert each item to float
                data.append(numbers)
                print(numbers)
            if len(data) > 5000:
                break
    # except ValueError:
    #     # Handles cases where conversion to float fails
    #     print(f"Skipping bad line: {line}")
    # except KeyboardInterrupt:
    #     # Allows you to stop the loop with Ctrl+C
    #     print("Interrupted by user.")
    #     break

# Convert to NumPy array
data = np.array(data)

# Optionally save to CSV
np.savetxt('src/FOCcurrent_control/test10_PROTO/trial.csv', data, delimiter=',')

print(data)
