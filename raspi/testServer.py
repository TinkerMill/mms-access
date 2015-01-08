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
connected = False
exitFlag = False

config = {}
bufferArray = []

### CLASSES ###

### FUNCTIONS ###
def handle_data( ser, data):
    # Add the byte to an array and check for a complete packet
    global bufferArray
    if( len( data) > 0):
        bufferArray.append( data)
        check_packet( ser)

def check_packet( ser):
    # Check the global array for a packet
    global bufferArray
    while( len( bufferArray) > 15):
        print( "Buffer Length: %d" % ( len( bufferArray), ))
        #print( "       contents: %s" % ( ''.join( bufferArray), ))
        # Assuming the first char is the first char of the read packet
        try:
            startIndex = 0;
        #    endIndex = bufferArray.index( '\x0A')
        #    #print( "       Start Index: %d" % ( startIndex, ))
        #    #print( "       End Index: %d" % ( endIndex, ))
        #    #print( "       length: %d" % ( endIndex - startIndex, ))
        #    #print( "       value: %s" % ( ''.join( bufferArray[ startIndex + 2: startIndex + 12]), ))
            if( check_card( ''.join( bufferArray[ startIndex + 2: startIndex + 12]))):
                # The card is allowed, so send the good command
                print( "Card status: ALLOWED")
                send_access( ser)
            else:
                # The card is unknown, so send the bad command
                print( "Card status: DENIED")
                send_denied( ser)
        except:
            # Something went wrong, so send the bad command
            print( "Card status: ERROR")
            send_denied( ser)
        # Remove the card read from the buffer
        bufferArray = []

def check_card( value):
    # check the card value against the list of proper cards
    global config
    print( "Checking card: %s" % ( value, ))
    # if in list, return true
    if( value in config[ 'cards']):
        return True
    return False

def send_access( ser):
    # Generate a hardcoded command for the prototype
    command = bytearray(6)
    command[ 0] = chr( 0x01) # ID of Access Module (Arduino)
    command[ 1] = chr( 0x01) # Bad command
    command[ 2] = chr( 0x55) # Checksum (currently unused)
    command[ 3] = chr( 0xAA) # Checksum (currently unused)
    command[ 4] = chr( 0x0D) # Carriage Return
    command[ 5] = chr( 0x0A) # Line Feed
    # Send the command
    ser.write( command)

def send_denied( ser):
    # Generate a hardcoded command for the prototype
    command = bytearray(6)
    command[ 0] = chr( 0x01) # ID of Access Module (Arduino)
    command[ 1] = chr( 0x00) # Bad command
    command[ 2] = chr( 0x55) # Checksum (currently unused)
    command[ 3] = chr( 0xAA) # Checksum (currently unused)
    command[ 4] = chr( 0x0D) # Carriage Return
    command[ 5] = chr( 0x0A) # Line Feed
    # Send the command
    ser.write( command)

def read_from_port( ser):
    global connected
    global exitFlag
    while not connected:
        connected = True
        
        while not exitFlag:
            reading = ser.read() # read a byte
            # Pass the serial port along so we can send packets out of it.
            handle_data( ser, reading)

### MAIN ###
def main():
    global config
    global exitFlag
    
    # Load the config
    config = yaml.load( file( 'config.yaml', 'r'))
    # Check for values (serialPort, baudRate, cards)
    
    # Run the fun
    ser = serial.Serial( config[ 'serialPort'], config[ 'baudRate'], timeout = 0)
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
