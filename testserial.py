#!/usr/bin/python

import serial
ser = serial.Serial("/dev/ttyACM0", 9600)
print "Serial started"
#while 1 :
print ser.read()
