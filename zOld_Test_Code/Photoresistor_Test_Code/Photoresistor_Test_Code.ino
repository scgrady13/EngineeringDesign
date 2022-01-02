void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value;
  value = analogRead(A10); 
   
Serial.print(value);
Serial.print(" ");
  value = analogRead(A11);  

Serial.print(value);
Serial.print(" ");
  value = analogRead(A12);  

Serial.print(value);
Serial.print(" ");
  value = analogRead(A13);  

Serial.print(value);
Serial.print(" ");
  value = analogRead(A14);  

Serial.println(value);
delay(5000);
}
