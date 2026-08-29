#  wireframe-c

> A zero-dependency 3D software rendering engine built from first principles in pure C11.

![C11](https://img.shields.io/badge/C-11-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-informational.svg)

`wireframe-c` is a CPU-based 3D graphics renderer designed to demonstrate the complete 3D graphics pipeline—from raw 3D mesh vertices to 2D screen pixels—without relying on external graphics libraries (like OpenGL, DirectX, or WebGL).

---

## 🚀 Key Features

* **Zero Hardware Abstraction:** Executes the entire rendering pipeline on the CPU using custom data structures.
* **First-Principles Math:** Custom implementation of 3D vector arithmetic, $4\times 4$ transformation matrices, matrix multiplication, and perspective projection.
* **Homogeneous Coordinate Space:** Implements 4D vectors $(x, y, z, w)$ to process 3D transformations and perspective division ($w$-divide).
* **Software Rasterization:** Employs Bresenham's line-drawing algorithm for efficient rendering onto a dynamic frame buffer.
* **PPM Image / Frame Output:** Outputs rendered 3D scenes directly into portable pixelmap formats via automated build rules.

---

## 🛠️ The Graphics Pipeline

```text
 ┌────────────────┐     ┌──────────────────────┐     ┌────────────────────────┐     ┌─────────────────┐
 │ 3D Mesh Data   │ ──> │ Model Transformation │ ──> │ Perspective Projection │ ──> │ Rasterization   │
 │ (Local Vertices│     │ (Scale, Rotate, Move)│     │ (Divide by W)          │     │ (Bresenham C)   │
 └────────────────┘     └──────────────────────┘     └────────────────────────┘     └─────────────────
```
┘
Bash
`make`:
Run the compiled executable to render the scene:

Bash
`./wireframe-c`:
To clean compiled object files and target output:

Bash
`make clean`
