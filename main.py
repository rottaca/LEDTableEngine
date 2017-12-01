
import argparse

from engine.desktop import DesktopController, DesktopInput
from engine.ledTable import LEDTableController, LEDTableInput, TPM2

from applications.testpattern import TestPattern
from applications.snake import Snake
from applications.bubbleshooter import BubbleShooter
from applications.raininglava import RainingLava
from applications.menu import Menu

DISPLAY_WIDTH = 20
DISPLAY_HEIGHT = 15

if __name__ == '__main__':

    parser = argparse.ArgumentParser()
    parser.add_argument('--display',default="desktop", choices=["desktop","table"], help="Render device / controller.")
    parser.add_argument('--input', default="keyboard", choices=["keyboard","controller","touch"], help="Input device that passes controls to controller.")
    parser.add_argument('--ledDisplayDevice', default="/dev/ttyACM0", help="Serial device for LED Matrix control.")
    parser.add_argument('--FPS', default=30, type=int, help="Processing framerate for applications.")
    parser.add_argument('--debug', default=False, action='store_true', help="Enable debug output.")
    args = parser.parse_args()
    
    if args.debug:
        print "Parameters: ", args

    inputs = {
        "keyboard" : DesktopInput(debouncedInput = True),
        "controller" : LEDTableInput()
    }
    controllers = {
        "desktop" : DesktopController(DISPLAY_WIDTH,DISPLAY_HEIGHT, debugMode=args.debug, upscale=30),
        "table" : LEDTableController(DISPLAY_WIDTH,DISPLAY_HEIGHT, debugMode=args.debug, serialPort=args.ledDisplayDevice)
    }

    test = TestPattern(mode=0)
    snake = Snake()
    shoot = BubbleShooter()
    rainingLava = RainingLava()

    menu2 = Menu(isClosable=True)
    menu2.setMenuItemsAndApps(
        [("Entry1",0)])

    menu = Menu(isClosable=True)
    menu.setMenuItemsAndApps(
        [("Test Pattern",test),
         ("Snake",snake),
         ("Bubbleshooter", shoot),
         ("Raining Lava", rainingLava),
         ("Submenu",menu2)])

    r = controllers[args.display]
    r.initialize()
    input = inputs[args.input]

    r.setInputHandler(input)
    r.addApplication(menu)

    r.run(FPS=args.FPS)
    r.shutdown()
    exit(0)
