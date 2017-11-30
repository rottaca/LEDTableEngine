
import time
from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
from random import randint

from engine.base import Application

class RainingLava(Application):

    def __init__(self):
        super(RainingLava, self).__init__()
        self.palettes = {
                     "fire": [0,0,0,
                             255,0,0,
                             255,141,0,
                             255,180,02,
                             255,206,0,
                             255,231,0],
                      "water": [0,0,0,
                              15,94,156,
                              35,137,218,
                              28,163,236,
                              90,188,216,
                              116,204,244]
                         }

        # Flow direction (up down)
        # Clearing (yes/no)
        self.paletteSettings = {
                        "fire":(-1,1),
                        "water":(1,0),}
        self.activePalette = "fire"


    def updatePalette(self):
        palette = list(self.palettes[self.activePalette])
        palette.extend([0 for i in range(0, 3*(256 - len(palette)/3))])
        self.gameField.paste(0,(0,0,self.width, self.height))
        self.gameField.putpalette(palette)

    def initialize(self, controller,  width, height):
        super(RainingLava,self).initialize(controller,  width, height)
        self.last_step_time = 0
        self.gameField = Image.new('P', [self.width, self.height])
        self.updatePalette()

    def processInput(self, inputs, delta_time):
        if any("quit" == x.name for x in inputs):
            self.finished = True
            return True

        if any("enter" == x.name for x in inputs):
            found = False
            updated = False
            for key in self.palettes.keys():
                if found:
                    self.activePalette = key
                    updated = True
                    break
                if key == self.activePalette:
                    found = True

            if not updated:
                self.activePalette = self.palettes.keys()[0]

            self.updatePalette()

        if self.controller.getTime() - self.last_step_time < 100:
            return True

        self.last_step_time = self.controller.getTime()
        self.redraw_frame = True

        settings = self.paletteSettings[self.activePalette]
        paletteSize = len(self.palettes[self.activePalette]) / 3
        if settings[0] == -1:
            cpy = self.gameField.copy()
            self.gameField.paste(cpy, (0,1))
            if settings[1] == 1:
                self.gameField.paste(0, (0,0,self.width,1))

            for i in range(0,5):
                self.gameField.putpixel([randint(0,self.width-1), 0], randint(1,paletteSize))
        else:
            cpy = self.gameField.crop([0, 1, self.width, self.height])
            cpy.load()
            self.gameField.paste(cpy, (0,0))
            if settings[1] == 1:
                self.gameField.paste(0, (0,self.height-1,self.width,self.height))
            for i in range(0,5):
                self.gameField.putpixel([randint(0,self.width-1), self.height-1], randint(1,paletteSize))

        return True

    def draw(self, frame, draw, delta):
        self.redraw_frame = False
        frame.paste(self.gameField)
