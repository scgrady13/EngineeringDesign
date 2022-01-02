#include <Servo.h>

/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int j = 200;
int value;
void setup() {
  myservo.attach(8);  // attaches the servo on pin 8 to the servo object
  Serial.begin(9600);
  myservo.write(60);
  delay(2000);
 myservo.write(120);
}

void loop() {

 value = analogRead(A10);
Serial.println("Photoresistor Value :");
Serial.println(value);
delay(250);
  if(value >= 600){
    j = 250;
  }else{
    j=500;
  }
  for (pos = 0; pos <= 180; pos += 15) { // goes from 0 degrees to 180 degrees
    // in steps of 15 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(j);
    
  }
   value = analogRead(A10);
Serial.println("Photoresistor Value :");
Serial.println(value);
delay(250);
  if(value >= 600){
    j = 250;
  }else{
    j=500;
  }
  for (pos = 180; pos >= 0; pos -= 15) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(j);                       // waits 15ms for the servo to reach the position
  }
}
