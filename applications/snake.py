
import time
from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
from random import randint

from engine.base import Application

class Snake(Application):

    def __init__(self):
        super(Snake, self).__init__()

    def initialize(self, controller,  width, height):
        super(Snake,self).initialize(controller,  width, height)
        self.snake = [(self.width/2, self.height/2)]
        self.dir = (0,1)
        self.food = (self.width/2 , self.height/2 + 5 )
        self.last_step_time = 0

    def processInput(self, inputs, delta_time):
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
            elif i.name is "quit":
                self.finished = True
                return True
            else:
                continue

            if newDir[0] == -self.dir[0] and newDir[1] == -self.dir[1]:
                pass
            else:
                self.dir = newDir

        if self.controller.getTime() - self.last_step_time < 1000.0/(0.2*len(self.snake)+1):
            self.redraw_frame = False
            return True

        self.last_step_time = self.controller.getTime()
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

    def draw(self, frame, draw, delta):
        if self.finished:
            self.controller.clearFrame(clearColor=(255,0,0))
        else:
            self.controller.clearFrame()

        draw.point(self.food,fill=(0,255,0))

        for p in self.snake:
            draw.point(p, fill=(128,128,128))

        return True
