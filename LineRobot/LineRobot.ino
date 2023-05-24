void setup() {
  // put your setup code here, to run once:
  for(int i = 13; i > 5; i--){
    pinMode(i, OUTPUT);
  }
  pinMode(4, INPUT);
  pinMode(5, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 13; i > 5; i--){
    digitalWrite(i, LOW);
  }
  int left = digitalRead(5);
  int right = digitalRead(4);
  int leftMove = (left == 1) && (right == 0);
  int rightMove = (left == 0) && (right == 1);
  int goStop = (left == 1) && (right == 1);
  int go = (left == 0) && (right == 0);
  if(leftMove == 1){
    digitalWrite(12, LOW);
    digitalWrite(10, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(6, HIGH);
  }else if(rightMove == 1){
    digitalWrite(12, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(6, LOW);
  }else if(goStop == 1){
    digitalWrite(12, LOW);
    digitalWrite(10, LOW);
    digitalWrite(8, LOW);
    digitalWrite(6, LOW);
  }else{
    digitalWrite(12, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(6, HIGH);
  }
  delay(200);
}
