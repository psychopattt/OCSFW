# OCSFW
OCSFW (OpenGL Compute Shader Framework) is an open source, multi-platform library for OpenGL compute shader development. It already includes GLFW, GLAD and Dear ImGui to quickly allow writing compute shaders. It also includes multiple features to analyze the simulations you've written.

## Features
- Simulation
  - Size and seed selection
  - Control target FPS
    - Thread sleep
    - Frame step (F)
  - Restart (F5)
- Interface
  - Zoom on mouse or on screen center
  - Pan with keyboard (WASD / arrows) or mouse
  - Metrics window
    - Control position and displayed info
  - Log window
  - Toggle fullscreen (F11)
  - Font resize
- Debug
  - Log software versions
  - Pack shaders into build

## Requirements
- An OpenGL 4.6 capable GPU

## Using OCSFW
1. Add OCSFW as a submodule to your project
2. Create a child class of Simulation.h
3. Call OSCFW's constructor with your simulation and a title

A template for a basic project is available [here](https://github.com/psychopattt/OCSFW-Galaxy)<br>
An example of a larger project is available [here](https://github.com/psychopattt/Game-of-Life-OpenGL)

## Credits
- [GLFW](https://www.glfw.org)
- [GLAD](https://github.com/Dav1dde/glad)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Proggy Fonts](https://github.com/bluescan/proggyfonts)
