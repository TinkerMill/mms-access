#!/use/bin/env python
#
# sendDoorLock.py
# This script sends the "lock door persistently" command, unlocking the door for seven seconds.

### IMPORTS ###
import time
import serial

### GLOBALS ###
serialPort = '/dev/ttyAMA0'
inputBuffer = ""

### CLASSES ###

### FUNCTIONS ###
def lockDoor( moduleId = 1):
    # Generate the command to unlock the door for access ( short time)
    command = bytearray(6)
    command[ 0] = chr( moduleId) # ID of Access Module (Arduino)
    command[ 1] = chr( 0x00) # Bad command
    command[ 2] = chr( 0x55) # Checksum (currently unused)
    command[ 3] = chr( 0xAA) # Checksum (currently unused)
    command[ 4] = chr( 0x0D) # Carriage Return
    command[ 5] = chr( 0x0A) # Line Feed
    return command

### MAIN ###
def main():
    # Open the serial port
    ser = serial.Serial( serialPort, 9600, timeout = 0)
    # Send the command
    ser.write( lockDoor())
    #
    print "Sent unlockDoorAccess command."

if __name__ == '__main__':
    main()
