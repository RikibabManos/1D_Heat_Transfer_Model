import pandas as pd
import re
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

df = pd.read_csv(
    r"C:\Codes\cpp_starter_project\node_temp_variation.csv"
)

temp_table = df.to_numpy()
temperature_data = temp_table[:, 1:]
times = temp_table[:, 0] 
# search the column names (headers) for digits, as the digits represent positions of each node 
x_values = [
    float(match.group(1))
    for col in df.columns
    if (match := re.search(r"([\d.]+)", str(col))) is not None
]
node_num = len(x_values)

initial_temp_values = temperature_data[0, :] # initial temp values
vmin_val = np.min(temperature_data)          # used for overall bounds in heat map
vmax_val = np.max(temperature_data)

fig = plt.figure(figsize = (12, 6))
ax = fig.add_subplot()
plot_nodes = ax.scatter(
    x_values,
    np.zeros(node_num),
    c = initial_temp_values,
    marker = 'o',
    cmap = 'viridis',
    vmin = vmin_val,
    vmax = vmax_val,
    s = 100, 
)

# colourbar for temperature intensity reference
cbar = fig.colorbar(plot_nodes, ax=ax, orientation="horizontal", pad=0.25)
cbar.set_label("Temperature (Kelvin)")

ax.set_xlim(min(x_values) - 0.2, max(x_values) + 0.2)
ax.set_ylim(-.5, .5)
ax.set_xlabel("Spatial position x (m)")
ax.set_yticks([])  # remove unnecessary Y-axis ticks for 1D representation

def update(frame):

    current_temp_values = temperature_data[frame, :]                 # get new temperature values for current time
    plot_nodes.set_array(current_temp_values)                        # apply to scatter plot
    ax.set_title(                                                    # title to show current time
        f"1D Thermal Conduction Profile (Time: {times[frame]:.2f}s)"
    )
    return (plot_nodes,)
   

ani = animation.FuncAnimation(
    fig = fig,
    func = update,
    frames = len(times),
    interval = 50,
    blit = False,
    repeat = False
)

# Save the running animation directly as a GIF
ani.save(
    "heat_flow_across_rod.gif",
    writer = "pillow",
    fps  = 1000,
    dpi = 100
)

plt.show()