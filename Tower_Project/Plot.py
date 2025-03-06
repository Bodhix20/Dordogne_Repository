import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.colors as mcolors

# Load data
df = pd.read_csv("C:/Users/bodhi.fellerhoff/Desktop/Cours/3eme_Annee/Group_Project/Dordogne_Repository/Tower_Project/objective_function_data.csv")

# Extract columns
x1 = df["x1"].values
x2 = df["x2"].values
z = df["z"].values

# Reshape for 3D plotting
resolution = int(np.sqrt(len(x1)))  # Assumes a square grid
X1 = x1.reshape((resolution, resolution))
X2 = x2.reshape((resolution, resolution))
Z = z.reshape((resolution, resolution))

# Find the minimum value in Z
min_index = np.argmin(z)  # Index of min value
min_x1, min_x2, min_z = x1[min_index], x2[min_index], z[min_index]

# Logarithmic normalization for color mapping
norm = mcolors.LogNorm(vmin=np.min(Z) + 1e-6, vmax=np.max(Z))  # Avoid log(0) issues

# Create figure
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# Plot surface with logarithmic color mapping
surf = ax.plot_surface(X1, X2, Z, cmap="viridis", norm=norm)

# Add a marker at the minimum point
ax.scatter(min_x1, min_x2, min_z, color='red',edgecolors='black', s=100, label=f"Min: ({min_x1:.2f}, {min_x2:.2f}, {min_z:.2f})")
ax.legend()

# Color bar with log scale
cbar = fig.colorbar(surf, shrink=0.5, aspect=5)
cbar.set_label("Objective Function Value (Log Scale)")

# Labels & View Angle
ax.set_xlabel("X1 (Height 1)")
ax.set_ylabel("X2 (Height 2)")
ax.set_zlabel("Objective Function")
ax.set_title("3D Objective Function Surface with Log Color Gradient")
ax.view_init(elev=30, azim=225)  # Adjust viewing angle

# Show the plot
plt.show()