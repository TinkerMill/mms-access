#!/use/bin/python
#
# fakeGateway.py
# This script fakes out a gateway for testing the arduino code.

### IMPORTS ###
import time
import serial

### GLOBALS ###
serialPort = '/dev/tty.usbserial-A5027KD1'
inputBuffer = ""

### CLASSES ###

### FUNCTIONS ###
def unlockDoorAccess():
	# Send the command to unlock the door for access ( short time)
	pass

def unlockDoorPesistent():
	# Send the command to unlock the door for persistent
	pass

def lockDoor():
	# Send the command to lock the door
	pass

### MAIN ###
def main():
	# Open the serial port
	ser = serial.Serial( serialPort, 9600, timeout = 0)
	# Loop indefinitely until a CTRL+C
	while( True):
		# Append the serial input to an input buffer
		#   Requests are 16 bytes in size, so try to read 16 bytes
		inputBuffer.append( ser.read(16))
		# Check if there's a complete request.  This will probably require some validation.
		# If the request is good, reply with the proper sequence
		#   If the first test card is swiped, reply with an unlockDoorAccess
		#   If the second test card is swiped, replay with an unlockDoorPersistent, then a lockDoor after a delay
		# Sleep for a second
		time.sleep(1)



if __name__ == '__main__':
	main()
