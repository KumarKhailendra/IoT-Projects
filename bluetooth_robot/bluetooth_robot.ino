#include<SoftwareSerial.h>
SoftwareSerial BT(4, 5);
bool ok = false;
void setup() {
  for(int i = 13; i > 10; i--){
    pinMode(i, OUTPUT);
  }
  while(ok == false){
    if(BT.available()){
      if(BT.readString() == "1234"){
        ok = true;
      }else{
        Serial.println("Enter First Password");
      }
    }
  }
  Serial.begin(9600);
  BT.begin(9600);
}
void loop() {
  if(BT.available()){
    if(ok){
      
      char x = BT.read();
      Serial.print(x);
      
      if(x == "B"){
        digitalWrite(13, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(11, HIGH);
      }
      if(x == "R"){
        digitalWrite(13, LOW);
        digitalWrite(12, LOW);
        digitalWrite(11, HIGH);
      }
      if(x == "L"){
        digitalWrite(13, LOW);
        digitalWrite(12, HIGH);
        digitalWrite(11, LOW);
      }
      if(x == "F"){
        digitalWrite(13, HIGH);
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
      }
      if(x == "S"){
        for(int i = 13; i > 10; i--){
          digitalWrite(i, LOW);
        }
      }
    }
  }else{
    ok = false;
  }
}
