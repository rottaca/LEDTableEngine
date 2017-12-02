from PIL import ImageTk, Image, ImageDraw
import Tkinter as tk
import serial
import itertools
import numpy as np

from base import BaseController
from base import BaseInput, KeyEvent

class LEDTableController(BaseController):

    def __init__(self, width, height, debugMode = False, serialPort="/dev/ttyACM0", baud=115200, timeout=1):
        super(LEDTableController, self).__init__(width, height, debugMode)
        self.tpm2 = TPM2()
        self.serialPort = serialPort
        self.baud = baud
        self.timeout = timeout

    def initialize(self):
        super(LEDTableController, self).initialize()
        self.serial = serial.Serial(self.serialPort, timeout=self.timeout, rtscts=True, dsrdtr=True)
        print "Opened serial port: ", self.serial

        # p = self.tpm2.createCmdPacket("rwConnPixelCnt",[50])
        # res = self.serialTPM2(p)
        # if len(res) == 1:
        #     print "Result: " < self.tpm2.ackByte[res]
        # else:
        #     print "No answer recieved"


    def showFrame(self):
        # Swap row to column order of pixels for output
        img_data = np.array(self.frame.getdata()).reshape(self.height, self.width, 3)
        img_data2 = np.zeros([self.width, self.height, 3],dtype=np.int)
        for x in range(0,self.width):
            for y in range(0, self.height):
                img_data2[x][y] = img_data[y][x]

        p = self.tpm2.createDataPacket(img_data2.flatten())
        res = self.serialTPM2(p)
        return True

    def serialTPM2(self, packet):
        sz = self.serial.write(packet)
        if sz != len(packet):
            print "Failed to write all data"
            return bytes()
        if packet[1] == self.tpm2.blockTypes["cmd"] and ((packet[4] & 0x07 == 0) or (packet[4] & 0x06 == 1)):
           return self.serial.read()
        else:
           return bytes()

class LEDTableInput(BaseInput):

    def __init__(self):
        super(LEDTableInput, self).__init__()

    def setController(self, controller):
        super(LEDTableInput, self).setController(controller)

    def pollKeys(self):
        return []

class TPM2:
    def __init__(self):
        self.startByte = 0xC9
        self.blockTypes = {
                        "data":0xDA,
                        "cmd":0xC0,
                        "ack":0xAC,
                        "ackdata":0xAD
                        }
        self.endByte = 0x36
        self.cmdTypes = {
                        "config":0x00,
                        "saveCfg":0x01,
                        "loadCfg":0x02,
                        "initWriteSD":0x03,
                        "startPlaySD":0x04,
                        "stopPlaySD":0x05,
                        "rwFrameSD":0x06,

                        "rwMasterIntensity":0x0A,
                        "rwGammaCorrect":0x0B,
                        "rwMasterSpeed":0x0C,
                        "rwCurrProgNr":0x0D,
                        "rwStartColor":0x0E,
                        "rwGlobalTimeOut":0x0F,
                        "rwConnPixelCnt":0x10,
                        "rwRepeatCnt":0x11,
                        "rwStartAddr":0x12,

                        "ping":0x20
                        }
        self.ackByte = {
                        0x00:"Command and param recieved",
                        0x01:"Command recieved, but param error",
                        0x02:"Unknwon command",
                        0x03:"Protocol error",
                        0x04:"Data too big"
                        }

    def createPacket(self, type, data):
        szHigh = (len(data) >> 0x08) & 0xFF
        szLow = len(data) & 0xFF
        arr = [self.startByte, self.blockTypes[type], szHigh, szLow]
        arr.extend(data)
        arr.append(self.endByte)
        return bytearray(arr)

    def createCmdPacket(self, cmdType, paramBytes, cmdWithAnswer=True, readOrWrite="write"):
        ctrlByte = (int(readOrWrite=="write") << 0x07) | (int(cmdWithAnswer) << 0x07)
        data = [ctrlByte, self.cmdTypes[cmdType]]
        data.extend(paramBytes)

        return self.createPacket("cmd", data)

    def createDataPacket(self, data):
        return self.createPacket("data", data)