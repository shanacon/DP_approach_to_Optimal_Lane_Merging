import tkinter as tk
import tkinter.font as font
import time
import threading
import math
SIZEX = 1200
SIZEY = 600
window = tk.Tk()
BtnFont = font.Font(size=15)
class CanvasControl :
    def __init__(self):
        self.canvas = tk.Canvas(window, width = SIZEX, height = 300)
        self.canvas.pack()
        ##  line setting : x1 = 10~500  y1 = 10~170  
        self.canvas.create_line(10, 10, 500, 10, width = 2)
        self.canvas.create_line(10, 90, 470, 90, width = 2)
        self.canvas.create_line(10, 170, 500, 170, width = 2)
        self.canvas.create_line(500, 10, 580, 50, width = 2)
        self.canvas.create_line(500, 170, 580, 130, width = 2)
        self.canvas.create_line(580, 50, 1200, 50, width = 2)
        self.canvas.create_line(580, 130, 1200, 130, width = 2)
        ## car setting
        self.CarSizeX = 70
        self.CarSizeY = 40
        self.Wequal = 20
        self.CarListA = []
        self.CarListB = []
    def create_car(self, AorB) :
        if AorB == 0 :
            NowSize = len(self.CarListA)
            x1 = 20 + (self.Wequal + self.CarSizeX) * NowSize
            self.CarListA.append(self.canvas.create_rectangle(x1, 30, x1 + self.CarSizeX, 30 + self.CarSizeY, fill = 'blue'))
        elif AorB == 1:
            NowSize = len(self.CarListB)
            x1 = 20 + (self.Wequal + self.CarSizeX) * NowSize
            self.CarListB.append(self.canvas.create_rectangle(x1, 110, x1 + self.CarSizeX, 110 + self.CarSizeY, fill = 'yellow'))
##
# time setting
StartTime = time.time()
sec = 0.0
timer = tk.Label(window, text='0.0', font=('Arial', 18))
timer.pack()
def TimerCount():
    global sec
    sec = time.time() - StartTime
    while sec < 5.0 :
        sec = time.time() - StartTime
        timer.config(text = str(format(sec, '.2f')))
        window.update()
## button start
BtnStart = tk.Button(window, text="Start", bg = "light blue", width = '3', height = '1', font = BtnFont, command= TimerCount)
BtnStart.pack()
canvas = CanvasControl()
canvas.create_car(0)
canvas.create_car(0)
canvas.create_car(1)
canvas.create_car(1)
canvas.create_car(1)
window.title('DemoGUI')
window.geometry(str(SIZEX) + "x" + str(SIZEY))
window.mainloop()