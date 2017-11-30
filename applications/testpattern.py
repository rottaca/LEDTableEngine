
import time
from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
from random import randint

from engine.base import Application

class TestPattern(Application):

    def __init__(self, mode):
        super(TestPattern, self).__init__()
        self.modeIdx = mode
        self.modes = [self.interpolate, self.bouncing, self.randomColors]
        self.interpolation = 0.0
        self.interpolateDir = 1

    def initialize(self, controller,  width, height):
        super(TestPattern,self).initialize(controller,  width, height)
        self.pos = [0,0]
        self.dir = [1,1]
        self.last_step_time = 0

    def processInput(self, inputs, delta_time):
        if any("quit" == x.name for x in inputs):
            self.finished = True
            return True

        if any("enter" == x.name for x in inputs):
            self.modeIdx = self.modeIdx + 1
            if self.modeIdx >= len(self.modes):
                self.modeIdx = 0


        if self.controller.getTime() - self.last_step_time < 100:
            self.redraw_frame = False
            return True

        self.last_step_time = self.controller.getTime()
        self.redraw_frame = True

        if self.modes[self.modeIdx] == self.bouncing:
            self.pos[0] = self.pos[0] + self.dir[0]
            self.pos[1] = self.pos[1] + self.dir[1]

            if(self.pos[1] > self.height - 1):
                self.dir[1] = -self.dir[1]
                self.pos[1] = self.height - 1
            elif(self.pos[1] < 0):
                self.dir[1] = -self.dir[1]
                self.pos[1] = 0

            if(self.pos[0] > self.width - 1):
                self.dir[0] = -self.dir[0]
                self.pos[0] = self.width - 1
            elif(self.pos[0] < 0):
                self.dir[0] = -self.dir[0]
                self.pos[0] = 0

        elif self.modes[self.modeIdx] == self.interpolate:
            self.interpolation = self.interpolation + self.interpolateDir*delta_time/2000.0
            print self.interpolation
            if self.interpolation > 1:
                self.interpolation = 1
                self.interpolateDir = -self.interpolateDir
            elif self.interpolation < 0:
                self.interpolation = 0
                self.interpolateDir = -self.interpolateDir

        return True

    def draw(self, frame, draw, delta):
        return self.modes[self.modeIdx](frame, draw, delta)

    def continueApp(self):
        super(TestPattern,self).continueApp(self.controller)
        self.controller.clearFrame()

    def bouncing(self, frame, draw, delta):
        self.controller.clearFrame()

        px = round(self.pos[0])
        py = round(self.pos[1])
        draw.rectangle([px-2, py-2, px+2, py+2], outline=(0,100,54))
        draw.point([px, py], fill=(255,0,0))
        return True

    def randomColors(self, frame, draw, delta):
        # Set random pixel to a color
        draw.point([randint(0, self.width), randint(0, self.height)], fill=(randint(100, 255),randint(100, 255),randint(100, 255)))
        # Clear random pixel
        draw.point([randint(0, self.width), randint(0, self.height)], fill=(0,0,0))
        return True

    def interpolate(self, frame, draw, delta):
        color = (0,0,0)
        if self.interpolation < 0.5:
            s = self.interpolation*2
            color = (int(255*(1 - s)), int(255*s), 0)
        else:
            s = (self.interpolation-0.5)*2
            color = (0, int(255*(1-s)), int(255*(s)))

        draw.rectangle([0,0, self.width,self.height],fill=color)
        return True
