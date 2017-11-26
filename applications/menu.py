
import time
from PIL import ImageTk, Image, ImageDraw, ImageFont
import Tkinter as tk
from random import randint

from engine.base import Application

class Menu(Application):

    def __init__(self, isClosable):
        super(Menu, self).__init__()
        self.isClosable = isClosable
        self.menu = None

    def initialize(self, controller,  width, height):
        super(Menu,self).initialize(controller,  width, height)
        self.last_step_time = 0
        self.time_text_wrap_initial_wait = 1000
        self.time_text_wrap_start = 0
        self.time_text_wrap_per_char = 100
        self.itemIdx = 0
        self.font = ImageFont.load_default()
        self.textsize = (0,0)
        self.scrollPixels = 0
        self.currScrollPixels = 0
        self.redraw_frame = True

        if self.menu:
            self.updateText()

    def setMenuItemsAndApps(self, menuMap):
        self.menu = menuMap
        if self.isClosable:
            self.menu.insert(len(self.menu),("Back",None))

    def processInput(self, inputs, delta_time):
        t = self.controller.getTime()

        if ((self.currScrollPixels not in [0,self.scrollPixels-1] and t - self.time_text_wrap_start >= self.time_text_wrap_per_char) or
            (self.currScrollPixels in [0,self.scrollPixels-1]  and t - self.time_text_wrap_start >= self.time_text_wrap_per_char + self.time_text_wrap_initial_wait)):
            self.time_text_wrap_start = t
            self.currScrollPixels = self.currScrollPixels + 1
            if self.currScrollPixels >= self.scrollPixels:
                self.currScrollPixels = 0
            self.redraw_frame = True

        if t - self.last_step_time >=0:
            self.last_step_time = t

            menuChanged = False
            for i in inputs:
                if i.name is "up":
                    self.itemIdx = self.itemIdx - 1
                    if self.itemIdx < 0:
                        self.itemIdx = 0
                    else:
                        self.redraw_frame = True
                        menuChanged = True
                elif i.name is "down":
                    self.itemIdx = self.itemIdx + 1
                    if self.itemIdx >= len(self.menu):
                        self.itemIdx = len(self.menu)-1
                    else:
                        self.redraw_frame = True
                        menuChanged = True
                elif i.name is "enter":
                    # Last entry is "back"
                    if self.isClosable and self.itemIdx == len(self.menu)-1:
                        self.finished = True
                    # Otherwise append new application to application stack
                    elif self.menu[self.itemIdx][1] != 0:
                        self.controller.addApplication(self.menu[self.itemIdx][1])

            if menuChanged:
                self.updateText()
                self.time_text_wrap_start = t

        return True

    def updateText(self):
        img = Image.new("RGBA", (1,1))
        draw = ImageDraw.Draw(img)
        self.textsize = draw.textsize(self.menu[self.itemIdx][0], self.font)
        self.scrollPixels = self.textsize[0] - self.width + 4
        self.currScrollPixels = 0

    def draw(self, frame, draw, delta):
        if not self.redraw_frame:
            return False
        self.controller.clearFrame()
        self.redraw_frame = False

        if self.itemIdx > 0:
            draw.line([self.width/2 - 1, 1, self.width/2 + 1, 1], fill=(255,0,0))
            draw.point([self.width/2,0], fill=(255,0,0))

        draw.text((2-self.currScrollPixels, self.height/2 - self.textsize[1] / 2), self.menu[self.itemIdx][0], font=self.font, fill=(0,200,0))

        if self.itemIdx < len(self.menu)-1:
            draw.line([self.width/2 - 1, self.height-2, self.width/2 + 1, self.height-2], fill=(255,0,0))
            draw.point([self.width/2,self.height-1], fill=(255,0,0))

        return True
