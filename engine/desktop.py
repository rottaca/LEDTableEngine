from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk

from base import BaseController
from base import BaseInput, KeyEvents

class DesktopController(BaseController):

    def __init__(self, width, height,  upscale=10):
        super(DesktopController, self).__init__(width, height)
        self.root = tk.Tk()
        self.root.resizable(width=False, height=False)
        self.root.geometry('{}x{}'.format(width*upscale, height*upscale))
        self.tkimage = ImageTk.PhotoImage(self.frame)
        self.label = tk.Label(self.root,image=self.tkimage)
        self.label.pack(fill=tk.BOTH, expand=1)
        self.root.update_idletasks()
        self.root.update()


    def showFrame(self):
        resized = self.frame.resize((self.root.winfo_width(),self.root.winfo_height()),Image.NEAREST)

        # Draw grid overlay
        draw = ImageDraw.Draw(resized)
        sx = self.root.winfo_width()/self.width
        sy = self.root.winfo_height()/self.height
        for x in range(0,self.width):
            draw.line([x*sx, 0, x*sx, self.root.winfo_height()])
        for y in range(0,self.height):
            draw.line([0, y*sy, self.root.winfo_width(), y*sy])

        self.tkimage = ImageTk.PhotoImage(resized)
        self.label.configure(image=self.tkimage)

        self.root.update_idletasks()
        self.root.update()

        return True

class DesktopInput(BaseInput):

    def __init__(self):
        super(DesktopInput, self).__init__()
        self.keys = set()

    def setController(self, controller):
        super(DesktopInput, self).setController(controller)
        self.controller = controller
        self.controller.root.bind_all("<KeyPress>", self.callbackPress)
        self.controller.root.bind_all("<KeyRelease>", self.callbackRelease)

    def callbackPress(self,event):
        k = str(event.char)
        self.keys.add(k)

    def callbackRelease(self,event):
        k = str(event.char)
        self.keys.discard(k)

    def pollKeys(self):
        tmp = []

        for k in self.keys:
            if k is "w":
                tmp.append(KeyEvents("up"))
            elif k is "a":
                tmp.append(KeyEvents("left"))
            elif k is "s":
                tmp.append(KeyEvents("down"))
            elif k is "d":
                tmp.append(KeyEvents("right"))
            elif k is "b":
                tmp.append(KeyEvents("enter"))

        return tmp
