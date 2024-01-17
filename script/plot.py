import matplotlib.pyplot as plt
import numpy as np

data = np.genfromtxt('data.txt', delimiter=' ->', dtype=float)

##########
TicksPerSecond = 20 # <--- ONLY MODIFY HERE
##########

time_interval = (1 / TicksPerSecond)
time = np.arange(0, len(data) * time_interval, time_interval)
speed = data[:, 1]

plt.plot(time, speed)
plt.xlabel('Time (s)')
plt.ylabel('Speed (rad/s)')
plt.title('Speed Response')
plt.grid(True)
plt.savefig('plot.png')
plt.show()
