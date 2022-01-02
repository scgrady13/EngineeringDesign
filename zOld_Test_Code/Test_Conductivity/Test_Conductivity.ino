#include <conductivity.h>
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  int conductivityReading = getConductivity();
  /*int salinity = myCalibrationFunction(conductivityReading);*/
  Serial.println("Conductivity Reading:");
  Serial.println(conductivityReading/*salinity*/);
}
