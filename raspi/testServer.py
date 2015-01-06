#!/usr/bin/env python
#
# mms-card-updater.py
# A desktop based card updater client to allow quick adding or changing of member's RFID badges.

# The initial test version will run in the command line and will just output the card reads to the console.

### IMPORTS ###
import time
import threading
import serial
import yaml
#import wx

### GLOBALS ###
serialPort = "/dev/tty.usbserial-A5027KD1" # /dev/tty* for *nix, COM? for Windows
baudRate = 9600

connected = False
exitFlag = False

bufferArray = []

### CLASSES ###

### FUNCTIONS ###
def handle_data( data):
    # Add the byte to an array and check for a complete packet
    global bufferArray
    if( len( data) > 0):
        bufferArray.append( data)
        check_packet()

def check_packet():
    # Check the global array for a packet
    global bufferArray
    while( len( bufferArray) > 15):
        print( "Buffer Length: %d" % ( len( bufferArray), ))
        # search for the 0x02 char, which is the start of the card
        try:
            startIndex = bufferArray.index( '\x02')
            endIndex = bufferArray.index( '\x03')
            #print( "       Start Index: %d" % ( startIndex, ))
            #print( "       End Index: %d" % ( endIndex, ))
            #print( "       length: %d" % ( endIndex - startIndex, ))
            print( "       value: %s" % ( ''.join( bufferArray[ startIndex + 1: startIndex + 11]), ))
            # Remove the card read from the buffer
            bufferArray = bufferArray[ endIndex + 1: ]
        except:
            pass

def read_from_port( ser):
    global connected
    global exitFlag
    while not connected:
        connected = True
        
        while not exitFlag:
            reading = ser.read() # read a byte
            handle_data( reading)

### MAIN ###
def main():
    global exitFlag
    # Stuff.
    ser = serial.Serial( serialPort, baudRate, timeout = 0)
    thread = threading.Thread( target = read_from_port, args = ( ser, ))
    thread.start()
    
    while not exitFlag:
        try:
            time.sleep( 1)
        except:
            # Usually catching the keyboard interrupt
            exitFlag = True

if __name__ == '__main__':
	main()
