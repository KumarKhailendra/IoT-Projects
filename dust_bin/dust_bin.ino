#include <Servo.h>

Servo myservo; 
//int pos;
void setup() {
  // put your setup code here, to run once:
  pinMode(16, OUTPUT);
  pinMode(5, INPUT);
  pinMode(4, OUTPUT);
  pinMode(2, INPUT);
  myservo.attach(13);
  myservo.write(0);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(16, LOW);
  delayMicroseconds(2);
  digitalWrite(16, HIGH);
  delayMicroseconds(10);
  digitalWrite(16, LOW);

  long T1 = pulseIn(5, HIGH);
  long dis1 = 0.01715 * T1; 
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);

  long T2 = pulseIn(2, HIGH);
  long dis2 = 0.01715 * T2; 
  if(dis1 > 20){
      myservo.write(0);
  }else{
      myservo.write(180); 
  }
    
  Serial.println(dis1);
  Serial.println(dis2);
  delay(1000);
}
