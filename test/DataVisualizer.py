import matplotlib.pyplot as plt

# Read the data from log.txt
data = []
with open('LogFiles/testlog.txt', 'r') as file:
    for line in file:
        data.append(list(map(int, line.strip().split(','))))

# Extracting the columns
gnssFixType = [row[0] for row in data]
iridiumMessageCount = [row[1] for row in data]
epochTime = [row[2] for row in data]
latitude = [row[3] for row in data]
longitude = [row[4] for row in data]
altitude = [row[5] for row in data]
speed = [row[6] for row in data]
heading = [row[7] for row in data]
stepCount = [row[8] for row in data]

# Plotting
fig, ax = plt.subplots(3, 3, figsize=(15, 10))

ax[0, 0].plot(epochTime, gnssFixType)
ax[0, 0].set_title('gnssFixType over Time')
ax[0, 0].set_ylabel('gnssFixType')

ax[0, 1].plot(epochTime, iridiumMessageCount)
ax[0, 1].set_title('iridiumMessageCount over Time')
ax[0, 1].set_ylabel('iridiumMessageCount')

ax[0, 2].plot(epochTime, latitude)
ax[0, 2].set_title('Latitude over Time')
ax[0, 2].set_ylabel('Latitude')

ax[1, 0].plot(epochTime, longitude)
ax[1, 0].set_title('Longitude over Time')
ax[1, 0].set_ylabel('Longitude')

ax[1, 1].plot(epochTime, altitude)
ax[1, 1].set_title('Altitude over Time')
ax[1, 1].set_ylabel('Altitude')

ax[1, 2].plot(epochTime, speed)
ax[1, 2].set_title('Speed over Time')
ax[1, 2].set_ylabel('Speed')

ax[2, 0].plot(epochTime, heading)
ax[2, 0].set_title('Heading over Time')
ax[2, 0].set_ylabel('Heading')

ax[2, 1].plot(epochTime, stepCount)
ax[2, 1].set_title('Step Count over Time')
ax[2, 1].set_ylabel('Step Count')

fig.tight_layout()
plt.show()
