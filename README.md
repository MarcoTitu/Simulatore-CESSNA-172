# Cessna 172 Flight Simulator

This repository contains a **flight simulator of the Cessna 172 Skyhawk**, implemented in C as part of the *Flight Simulation* course at Politecnico di Torino.  
The project models the dynamics of the Cessna 172, a four-seat single-engine piston aircraft, widely used in general aviation.

📄 For a complete explanation of the simulator, its structure, and usage examples, please refer to:  
**[Tutorial_Simulatore.pdf](./Tutorial_Simulatore.pdf)**

---

## 📂 Project Structure

The repository is organized as follows:

- **Main/**  
  Contains the simulator itself, including:
  - `main.exe` → executable of the simulator  
  - `main.c` → main source code  
  - `Input/` → aerodynamic, engine, and propeller databases (`dba.txt`, `engine.txt`, `propeller.txt`)  
  - `Output/` → simulation results generated during execution  
  - `Librerie/` → library files implementing the simulator functions  
  - `postProc.m` → MATLAB script for post-processing and data visualization  

- **Validation/**  
  Several folders with validation executables used to test individual parts of the code (propeller, stability, interpolation).

---

## ⚙️ Simulation Workflow

The simulation is divided into **three main phases**:

1. **Pre-Processing**  
   - Import aerodynamic, engine, and propeller databases  
   - Input of flight and atmospheric conditions (altitude, velocity, flight path angle, etc.)

2. **Processing**  
   - Calculation of trim conditions  
   - Stability analysis (using Routh criterion)  
   - Integration of the equations of motion (6-DoF + mass variation)  
   - Execution of maneuvers defined by the user  

3. **Post-Processing**  
   - Output files are processed with MATLAB to visualize results (trajectories, forces, stability modes, etc.)

---

## 📥 Input Files

- `dba.txt` → aerodynamic database  
- `engine.txt` → engine parameters  
- `propeller.txt` → propeller characteristics  

Additional flight conditions (altitude, velocity, atmosphere) are provided by the user through the interface.

---

## 📤 Output Files

Simulation results are stored in text files:

- **Trim** results:  
  - `commandHistory0.txt` → command vector for trim  
  - `simOutput0.txt` → state variables for trim  

- **Maneuvers** results:  
  - `commandHistory[n].txt` → command vector for maneuver *n*  
  - `simOutput[n].txt` → state variables for maneuver *n*  

---

## 🛠️ Validation Tools

To simplify testing, several executables are available:  

- `mainValPropel.exe` → Propeller performance validation  
- `mainStability.exe` → Stability validation  
- `mainVerificaLett.exe` → Database reading and interpolation test  

---

## 📐 Mathematical Model

- **Equations of Motion**: 6-DoF aircraft dynamics + mass variation due to fuel consumption  
- **Reference Frames**: Body axes & NED frame  
- **Integration Method**: Explicit Euler with Δt = 0.01 s  

The state vector includes 13 variables:  
`[u, v, w, p, q, r, φ, θ, ψ, h, x, y, m]`

---

## 🎮 Running the Simulator

When launching the simulator, the interface will guide you through the following steps:

1. File loading confirmation  
2. Initialization of atmospheric and flight conditions  
3. Trim calculation and stability check  
4. Execution of:
   - **Trim hold simulation**  
   - **Maneuvers** (Step, Ramp, Impulse, Doublet) on elevator, aileron, rudder, or throttle  

---

## ✈️ Example Simulations

The tutorial includes several case studies:

- **Trim hold** → stability check at cruise conditions  
- **Impulse Rudder Command** → yaw response analysis  
- **Combined Maneuver** → coordinated turn with altitude hold  

Each example includes **post-processed plots** of velocities, Euler angles, trajectories, and forces.

---

## 📖 Documentation

For a detailed explanation of the simulator, code structure, input/output files, and example results, please read:  
👉 **[Tutorial_Simulatore.pdf](./Tutorial_Simulatore.pdf)**
