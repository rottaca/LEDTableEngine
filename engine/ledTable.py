from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk

from base import BaseController
from base import BaseInput, KeyEvents

class LEDTableController(BaseController):

    def __init__(self, width, height,  upscale=10):
        super(LEDTableController, self).__init__(width, height)

    def showFrame(self):

        return True

class LEDTableInput(BaseInput):

    def __init__(self):
        super(LEDTableInput, self).__init__()

    def setController(self, controller):
        super(LEDTableInput, self).setController(controller)

    def pollKeys(self):
        return []
