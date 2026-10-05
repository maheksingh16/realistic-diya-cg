# Realistic Diya - Computer Graphics

A C++ Computer Graphics project that converts a traditional diya into a computer-generated scene using basic geometry, pixel manipulation, shading, lighting and rasterization.

## Project Overview

The project demonstrates how a real-world diya can be represented using fundamental Computer Graphics concepts.

### Components

- Clay diya body
- Upper rim
- Oil
- Wick
- Flame
- Flame-based glow
- Ground shadow

## Computer Graphics Concepts Used

- Geometric Modelling
- Ellipse-based Geometry
- Pixel Manipulation
- Colour Blending
- Gradient Shading
- Lighting Simulation
- Shadow
- Translation
- Scaling
- Rasterization
- BMP Image Generation

## Implementation

The project is implemented in standard C++ without external graphics libraries.

The diya is constructed using mathematical shapes and directly rendered into a pixel buffer. The final pixel data is written to a 24-bit BMP image.

## Output

The program generates:

`diya.bmp`

## How to Run

Compile:

```bash
g++ diya.cpp -o diya.exe
