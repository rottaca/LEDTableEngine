# LEDTableEngine
This repository contains a C++ implementation of a control engine for LED tables. It also contains a simulation environment for desktop applications. It contains different controllers, input handlers and applications and helper functions for any kind of game or visualization.

### Controllers
The desktop controller uses SDL2 to display the matrix with on an upscaled display.
The matrix controller outputs display data as TPM2 packets which are then transmitted to an WS2812B Controller chip that controls the actual LED matrix.

### Input Handlers
The applications, running on the engine controllers, can be controlled from various types of input sources. Each input controller can handle multiple players. For now, the following input controllers are implemented:
 - Keyboard Input (requires root permissions)
 - I2C-Raspberry Pi gaming controllers (3D-Printing and setup instructions: https://www.thingiverse.com/thing:2728020)

### Applications
Different applications are launched on top of these controllers and input handlers. The simple design of controllers and applications allows an easy implementation of different applications, games or visualizations.

At the moment, the following applications are implemented:
  - Menu Application (Displays menus with scroll text)
  - Snake
  - Pong
  - Tetris
  - Conway's Game Of Life
  - Flappy FlappyBird
  - Fractal visualization
  - Image slide show with fading (from downsampeled image files, see res/imageSlideShowApp)
  - Shader App: Implemented effects are water shader, chess board and RGB color waves
  - Update App: This app automatically pulls this project and compiles the executable. On success, the Engine restarts if correctly installed (see below).
  - Test pattern (Color gradients, random dots)

# Setup Instructions
coming soon :)
