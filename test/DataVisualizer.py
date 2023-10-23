import matplotlib.pyplot as plt
import datetime
import matplotlib.dates as mdates

# Read the data from log.txt and remove duplicates
data_set = set()
duplicates_count = 0
unique_data = []

with open('LogFiles/10-23-23_Trip_#9.txt', 'r') as file:
    for line in file:
        cleaned_line = line.strip()
        if not cleaned_line:  # Skip empty lines
            continue
        line_list = cleaned_line.split(',')
        if len(line_list) != 9:  # Skip lines that don't have the expected number of columns
            continue
        line_tuple = tuple(map(int, line_list))
        if line_tuple in data_set:
            duplicates_count += 1
        else:
            data_set.add(line_tuple)
            unique_data.append(line_tuple)

print(f"Number of duplicate lines found and removed: {duplicates_count}")

# Extracting the columns
gnssFixType = [row[0] for row in unique_data]
iridiumMessageCount = [row[1] for row in unique_data]
epochTime = [datetime.datetime.utcfromtimestamp(row[2]) for row in unique_data]  # Keep as datetime objects
latitude = [row[3] * 1e-7 for row in unique_data]
longitude = [row[4] * 1e-7 for row in unique_data]
altitude = [row[5] * 0.00328084 for row in unique_data]  # converting mm to feet
speed = [row[6] * 0.00223694 for row in unique_data]     # converting mm/s to mph
heading = [row[7] * 1e-5 for row in unique_data]
stepCount = [row[8] for row in unique_data]

# Plotting
fig, ax = plt.subplots(3, 3, figsize=(15, 10))

# Define a formatter for the x-axis
date_format = mdates.DateFormatter('%Y-%m-%d %H:%M:%S')

# Plot and apply formatter for each subplot
for axes_row in ax:
    for a in axes_row:
        a.xaxis.set_major_formatter(date_format)
        plt.setp(a.xaxis.get_majorticklabels(), rotation=45)

ax[0, 0].plot(epochTime, gnssFixType)
ax[0, 0].set_title('GNSS Fix Type over Time')
ax[0, 0].set_ylabel('Number of satellites used in fix')

ax[0, 1].plot(epochTime, iridiumMessageCount)
ax[0, 1].set_title('Iridium Message Count over Time')
ax[0, 1].set_ylabel('Number of SOS messages sent')

ax[0, 2].plot(epochTime, latitude)
ax[0, 2].set_title('Latitude over Time')
ax[0, 2].set_ylabel('Latitude (°)')

ax[1, 0].plot(epochTime, longitude)
ax[1, 0].set_title('Longitude over Time')
ax[1, 0].set_ylabel('Longitude (°)')

ax[1, 1].plot(epochTime, altitude)
ax[1, 1].set_title('Altitude over Time')
ax[1, 1].set_ylabel('Altitude (feet)')

ax[1, 2].plot(epochTime, speed)
ax[1, 2].set_title('Speed over Time')
ax[1, 2].set_ylabel('Speed (mph)')

ax[2, 0].plot(epochTime, heading)
ax[2, 0].set_title('Heading over Time')
ax[2, 0].set_ylabel('Heading (°)')

ax[2, 1].plot(epochTime, stepCount)
ax[2, 1].set_title('Step Count over Time')
ax[2, 1].set_ylabel('Step Count')

fig.tight_layout()
plt.show()
