#include <Servo.h>
#include <math.h>
int phoresVals[] = {A10, A9, A8, A7/*, A8*/};
int photoresPins[] = {A10, A9, A8, A7/*, A8*/};
int stats[3];
Servo baseServo;
Servo panelServo;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  baseServo.attach(8);
  //panelServo.attach();
}
void loop()
{
  // put your main code here, to run repeatedly:
  for (int resVal : phoresVals)
  {
    resVal = ReadResistor(resVal);

    Serial.print(resVal);
    Serial.print(" ");
  }
  StatsCreator();
  SetServo();
  Serial.println();
  delay(10000);
}

int ReadResistor(int anPin)
{
  return analogRead(anPin);
}

void StatsCreator()
{
  int sum = 0;
  int avg = 0;
  int deviation = 0;
  int minRead = 0;
  for (int i = 0; i < 4; i++)
  {
    sum += phoresVals[i];
    if (phoresVals[i] < minRead)
    {
      minRead = phoresVals[i];
      stats[2] = i;
    }
  }
  avg = sum / 4;
  stats[0] = avg;
  for (int resVal : phoresVals)
  {
    deviation += pow((resVal - avg), 2);
  }
  deviation = sqrt(deviation / 4);
  stats[1] = deviation;
}

void SetServo()
{
  if (phoresVals[stats[3]] < (stats[0] - stats[1]))
  {
    if (stats[2] = 0)
    {
      baseServo.write(0);
    }
    else if (stats[2] = 1)
    {
      baseServo.write(45);
    }
    else if (stats[2] = 2)
    {
      baseServo.write(135);
    }
    else if (stats[2] = 3)
    {
      baseServo.write(180);
    }
    else
    {
      baseServo.write(90);
    }
  }
}
