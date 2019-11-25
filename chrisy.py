import serial #install pySerial

ser = serial.Serial('/dev/ttyACM0', 115200, timeout = 0.5)  # open serial port
print(ser.name)         # check which port was really used
ser.write(b'hello')     # write a string
ser.close()             # close port