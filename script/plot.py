import matplotlib.pyplot as plt
import numpy as np

# Initialize lists to store time and RPM values
time_values = []
rpm_values = []

# Set the TicksPerSecond variable
TicksPerSecond = 20  # <--- ONLY MODIFY HERE

# Read data from file
with open('data.txt', 'r') as file:
    for line in file:

        # check if the line is blank or not
        if not line.strip():
            continue
        
        try:
            # Split the line using '|' as a delimiter
            data = line.strip().split('|')
            # Check if there are at least three '|' symbols
            if len(data) < 4:
                continue
            
            # Extract time and RPM values from the data
            time = float(data[1].strip())
            rpm = float(data[3].strip())
            
            # Append values to the lists
            time_values.append(time / TicksPerSecond)
            rpm_values.append(rpm)

        except ValueError as e:
            continue  # Skip lines with incorrect format

plt.plot(time_values, rpm_values)
plt.xlabel('Time (s)')
plt.ylabel('Speed (rad/s)')
plt.title('Speed Response')
plt.grid(True)
plt.savefig('plot.png')
plt.show()
