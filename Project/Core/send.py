import serial
import struct
import time

arduino = serial.Serial("/dev/ttyACM0", 9600)

file = open("angulos", "r")

for line in file:
    arduino.write(struct.pack('>B', line))
    sleep(4)
