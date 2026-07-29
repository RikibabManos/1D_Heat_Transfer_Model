# 1D Transient Heat Conduction Solver

## Overview
A high-performance C++ numerical solver for the 1D transient heat equation, implemented using the explicit Finite Difference Method (FDM). This project models heat dissipation through solid materials over time and serves as a foundational step toward more complex grid-based computational fluid dynamics (CFD) and magnetohydrodynamics (MHD) simulations.

The simulation computes the temperature distribution across a discretized 1D spatial grid, outputting the time-step data into a CSV format for post-processing and visualization in Python. 

The default simulation models a 0.5-meter aluminium rod with a thermal diffusivity ($\alpha$) of $4.82 \times 10^{-5} \text{ m}^2/\text{s}$

<p align = "center">
<img src="heat_flow_across_rod.gif" alt="Transient Heat Conduction" width="600" />
</p>

## Physics & Numerical Method
The core of the simulation solves the 1D parabolic partial differential equation for heat conduction:

$$ \frac{\partial T}{\partial t} = \alpha \frac{\partial^2 T}{\partial x^2} $$

Where:
*   $T$ is the temperature.
*   $t$ is time.
*   $x$ is the spatial coordinate.
*   $\alpha$ is the material's thermal diffusivity.

### Finite Difference Scheme
The equation is discretized using an explicit Forward-Time Central-Space (FTCS) scheme:
$T_{i}^{n+1} = T_{i}^{n} + \frac{\alpha \Delta t}{\Delta x^2} (T_{i+1}^{n} - 2T_{i}^{n} + T_{i-1}^{n})$

**Stability Condition:** 
To ensure mathematical stability (avoiding numerical divergence), the time step $\Delta t$ and spatial step $\Delta x$ strictly adhere to the Fourier number constraint:
$Fo = \frac{\alpha \Delta t}{\Delta x^2} \le 0.5$

## Architecture & Tech Stack
*   **Language:** C++11 (or higher)
*   **Memory Management:** `std::vector` for dynamic, safe spatial grid allocation.
*   **File I/O:** `<fstream>` for high-throughput CSV data logging.
*   **Post-Processing:** Python 3 & Matplotlib for reading the time-series CSV and generating thermal gradient visualizations.

## Project Structure
```text
├── src/
│   ├── main.cpp          # Core C++ application and simulation loop
│   └── Material.h        # Struct definitions for thermodynamic parameters
├── scripts/
│   └── visualize.py      # Python script to plot thermal gradients from CSV
├── data/
│   └── output.csv        # Generated simulation data (gitignored)
└── README.md
```

## Compilation and Execution

### 1. Build the C++ Solver
Use `g++` (or your preferred compiler) with optimization flags enabled for maximum performance:
```bash
g++ -O3 src/main.cpp -o heat_solver
```

### 2. Run the Simulation
Execute the compiled binary. The program will initialize the grid, apply the Dirichlet boundary conditions (e.g., constant heat source at $x=0$), step through time, and write the data to `data/output.csv`.
```bash
./heat_solver
```

### 3. Visualize the Results
Ensure you have `matplotlib` and `pandas` installed in your Python environment, then run the plotting script:
```bash
python scripts/visualize.py
```
This will generate a plot illustrating the temperature evolution across the rod's length at various time steps. (Example .gif above)

## Future Scope
This 1D thermal model lays the groundwork for multidimensional array processing and explicit numerical integration. The architecture is designed to be highly extensible, allowing for future upscaling into 2D/3D Navier-Stokes equations and more rigorous implicit solver methods (e.g., Crank-Nicolson).

## Author
**Babikir Osman**
* [GitHub](https://github.com/RikibabManos)
* [LinkedIn](https://www.linkedin.com/in/babikir-osman-8a4261282)
