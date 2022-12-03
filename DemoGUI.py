import tkinter as tk
import tkinter.font as font
SIZEX = 1200
SIZEY = 600
window = tk.Tk()
class CanvasControl :
    def __init__(self):
        self.canvas = tk.Canvas(window, width = SIZEX, height = SIZEY)
        self.canvas.pack()
        ## create line of DP x1 = 10~500  y1 = 10~170  
        self.canvas.create_line(10, 10, 500, 10, width = 2)
        self.canvas.create_line(10, 90, 470, 90, width = 2)
        self.canvas.create_line(10, 170, 500, 170, width = 2)
        self.canvas.create_line(500, 10, 580, 50, width = 2)
        self.canvas.create_line(500, 170, 580, 130, width = 2)
        self.canvas.create_line(580, 50, 1200, 50, width = 2)
        self.canvas.create_line(580, 130, 1200, 130, width = 2)
        ## create line of FIFO x1 = 10~500  y1 = 310~470  
        self.dash = 300
        self.canvas.create_line(10, 10 + self.dash, 500, 10 + self.dash, width = 2)
        self.canvas.create_line(10, 90 + self.dash, 470, 90 + self.dash, width = 2)
        self.canvas.create_line(10, 170 + self.dash, 500, 170 + self.dash, width = 2)
        self.canvas.create_line(500, 10 + self.dash, 580, 50 + self.dash, width = 2)
        self.canvas.create_line(500, 170 + self.dash, 580, 130 + self.dash, width = 2)
        self.canvas.create_line(580, 50 + self.dash, 1200, 50 + self.dash, width = 2)
        self.canvas.create_line(580, 130 + self.dash, 1200, 130 + self.dash, width = 2)
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
canvas = CanvasControl()
canvas.create_car(0)
canvas.create_car(0)
canvas.create_car(1)
canvas.create_car(1)
canvas.create_car(1)
window.title('DemoGUI')
window.geometry(str(SIZEX) + "x" + str(SIZEY))
window.mainloop()