
import time
from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
from random import randint

from engine.base import Application

class Snake(Application):

    def __init__(self, width, height):
        super(Snake, self).__init__(width, height)
        self.initialize()

    def initialize(self):
        self.snake = [(self.width/2, self.height/2)]
        self.dir = (0,1)
        self.food = (self.width/2 , self.height/2 + 5 )
        self.last_step_time = 0
        self.redraw_frame = True
        self.finished = False

    def processInput(self, controller, inputs, delta_time):
        if self.finished:
            return True

        tmpDir = self.dir
        for i in inputs:
            if i.name is "up":
                newDir = (0, -1)
            elif i.name is "down":
                newDir = (0, 1)
            elif i.name is "left":
                newDir = (-1, 0)
            elif i.name is "right":
                newDir = (1, 0)
            else:
                continue

            if newDir[0] == -self.dir[0] and newDir[1] == -self.dir[1]:
                pass
            else:
                self.dir = newDir

        if controller.getTime() - self.last_step_time < 1000.0/(0.2*len(self.snake)+1):
            return True

        self.last_step_time = controller.getTime()
        self.redraw_frame = True
        newPos = (self.snake[0][0] + self.dir[0], self.snake[0][1] + self.dir[1])

        if newPos[0] < 0 or newPos[0] >= self.width or newPos[1] < 0 or newPos[1] >= self.height or newPos in self.snake:
            self.finished = True
        else:
            self.snake.insert(0, newPos)

            if newPos != self.food:
                self.snake.pop()
            else:
                self.food = (randint(0,self.width-1), randint(0,self.height-1))

        return True

    def draw(self, controller, frame, draw, delta):
        if not self.redraw_frame:
            return False

        self.redraw_frame = False

        if self.finished:
            controller.clearFrame(clearColor=(255,0,0))
        else:
            controller.clearFrame()

        draw.point(self.food,fill=(0,255,0))

        for p in self.snake:
            draw.point(p, fill=(128,128,128))

        return True
