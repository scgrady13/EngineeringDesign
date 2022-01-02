#include <Servo.h>
#include <math.h>
int res1 = A10;
int res2 = A9;
int res3 = A8;
int res4 = A7;
int res5 = A6;
int resVals[] = {res1, res2, res3, res4, res5};
int resPins[] = {res1, res2, res3, res4, res5};
Servo baseServo;
Servo panelServo;
int firMax;
int firMaxRes;
int secMax;
int secMaxRes;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  baseServo.attach(8);
  panelServo.attach(9);
}
void loop()
{
  // put your main code here, to run repeatedly:

  Serial.println();
  StatsCreator(firMax, firMaxRes, secMax, secMaxRes);
  SetServo();

  delay(10000);
}

void ReadResistor()
{
  for (int resVal : resVals)
  {
    resVal = analogRead(resVal);

    Serial.print(resVal);
    Serial.print(" ");
  }
}

void StatsCreator(int& firMax, int& firMaxRes, int& secMax, int& secMaxRes)
{
  firMax = resVals[0];
  firMaxRes = 0;
  secMax = resVals[1];
  secMaxRes = 1;
  for (int i = 0; i < 4; i++) {
    if (resVals[i] > firMax) {
      firMax = resVals[i];
      firMaxRes = i;
    } else if (resVals[i] > secMax) {
      secMax = resVals[i];
      secMaxRes = i;
    }
  }
}

void SetServo()
{
  int bAngle;
  int pAngle;
  bool sunRight = true;
  if ((firMaxRes == 0 && secMaxRes == 1) || (firMaxRes == 1 && secMaxRes == 0)) {

}else if ((firMaxRes == 1 && secMaxRes == 2) || (firMaxRes == 2 && secMaxRes == 1)) {

}else if ((firMaxRes == 2 && secMaxRes == 3) || (firMaxRes == 3 && secMaxRes ==2)) {

}else if ((firMaxRes == 3 && secMaxRes == 0) || (firMaxRes == 0 && secMaxRes == 3)) {

}else{
  Serial.println("Nope");
}
if (sunRight) {
  pAngle = map(resVals[4], 0, 1023, 0, 90);
  } else if(!sunRight) {
  pAngle = map(resVals[4], 0, 1023, 180, 90);
  }

  baseServo.write(bAngle);
  panelServo.write(pAngle);

}
