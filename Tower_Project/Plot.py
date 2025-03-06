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

# Logarithmic normalization for color mapping
norm = mcolors.LogNorm(vmin=np.min(Z) + 1e-6, vmax=np.max(Z))  # Avoid log(0) issues

# Create figure
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# Plot surface with logarithmic color mapping
surf = ax.plot_surface(X1, X2, Z, cmap="viridis", norm=norm)

# Color bar with log scale
cbar = fig.colorbar(surf, shrink=0.5, aspect=5)
cbar.set_label("Objective Function Value (Log Scale)")

# Labels & View Angle
ax.set_xlabel("X1 (Radius 1)")
ax.set_ylabel("X2 (Radius 2)")
ax.set_zlabel("Objective Function")
ax.set_title("3D Objective Function Surface with Log Color Gradient")
ax.view_init(elev=30, azim=225)  # Adjust viewing angle

# Show the plot
plt.show()