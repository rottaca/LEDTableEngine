from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
import threading
from thread import start_new_thread

from base import BaseController
from base import BaseInput, KeyEvent
from pynput import keyboard

import time

class DesktopController(BaseController):

    def __init__(self, width, height, debugMode, upscale=10):
        super(DesktopController, self).__init__(width, height, debugMode)
        self.gui_width = self.width*upscale
        self.gui_height = self.height*upscale
        self.upscale = upscale

    def initialize(self):
        super(DesktopController, self).initialize()
        self.root = tk.Tk()
        self.root.resizable(width=False, height=False)
        self.root.geometry('{}x{}'.format(self.width*self.upscale, self.height*self.upscale))
        self.label = tk.Label(self.root)
        self.label.pack(fill=tk.BOTH, expand=1)

    def shutdown(self):
        self.root.quit()

    def showFrame(self):
        resized = self.frame.resize((self.gui_width,self.gui_height),Image.NEAREST)

        # Draw grid overlay
        draw = ImageDraw.Draw(resized)
        sx = self.gui_width/self.width
        sy = self.gui_height/self.height
        for x in range(0,self.width):
            draw.line([x*sx, 0, x*sx, self.gui_height])
        for y in range(0,self.height):
            draw.line([0, y*sy, self.gui_width, y*sy])

        self.tkimage = ImageTk.PhotoImage(resized)
        self.label.configure(image=self.tkimage)
        self.root.update_idletasks()
        self.root.update()
        return True


class DesktopInput(BaseInput):

    def __init__(self, debouncedInput = True):
        super(DesktopInput, self).__init__()
        start_new_thread(self.processKeyboard,())
        self.keys = {}
        self.debounce = debouncedInput
        self.keyMapping = {"w":"up",
                           "a":"left",
                           "s":"down",
                           "d":"right",
                           "q":"quit",
                           "e":"enter"}
        print "Key mapping: ", self.keyMapping

    def processKeyboard(self):
        with keyboard.Listener(
                on_press=self.callbackPress,
                on_release=self.callbackRelease) as listener:
            listener.join()

    def callbackPress(self,k):
        try: k = k.char
        except: k = k.name
        if k not in self.keys:
            self.keys[k] = 0

    def callbackRelease(self,k):
        try: k = k.char
        except: k = k.name
        if k in self.keys:
            del self.keys[k]

    def pollKeys(self):
        tmp = []
        for k, s in self.keys.iteritems():
            if self.debounce:
                if s == 0:
                    self.keys[k] = 1
                else:
                    continue

            if k in self.keyMapping:
                tmp.append(KeyEvent(self.keyMapping[k]))

        return tmp
