void setup() {
  for(int i = 13; i > 10; i--){
    pinMode(i, OUTPUT);
  }
  pinMode(9, INPUT);
  pinMode(10, INPUT);
}
void backword(){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH);
}
void forword(){
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
}
void right(){
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, HIGH);
}
void left(){
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(11, LOW);
}
void stopNow(){
  for(int i = 13; i > 10; i--){
    digitalWrite(i, LOW);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(10) == 0 && digitalRead(9) == 0){
    stopNow();
    backword();
    delay(500);
    right();
    delay(750);
  }
  if(digitalRead(10) == 1 && digitalRead(9) == 0){
    stopNow();
    backword();
    delay(500);
    right();
    delay(500);
  }
  if(digitalRead(10) == 0 && digitalRead(9) == 1){
    stopNow();
    backword();
    delay(500);
    left();
    delay(500);
  }
  if(digitalRead(10) == 1 && digitalRead(9) == 1){
    forword();
  }

}
