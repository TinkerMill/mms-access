#!/use/bin/python
#
# fakeGateway.py
# This script fakes out a gateway for testing the arduino code.

### IMPORTS ###
import serial

### GLOBALS ###
serialPort = '/dev/tty.usbserial-A5027KD1'
inputBuffer = ""

### CLASSES ###

### FUNCTIONS ###

### MAIN ###
def main():
	# Open the serial port
	ser = serial.Serial( serialPort, 9600, timeout = 0)
	# Loop indefinitely until a CTRL+C
	while( True):
		# Append the serial input to an input buffer
		#   Requests are 16 bytes in size, so try to read 16 bytes
		inputBuffer.append( ser.read(16))
		# Check if there's a complete request
		# If the request is good, reply with the proper sequence
		#   If the first test card is swiped, reply with an unlockDoorAccess
		#   If the second test card is swiped, replay with an unlockDoorPersistent, then a lockDoor after a delay


if __name__ == '__main__':
	main()


ser = serial.Serial(, 9600, timeout=0)
>>> s = ser.read(100)
>>> s = ser.read(100)
>>> s
'\x026A0049F63EEB\r\n\x03'
>>> s = ser.read(100)
>>> s
'\x026A0049DFB844\r\n\x03'