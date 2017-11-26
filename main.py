

from engine.desktop import DesktopController, DesktopInput
from engine.ledTable import LEDTableController, LEDTableInput, TPM2

from applications.testpattern import TestPattern
from applications.snake import Snake
from applications.bubbleshooter import BubbleShooter
from applications.menu import Menu

DISPLAY_WIDTH = 20
DISPLAY_HEIGHT = 15

if __name__ == '__main__':

    r = DesktopController(DISPLAY_WIDTH,DISPLAY_HEIGHT, debugMode=True, upscale=30)
    test = TestPattern(mode=0)
    snake = Snake()
    shoot = BubbleShooter()

    menu2 = Menu(isClosable=True)
    menu2.setMenuItemsAndApps(
        [("Entry1",0)])

    menu = Menu(isClosable=True)
    menu.setMenuItemsAndApps(
        [("Test Pattern",test),
         ("Snake",snake),
         ("Bubbleshooter", shoot),
         ("Submenu",menu2)])

    input = DesktopInput(debouncedInput = True)
    r.setInputHandler(input)
    r.addApplication(menu)

    r.run(FPS=40)
    r.shutdown()
    exit()

    r = LEDTableController(DISPLAY_WIDTH,DISPLAY_HEIGHT,serialPort="/dev/pts/18", debugMode=True)

    t = TPM2()
    p = t.createCmdPacket("saveCfg", [])
    print "SaveCfg: ", [int(i) for i in p]
    p = t.createDataPacket([0,1,2,3,4,5,6,7,8,9])
    print "Data: ", [int(i) for i in p]
