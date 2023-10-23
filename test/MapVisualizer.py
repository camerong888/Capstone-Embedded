import gmplot

# Define a function to convert the raw lat/long values to actual lat/long
def convert_to_coord(value):
    return value / 1e7

# Read data from the file
with open("LogFiles/10-23-23_Trip_#9.txt", "r") as f:
    lines = f.readlines()

# Parse latitudes, longitudes, and epoch times from the file
latitudes = []
longitudes = []
epoch_times = []
for line in lines:
    data = line.strip().split(',')
    epoch_times.append(int(data[2]))
    latitudes.append(convert_to_coord(int(data[3])))
    longitudes.append(convert_to_coord(int(data[4])))

# Plot the data using gmplot
gmap = gmplot.GoogleMapPlotter(latitudes[0], longitudes[0], 15)
gmap.plot(latitudes, longitudes, 'cornflowerblue', edge_width=2.5)

# Save the plot to an HTML file
gmap.draw("trip_path.html")

