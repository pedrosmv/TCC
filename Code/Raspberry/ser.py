import serial

ser = serial.Serial("/dev/ttyACM0", 9600)
s = [0]

file = open("sensordata.txt", "w")
i = 0

while i<4:
	read_serial = ser.readline()
	file.write(read_serial)
	i+=1


