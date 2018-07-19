import serial
import time

ser = serial.Serial()
ser.baudrate = 9600
ser.port = "COM3"
ser.parity = 'N'
ser.bytesize = 8
ser.stopbits = 1
ser.open()
n = 10

while n:
    line = ser.readline()
    print(line)
    n-=1
    time.sleep(1)

ser.close()