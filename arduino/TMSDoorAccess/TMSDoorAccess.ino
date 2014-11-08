/*
  TMSDoorAccess
  This take a serial input from an RFID reader, send out a verification request, and upon replay, decides to engage the relay (i.e. open the door).
 */

/* ### INCLUDES ### */
#include <SoftwareSerial.h>

/* ### VARIABLES ### */
// The status LED is connected to pin 13
int statusLED = 13;
// The output relay is connected to pin 7
int relay = 7;
// The software serial for verification is connected to pins 2 (RX) & 3 (TX).
int verifyRX = 2;
int verifyTX = 3;
SoftwareSerial mySerial( verifyRX, verifyTX);

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

/* ### INTERRUPT ROUTINES ### */

/* ### MAIN ### */
// the loop routine runs over and over again forever:
void loop() {
  // For initial testing, we'll turn the outputs on and off at different times.
  digitalWrite(statusLED, HIGH);
  delay(1000);
  digitalWrite(relay, HIGH);
  delay(1000);
  digitalWrite(statusLED, LOW);
  delay(1000);
  digitalWrite(relay, LOW);
  delay(1000);
}
