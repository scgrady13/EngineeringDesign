void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  double value = analogRead(A7);  
  Serial.println("Analog Value :");
Serial.println(value);  
  value = (value/1023.0)*5.0;
  value*=1.51;
  Serial.println("Voltage Value :");
Serial.println(value);  
delay(1000);
}
