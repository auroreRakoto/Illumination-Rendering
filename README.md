# GPU Illumination

## Overview

This project explores advanced GPU-based rendering techniques to simulate realistic lighting and surface effects using OpenGL shaders. The focus is on the implementation of the Phong lighting model, texture blending, and geometric enhancements through shaders.

## Objectives

- Implement Phong illumination with ambient, diffuse, and specular components.
- Integrate multiple textures to simulate surface properties.
- Use ambient occlusion maps to simulate realistic shadowing in crevices.
- Apply height maps to dynamically modify vertex positions.
- Implement normal mapping in tangent space for detailed surface normals.
- Attempt parallax mapping for advanced texture displacement (experimental).
- Add a skybox using a cube map for immersive environments.
- Create an interactive button for basic GUI control.

## Features

- **Phong Lighting**: Classic lighting model for realistic highlights and shading.
- **Ambient Occlusion**: Darkens regions with limited exposure to ambient light.
- **Height Mapping**: Alters vertex geometry for displacement effects.
- **Normal Mapping**: Alters surface normals per-fragment for lighting variation.
- **Parallax Mapping**: Simulates deeper surface displacement based on view angle.
- **Cube Map (Skybox)**: Surrounds the scene with an environmental background.
- **Interactive UI Element**: Mouse-clickable button using screen-space coordinates.

## Folder Structure

GPU_illumination/
├── shaders/
│ ├── vertex_shader.glsl
│ ├── fragment_shader.glsl
│ └── ...
├── textures/
│ ├── color_map.png
│ ├── normal_map.png
│ ├── height_map.png
│ └── ...
├── src/
│ └── main.cpp
├── include/
├── README.md
└── CMakeLists.txt


## Dependencies

- OpenGL 3.3+
- GLFW
- GLAD or GLEW
- stb_image.h (for image loading)
- GLM (for math operations)

## Build Instructions

1. Clone the repository.
2. Run `cmake .` to configure the project.
3. Run `make` (or build from your IDE).
4. Execute the binary to launch the scene.

## Controls

- **Mouse Click**: Interact with the on-screen button.
- **ESC**: Close the application.

## Notes

- Parallax mapping is partially implemented and may produce visual artifacts.
- Normal maps must be precomputed and follow a tangent space format.
- Texture loading and binding should be verified to match shader uniform expectations.

## Author

**Aurore Rakotoarivony**  
5 ETI – CPE Lyon  
Major: Image, Modélisation et Informatique  
Date: December 2, 2023
