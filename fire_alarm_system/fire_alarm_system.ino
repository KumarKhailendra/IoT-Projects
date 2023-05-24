float sensorValue;
float sensor_volt;
float RS_gas;
const int limited_ppm = 10;
const int buzzer = 13;
void setup() {
  pinMode(buzzer, OUTPUT); 
  Serial.begin(9600); 
}

void loop() {
  sensorValue = analogRead(A0);
  sensor_volt = sensorValue/1024*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt;

  Serial.print("Sensor value in ppm is: ");
  Serial.println(sensorValue);

  if(RS_gas >= limited_ppm){
    tone(buzzer, 1000); // Send 1KHz sound signal...
    Serial.println("Buzzer is on");
  }
  if(RS_gas < limited_ppm){
    noTone(buzzer); // Stop sound 
    Serial.println("Buzzer is off");
  }
}
