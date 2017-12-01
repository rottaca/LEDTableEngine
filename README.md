# LEDTableEngine
This repository contains a python implementation of a controll engine for LED tables. It also contains a simulation environment for desktop applications.
### Controllers
The Desktop Controller uses Tkinter to display the matrix with on an upscaled display. 
The LEDTable controller outputs display data as TPM2 packets which are then transmitted to an WS2812B Controller chip that controls the actual LED matrix.

### Applications
Different applications are now launched on top of these controllers. The simple design of controllers and applications allows a fast implementation of different applications.
At the moment, there are the following Applications implemented:
  - Menu Application (Displays menus with scroll text)
  - Snake (Simple implementation of the snake game)
  - Various test patterns (Color gradients, bouncing box, random dots)
  
### Input handlers
These applications can be controlled from various types of input sources. For now, the following input controllers are implemented:
 - Keyboard Input
 - Planned: I2C-Raspberry Pi gaming controllers
 - Final Goal: Touch control input from each LED Matrix cell (realized with IR Sender/Reciever pairs)

  
