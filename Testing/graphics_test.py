from Tkinter import *
import serial
import random
import time

root = Tk()
ser = serial.Serial('COM3', 9600, timeout = 1)
canvas = Canvas(root, width = 500, height = 500)
canvas.pack()

redBox = canvas.create_rectangle(100,100,150,150, fill = "red")
x_speed = 0
y_speed = 0.5

#animation (can be modified)
def moveY(y_axis):
    y = 0;
    if y_axis > -10 and y_axis < 2.62:
       y = -1;
    if y_axis > 3 and y_axis < 10:
        y = 1;
    return y;

while True:
    y_axis = 0
    try:
        y_axis = float(ser.readline())
    except ValueError:
        y_axis = 0;
        print "Not Valid Data"
    print(moveY(y_axis))
    canvas.move(redBox, x_speed, moveY(y_axis))
    
    root.update()
        

      
    
