
import time
import random
from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
from random import randint

from LEDTableEngine.base import Application

class BubbleShooter(Application):



    def __init__(self):
        super(BubbleShooter, self).__init__()

    def initialize(self, controller,  width, height):
        super(BubbleShooter,self).initialize(controller,  width, height)
        self.bubbles = [] #List for all bubbles on the field
        self.cannon = [] # List with the points for the main sturcture
        self.pipe = [] # cannon pipe
        self.wall = [] # cannon wall
        self.createField()


    def processInput(self, inputs, delta_time):
        if any("quit" == x.name for x in inputs):
            self.finished = True
            return True


        return True

    def draw(self, frame, draw, delta):
        # Draw only once
        self.redraw_frame = False

        self.controller.clearFrame()

        # Draw bubbles
        for bubble in self.bubbles:
            if bubble[2] == 0:
                red = 255
            else:
                red = 0
            if bubble[2] == 1:
                green = 255
            else:
                green = 0
            if bubble[2] == 2:
                blue = 255
            else:
                blue = 0
            draw.point([bubble[0],bubble[1]],fill=(red,green,blue))


        # Draw canon pipe
        for point in self.wall:
            draw.point([point[0], point[1]],fill=(139,105,0))
        # Draw canon
        for point in self.cannon:
            draw.point([point[0], point[1]],fill=(105,105,105))
        # Draw canon pipe
        for point in self.pipe:
            draw.point([point[0], point[1]],fill=(238,238,0))


        return True

    def createField(self):

        # Bubbles
        for i in range(0,20): # 20 initial bubbles
            for j in range (0,3):
                color = random.randint(0,2) #0 = R, 1 = G, 2 = B
                self.bubbles.append([i, j, color])


        # Canon
        self.cannon.append([9,14])
        self.cannon.append([10,14])
        self.cannon.append([9,13])
        self.cannon.append([10,13])
        self.cannon.append([11,13])
        self.cannon.append([11,14])
        # Initial canon pipe
        self.pipe.append([10,12])
        self.pipe.append([10,11])
        self.pipe.append([10,10])
        # Wall cannon
        for i in range(0,20):
            self.wall.append([i,14])

        return True
