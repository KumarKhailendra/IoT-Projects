#include<SoftwareSerial.h>
SoftwareSerial BT(4, 5);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BT.available()){
    char x = BT.read();
    Serial.print(x);
  }
  if(Serial.available()){
    char y = Serial.read();
    BT.print(y);
  }
}
