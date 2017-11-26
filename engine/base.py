from PIL import Image, ImageDraw
import time

class BaseController(object):
    def __init__(self, width, height, debugMode=False):
        self.width = width
        self.height = height
        self.createFrame()
        self.input = None
        self.appStack = []
        self.queuedInsertions = []
        self.debugMode = debugMode

    def setInputHandler(self, input):
        self.input = input
        self.input.setController(self)

    def createFrame(self):
        self.frame = Image.new('RGB', [self.width, self.height], (0,0,0));
        self.draw = ImageDraw.Draw(self.frame)

    def clearFrame(self, clearColor=(0,0,0)):
        self.draw.rectangle([(0, 0), (self.width, self.height)],fill=clearColor)

    def getTime(self):
        return int(round(time.time() * 1000))

    # Insert a new app into the app stack
    # set queuedInsertions to True, if the insertion happens
    # during runtime!
    def addApplication(self, application, queueInsertion=False):
        if not queueInsertion:
            if len(self.appStack) > 0:
                self.appStack[-1].pauseApp()
            self.appStack.insert(len(self.appStack),application)
            application.initialize(self,self.width, self.height)
        else:
            self.queuedInsertions.insert(len(self.queuedInsertions),application)

    def run(self, FPS=50):
        if not self.input:
            print "Input controller not set!"
            return

        FPS_ms = 1000.0/FPS
        last_time = new_time =  self.getTime()
        averageDispUpdateTime = 0
        averageProcessingTime = 0
        lastDisplayUpdate = new_time
        lastDebugStatsUpdate = new_time

        self.showFrame()

        if len(self.appStack) > 0:
            self.appStack[-1].continueApp()

        while(len(self.appStack) > 0):

            inputs = self.input.pollKeys()

            if self.debugMode and len(inputs) > 0:
                print "Inputs: ", inputs

            delta_time = new_time - last_time
            averageProcessingTime = 0.9*averageProcessingTime + 0.1*delta_time

            self.appStack[-1].processInput(inputs, delta_time)

            if self.appStack[-1].requiresRedraw():
                self.appStack[-1].draw(self.frame, self.draw, delta_time)
                self.showFrame()
                averageDispUpdateTime = 0.9*averageDispUpdateTime + 0.1*(new_time - lastDisplayUpdate)
                lastDisplayUpdate = new_time

            if self.debugMode and new_time - lastDebugStatsUpdate  > 1000:
                lastDebugStatsUpdate = new_time
                print "Debug Statistics"
                print "------------------------------------------------------"
                print "Display Rate: ", 1000.0/averageDispUpdateTime, " FPS"
                print "Processing Rate: ", 1000.0/averageProcessingTime, " FPS"
                print "Apps on Stack:", len(self.appStack)
                print "Pending apps for insert:", len(self.queuedInsertions)
                print "------------------------------------------------------"

            # Are there any delayed insertions ?
            if len(self.queuedInsertions) > 0:
                for i in self.queuedInsertions:
                    if len(self.appStack) > 0:
                        self.appStack[-1].pauseApp()
                    self.appStack.insert(len(self.appStack),i)
                    i.initialize(self,self.width, self.height)
                self.queuedInsertions = ()

            # App has finished processing ?
            # Remove it from stack, and continue with the previous one
            if self.appStack[-1].hasFinished():
                self.appStack.pop()
                if len(self.appStack) > 0:
                    self.appStack[-1].continueApp()

            last_time = new_time
            new_time = self.getTime()

            if(new_time - last_time < FPS_ms):
                time.sleep((FPS_ms - (new_time - last_time))/1000.0)
                new_time = self.getTime()

    def shutdown(self):
        raise NotImplementedError("Please Implement this method")

    def showFrame(self):
        raise NotImplementedError("Please Implement this method")

class Application(object):

    # This function is called once, for each time, the application has been launched.
    def initialize(self, controller,  width, height):
        self.width = width
        self.height = height
        self.controller = controller
        self.paused = False
        self.finished = False
        self.redraw_frame = True

    # This function is called for each frame, before "draw" is called.
    # controller: Reference to the controller object to get time e.g.
    # key_list: List of keys that are currently pressed (see BaseInput)
    # delta_time: Time that has passed since the last call
    def processInput(self, key_list, delta_time):
        raise NotImplementedError("Please Implement this method")

    # This function is called for each frame, after "processInput" is called.
    # This function is only called, if self.redraw_frame is True.
    # Use this to minimize the nunber of drawcalls!!
    # controller: Reference to the controller object to clear the frame buffer
    # frame: Reference to the frame buffer (Pillow Image)
    # draw: Reference to the draw object, that is used to draw the image (ImageDraw)
    # delta_time: Time that has passed since the last call
    def draw(self, frame, draw, delta_time):
        raise NotImplementedError("Please Implement this method")

    # Returns true if the application has finished processing.
    # Afterwards the application is removed from the stack
    def hasFinished(self):
        return self.finished

    # Is called if the app now running and the previous application has been stopped
    def continueApp(self):
        self.paused = False

    # This is called if the app has launched a new app and the current app is now longer
    # the active one
    def pauseApp(self):
        self.paused = True

    def requiresRedraw(self):
        return self.redraw_frame

class BaseInput(object):

    def setController(self, controller):
        self.controller = controller

    def pollKeys():
        raise NotImplementedError("Please Implement this method")

class KeyEvent(object):
    def __init__(self, name, state=""):
        self.name = name
        self.state = state

    def __repr__(self):
        return self.name
