#include <Servo.h>
Servo myServo;
void setup() {
  // put your setup code here, to run once:
myServo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
myServo.write(0);
delay(2000);
myServo.write(180);
delay(2000);
myServo.write(30);
delay(2000);
myServo.write(150);
delay(2000);
myServo.write(60);
delay(2000);
myServo.write(120);
delay(2000);
myServo.write(90);
delay(2000);

}
