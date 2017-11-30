

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

    #r = LEDTableController(DISPLAY_WIDTH,DISPLAY_HEIGHT, debugMode=True)
    r = DesktopController(DISPLAY_WIDTH,DISPLAY_HEIGHT, debugMode=True, upscale=30)
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

    input = DesktopInput(debouncedInput = True)
    r.setInputHandler(input)
    r.addApplication(menu)

    r.run(FPS=25)
    r.shutdown()
    exit()
