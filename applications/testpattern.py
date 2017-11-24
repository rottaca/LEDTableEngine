
import time
from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
from random import randint

from engine.base import Application

class TestPattern(Application):

    def __init__(self, width, height, mode):
        super(TestPattern, self).__init__(width, height)
        self.modeIdx = mode
        self.modes = [self.bouncing, self.randomColors]
        self.initialize()

    def initialize(self):
        self.pos = [0,0]
        self.dir = [1,1]
        self.last_step_time = 0
        self.finished = False

    def processInput(self, controller, inputs, delta_time):
        if any("enter" == x.name for x in inputs):
            self.finished = True
            return True

        if any("up" == x.name for x in inputs):
            self.modeIdx = self.modeIdx + 1
            if self.modeIdx >= len(self.modes):
                self.modeIdx = 0
            print "New Mode: ", self.modeIdx

        if controller.getTime() - self.last_step_time < 100:
            return True
        self.last_step_time = controller.getTime()

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

        return True

    def draw(self, controller, frame, draw, delta):
        return self.modes[self.modeIdx](controller, frame, draw, delta)

    def continueApp(self, controller):
        super(TestPattern,self).continueApp(controller)
        controller.clearFrame()

    def bouncing(self, controller, frame, draw, delta):
        controller.clearFrame()

        px = round(self.pos[0])
        py = round(self.pos[1])
        draw.rectangle([px-2, py-2, px+2, py+2], outline=(0,100,54))
        draw.point([px, py], fill=(255,0,0))
        return True

    def randomColors(self, controller, frame, draw, delta):
        #controller.clearFrame()
        # Set random pixel to a color
        draw.point([randint(0, self.width), randint(0, self.height)], fill=(randint(100, 255),randint(100, 255),randint(100, 255)))
        # Clear random pixel
        draw.point([randint(0, self.width), randint(0, self.height)], fill=(0,0,0))
        return True
