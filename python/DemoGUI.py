import tkinter as tk
import tkinter.font as font
import time
SIZEX = 1200
SIZEY = 600
WEQUAL = 1.0
with open("DPResult.txt", "r") as DPResult :
    Atimes = list(map(float, DPResult.readline().split()))
    Btimes = list(map(float, DPResult.readline().split()))
window = tk.Tk()
BtnFont = font.Font(size=15)
class Car :
    def __init__(self, SizeX, SizeY, canvas, color, PosX, PosY):
        self.car = canvas.create_rectangle(PosX, PosY, PosX + SizeX, PosY + SizeY, fill = color)
        self.speed = 0.05
        self.canvas = canvas
        self.moveing = True
    def GoForward(self) :
        if self.moveing:
            self.canvas.move(self.car, self.speed, 0)
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
        self.CarSizeX = 60
        self.CarSizeY = 40
        self.CarListA = []
        self.CarListB = []
    def create_car(self, AorB) :
        if AorB == 0 :
            self.CarListA.append(Car(self.CarSizeX, self.CarSizeY, self.canvas, "blue", 20, 30))
        elif AorB == 1:
            self.CarListA.append(Car(self.CarSizeX, self.CarSizeY, self.canvas, "yellow", 20, 110))
##
# time setting
sec = 0.0
timer = tk.Label(window, text='0.0', font=('Arial', 18))
timer.pack()
def TimerCount(canvas):
    global sec
    StartTime = time.time()
    CreateI = 1
    CreateJ = 1
    LastITime = 0.0
    LastJTime = 0.0
    sec = time.time() - StartTime
    while sec < 20.0 :
        sec = time.time() - StartTime
        timer.config(text = str(format(sec, '.2f')))
        ## move car
        for car in canvas.CarListA :
            car.GoForward()
        for car in canvas.CarListB :
            car.GoForward()
        ## create car
        if Atimes[CreateI] <= sec:
            canvas.create_car(0)
            LastITime = sec
            CreateI = CreateI + 1
        if Btimes[CreateJ] <= sec:
            canvas.create_car(1)
            LastJTime = sec
            CreateJ = CreateJ + 1
        window.update()
canvas = CanvasControl()
## button start
BtnStart = tk.Button(window, text="Start", bg = "light blue", width = '3', height = '1', font = BtnFont, command = lambda:TimerCount(canvas))
BtnStart.pack()
window.title('DemoGUI')
window.geometry(str(SIZEX) + "x" + str(SIZEY))
window.mainloop()