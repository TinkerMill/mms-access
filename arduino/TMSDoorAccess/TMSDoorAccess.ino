/*
  TMSDoorAccess
  This take a serial input from an RFID reader, send out a verification request, and upon replay, decides to engage the relay (i.e. open the door).
 */

/***
  * The following are example scans of some RFID cards with the device.  The plan is to push these codes to the server for verification.
  * '\x026A0049DFB844\r\n\x03' ( There are non-printable ascii characters.  These were gathered via python.)
  * '\x026A0049F63EEB\r\n\x03'
  * The '\x02...\r\n\x03' should be stripped from the data before sending to the server.
  * The data will thue be 10 ascii chars of data and 2 ascii chars of checksum ( 12 chars total).
  * The format for sending the RFID swipe request to the gateway ( raspberry pi) is as follows:
  *   byte[ 0] - ID of the access module ( arduino)
  *   byte[ 1] - Request type ( 0x01 for RFID swipe)
  *   byte[ 2] - Data byte[ 0]
  *   byte[ 3] - Data byte[ 1]
  *   byte[ 4] - Data byte[ 2]
  *   byte[ 5] - Data byte[ 3]
  *   byte[ 6] - Data byte[ 4]
  *   byte[ 7] - Data byte[ 5]
  *   byte[ 8] - Data byte[ 6]
  *   byte[ 9] - Data byte[ 7]
  *   byte[ 10] - Data byte[ 8]
  *   byte[ 11] - Data byte[ 9]
  *   byte[ 12] - Checksum byte[ 0] ( from the reader)
  *   byte[ 13] - Checksum byte[ 1] ( from the reader)
  *   byte[ 14] - \r ( 0x0D)
  *   byte[ 15] - \n ( 0x0A)
  * The possible responses will be as follows ( NOTE: These are decoupled from the requests, therefore they can be sent at any time.  This will allow situations such as keeping the door unlocked for an extended period such as during an open house.):
  *   byte[ 0] - ID of the access module ( arduino)
  *   byte[ 1] - Response type:
  *     0x01 - Unlock door access ( short duration, i.e. badge in)
  *     0x02 - Unlock door persistent ( undefined duration, i.e. unlock door for an open house)
  *     0x03 - Lock door ( re-lock the door, i.e. after an open house)
  *   byte[ 2] - Checksum byte[ 0]
  *   byte[ 3] - Checksum byte[ 1]
  *   byte[ 4] - \r ( 0x0D)
  *   byte[ 5] - \n ( 0x0A)
 ***/

/* ### INCLUDES ### */
#include <SoftwareSerial.h>

/* ### VARIABLES ### */
// The status LED is connected to pin 13
int statusLED = 13;
// The Green and Red LEDs on the RFID mote
int greenRFIDStatusLED = 4;
int redRFIDStatusLED = 5;
// The output relay is connected to pin 7
int relay = 7;
// The software serial for verification is connected to pins 2 (RX) & 3 (TX).
int verifyRX = 2;
int verifyTX = 3;
SoftwareSerial mySerial( verifyRX, verifyTX);
// Input buffer from RFID reader ( currently 16 bytes)
int currentCharRFID = 0;
char inputRFID[ 16];
// Input buffer from gateway ( currently 4 bytes)
int currentCharGateway = 0;
char inputGateway[ 4];
// ID of the access module ( arduino)
// THIS MUST BE UNIQUE ON THE RS-485 BUS ( which hasn't been deployed yet, so leave as 0x01)
char myID = 0x01;

/* ### SETUP ### */
// the setup routine runs once when you press reset:
void setup() {                
  // Setup the output pins
  pinMode( statusLED, OUTPUT);
  pinMode( relay, OUTPUT);
  // Setup the serial ports
  Serial.begin( 9600);
  mySerial.begin( 9600);
  // A bit of the ol' debug info
  Serial.println( "Goodnight moon!");
  Serial.println( "TinkerMill Member Management System (MMS)");
  Serial.println( "Door Access - Arduino Firmware v0.0.1");
}

/* ### HELPER FUNCTIONS ### */
boolean checkInputRFIDBuffer() {
  // Run through a series of checks to see if the buffer is valid.  These are from the RFID reader datasheet.
  if( inputRFID[ 0] == 0x02) {
    if( inputRFID[ 13] == 0x0D) {
      if( inputRFID[ 14] == 0x0A) {
        if( inputRFID[ 15] == 0x03) {
          return( true);
        }
      }
    }
  }
  return( false);
}

boolean checkInputGatewayBuffer() {
  // Run through a series of checks to see if the buffer is valid.
  if( inputGateway[ 0] == myID) {
    if( inputGateway[ 4] == 0x0D) {
      if( inputGateway[ 5] == 0x0A) {
        // Should do a checksum verification here
        return( true);
      }
    }
  }
  return( false);
}

