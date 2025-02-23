/*
  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

  The circuit:
	- +V connection of the PING))) attached to +5V
	- GND connection of the PING))) attached to ground
	- SIG connection of the PING))) attached to digital pin 7

  created 3 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Ping
*/

// this constant won't change. It's the pin number of the sensor's output:
#include <Wire.h>
#include <Servo.h>

int ledPin = 13;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
const int pingPin = 6;
long length = 0;
long lasttime = 0;


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  Wire.begin(0x8);
  
  //Wire.onReceive(receiveEvent);
  Wire.onRequest(onRequest);
  
//  Wire.begin(0x9);
 // Wire.onRequest(onRequest2);

  myservo.attach(7);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  tone(8, 120 , 50 + (length * 3));
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    length = sensortest();
    //Serial.print(length);
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  tone(8, 120, 50 + (length * 3));
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    length = sensortest();
    delay(15);
                         // waits 15 ms for the servo to reach the position
  }

  
}
// Function that executes whenever data is received from master
// void receiveEvent(int howMany) {
//   while (Wire.available()) { // loop through all but the last
//     char c = Wire.read(); // receive byte as a character
//     digitalWrite(ledPin, c);
//   }
// }
// Function that executes whenever data is requested from master
void onRequest(){
  Wire.write(length);
  Wire.write(pos);
  //Wire.write(10);

 // Wire.beginTransmission(0x8);
 // Wire.write(256);
  
}
long sensortest() {
  
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH, 10000);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // Serial.print(inches);
  // Serial.print("in, ");
  // Serial.print(cm);
  // Serial.print("cm");
  // Serial.println();

  //delay(100);

  return cm;
}

void buzzer(){
  // put your main code here, to run repeatedly:
  
  if (millis() > lasttime +100){
    if(millis() > lasttime + (5000 - ((100-length)*50))){
    int range =1000;
    tone(8, 120 + ((100 - length)), 50);
  
    lasttime = millis();
    Serial.println(millis());
    /*
  while (millis() > millis() + 1000){
    tone(8, 120, 50);
*/
    }
  }
  
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

