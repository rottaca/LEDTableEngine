from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
import threading
from thread import start_new_thread

from base import BaseController
from base import BaseInput, KeyEvent
from pynput import keyboard

import time

class DesktopController(BaseController):

    def __init__(self, width, height,  upscale=10):
        super(DesktopController, self).__init__(width, height)
        self.gui = TkInterGUIThread(self, width, height,  upscale)
        self.gui_width = width*upscale
        self.gui_height = height*upscale

    def onExit(self):
        self.gui.close()
        self.gui.join()

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

        self.gui.setFrame(resized)

        return True


class TkInterGUIThread(threading.Thread):
    def __init__(self, controller, width, height, upscale):
        threading.Thread.__init__(self)
        self.start()
        self.width = width
        self.height = height
        self.upscale = upscale
        self.controller = controller
        self.frame = None
        self.closed = False

    def setFrame(self,frame):
        if not self.closed:
            self.frame = frame
            self.root.after(1, self.updateFrame)

    def close(self):
        self.closed = True
        self.root.quit()

    def updateFrame(self):
        if self.frame and not self.closed:
            self.tkimage = ImageTk.PhotoImage(self.frame)
            self.label.configure(image=self.tkimage)
            self.frame = None

    def run(self):
        self.root = tk.Tk()
        self.root.resizable(width=False, height=False)
        self.root.geometry('{}x{}'.format(self.width*self.upscale, self.height*self.upscale))
        self.label = tk.Label(self.root)
        self.label.pack(fill=tk.BOTH, expand=1)
        self.root.protocol("WM_DELETE_WINDOW", self.close)
        self.root.mainloop()

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