void sendRFIDswipeRequest() {
  // Send the request for the RFID swipe event.  Do this by writing each byte to the Gateway serial port.
  //   byte[ 0] - ID of the access module ( arduino)
  //   byte[ 1] - Request type ( 0x01 for RFID swipe)
  //   byte[ 2] - Data byte[ 0] ( inputRFID[ 1])
  //   byte[ 3] - Data byte[ 1] ( inputRFID[ 2])
  //   byte[ 4] - Data byte[ 2] ( inputRFID[ 3])
  //   byte[ 5] - Data byte[ 3] ( inputRFID[ 4])
  //   byte[ 6] - Data byte[ 4] ( inputRFID[ 5])
  //   byte[ 7] - Data byte[ 5] ( inputRFID[ 6])
  //   byte[ 8] - Data byte[ 6] ( inputRFID[ 7])
  //   byte[ 9] - Data byte[ 7] ( inputRFID[ 8])
  //   byte[ 10] - Data byte[ 8] ( inputRFID[ 9])
  //   byte[ 11] - Data byte[ 9] ( inputRFID[ 10])
  //   byte[ 12] - Checksum byte[ 0] ( from the reader) ( inputRFID[ 11])
  //   byte[ 13] - Checksum byte[ 1] ( from the reader) ( inputRFID[ 12])
  //   byte[ 14] - \r ( 0x0D)
  //   byte[ 15] - \n ( 0x0A)
  mySerial.write( myID);
  mySerial.write( inputRFID[ 1]);
  mySerial.write( inputRFID[ 2]);
  mySerial.write( inputRFID[ 3]);
  mySerial.write( inputRFID[ 4]);
  mySerial.write( inputRFID[ 5]);
  mySerial.write( inputRFID[ 6]);
  mySerial.write( inputRFID[ 7]);
  mySerial.write( inputRFID[ 8]);
  mySerial.write( inputRFID[ 9]);
  mySerial.write( inputRFID[ 10]);
  mySerial.write( inputRFID[ 11]);
  mySerial.write( inputRFID[ 12]);
  mySerial.write( 0x0D);
  mySerial.write( 0x0A);
}

void unlockDoorAccess() {
  // Engage the unlock relay and Green Status LED for 7 seconds.
  unlockDoor();
  delay( 7000); // 7 seconds should be enough to get the door open.
  lockDoor();
}

void unlockDoor() {
  // Engage the unlock relay and Green Status LED indefinitely.
  digitalWrite( greenRFIDStatusLED, HIGH);
  digitalWrite( relay, HIGH);
}

void lockDoor() {
  // Disengage the unlock relay and Green Status LED indefinitely.
  digitalWrite( greenRFIDStatusLED, LOW);
  digitalWrite( relay, LOW);
}

void displayBadRequest() {
  // Flash the Red Status LED 3 times in 1 second.
  digitalWrite( redRFIDStatusLED, HIGH);
  delay( 1000 / 6);
  digitalWrite( redRFIDStatusLED, LOW);
  delay( 1000 / 6);
  digitalWrite( redRFIDStatusLED, HIGH);
  delay( 1000 / 6);
  digitalWrite( redRFIDStatusLED, LOW);
  delay( 1000 / 6);
  digitalWrite( redRFIDStatusLED, HIGH);
  delay( 1000 / 6);
  digitalWrite( redRFIDStatusLED, LOW);
}

void inputRFIDClearBuffer() {
  // Set all of the chars in the buffer to 0x00
  for( currentCharRFID = 15; currentCharRFID <= 0; currentCharRFID--) {
    inputRFID[ currentCharRFID] = 0x00;
  }
}

void inputGatewayClearBuffer() {
  // Set all of the chars in the buffer to 0x00
  for( currentCharGateway = 4; currentCharGateway <= 0; currentCharGateway--) {
    inputGateway[ currentCharGateway] = 0x00;
  }
}

/* ### INTERRUPT ROUTINES ### */

/* ### MAIN ### */
// the loop routine runs over and over again forever:
void loop() {
  // Currently, for testing and to get the system operational quickly, I'm going to code this using a simple polling methodology.
  // Check for an incoming character from the RFID reader.
  if( Serial.available()) {
    // Save the byte to the RFID input buffer.
    inputRFID[ currentCharRFID] = Serial.read();
    currentCharRFID++;
  }
  // Check for an incoming character form the gateway.
  if( mySerial.available()) {
    // Save the byte to the RFID input buffer.
    inputGateway[ currentCharGateway] = mySerial.read();
    currentCharGateway++;
  }
  // Check to see if the RFID input buffer is full or the last char was a newline.
  if( currentCharRFID > 15 || inputRFID[ currentCharRFID] == 0x0A) {
    // Turn on the yellow status LED to show that a request is being processed
    digitalWrite( statusLED, HIGH);
    // Check the buffer to make sure it's valid
    if( checkInputRFIDBuffer()) {
      // Send the request for an RFID swipe
      sendRFIDswipeRequest();
    }
    // Clear the buffer
    inputRFIDClearBuffer();
    // Turn off the yellow status LED to show that a request has been processed
    digitalWrite( statusLED, LOW);
  }
  // Check to see if the gateway input buffer is full or the last char was a newline.
  if( currentCharGateway > 4 || inputGateway[ currentCharGateway] == 0x0A) {
    // Turn on the yellow status LED to show that a request is being processed
    digitalWrite( statusLED, HIGH);
    // Check the buffer to make sure it's valid
    if( checkInputGatewayBuffer()) {
      // Perform the actions of the response
      switch( inputGateway[ 2]) {
        case 0x01: {
          // Unlock door access (short term)
          unlockDoorAccess();
          break;
        }
        case 0x02: {
          // Unlock door persistently
          unlockDoor();
          break;
        }
        case 0x03: {
          // Lock door
          lockDoor();
          break;
        }
        default: {
          // Bad command, do nothing.
          break;
        }
      }
    }
    // Clear the buffer
    inputGatewayClearBuffer();
    // Turn off the yellow status LED to show that a request has been processed
    digitalWrite( statusLED, LOW);
  }
}
