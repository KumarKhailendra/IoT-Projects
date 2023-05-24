//----------------------------------------Include the NodeMCU ESP8266 Library
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_BMP085.h>
//----------------------------------------
MDNSResponder mdns;
//----------------------------------------

#include "PageIndex.h" //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

#define LEDonBoard 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router

//----------------------------------------SSID and Password of your WiFi router
const char* ssid = "Redmi 7s";
const char* password = "atstaea83h";
//----------------------------------------
int pin_d0 = 16;
int pin_d3 = 0;
int pin_d4 = 13;
int pin_d5 = 14;
int pin_d6 = 12;
//----------------------------------------
ESP8266WebServer server(80);  //--> Server on port 80

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;
//----------------------------------------Variabals for the pulse sensor
//----------------------------------------
unsigned long previousMillisGetHR = 0; //--> will store the last time Millis (to get Heartbeat) was updated.
unsigned long previousMillisHR = 0; //--> will store the last time Millis (to get BPM) was updated.

const long intervalGetHR = 10; //--> Interval for reading heart rate (Heartbeat) = 10ms.
const long intervalHR = 10000; //--> Interval for obtaining the BPM value based on the sample is 10 seconds.

const int LED_D1 = D1; //--> LED to detect when the heart is beating. The LED is connected to PIN D1 (GPIO5) on the NodeMCU ESP12E.
int Threshold = 600; //--> Determine which Signal to "count as a beat" and which to ignore.

int cntHB = 0; //--> Variable for counting the number of heartbeats.
boolean ThresholdStat = true; //--> Variable for triggers in calculating heartbeats.
int BPMval = 0; //--> Variable to hold the result of heartbeats calculation.

//----------------------------------------
//----------------------------------------Procedure for get Analog value for AD8232 sensor
int AD8232(){
  digitalWrite(pin_d0, HIGH);
  digitalWrite(pin_d5, LOW);
  digitalWrite(pin_d6, LOW);
  return analogRead(A0);
}
//----------------------------------------
//----------------------------------------Procedure for get Analog value for Lm35 sensor
int Lm35(){
  digitalWrite(pin_d0, LOW);
  digitalWrite(pin_d5, HIGH);
  digitalWrite(pin_d6, LOW);
  return analogRead(A0);
}
//----------------------------------------
//----------------------------------------Procedure for get Analog value for PULSE sensor
int PULSE(){
  digitalWrite(pin_d0, LOW);
  digitalWrite(pin_d5, LOW);
  digitalWrite(pin_d6, HIGH);
  return analogRead(A0);
}
//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //--> Read HTML contents
 server.send(200, "text/html", s); //--> Send web page
}
//----------------------------------------
//----------------------------------------Procedure for reading the ECG value of a AD8232 sensor
void readECG(){
  int ecg = AD8232();
  String ecgS = String(ecg);
  
  if((digitalRead(pin_d4) == 1)||(digitalRead(pin_d3) == 1)){
    Serial.println('!');
  }
  else{
    // send the value of analog input 0:
    Serial.println(ecgS);
    server.send(200, "text/plane", ecgS); //--> Send Ecg data value only to client ajax request
  }
}

//----------------------------------------
//----------------------------------------Procedure for reading the ECG value of a LM35 sensor
void readLM35(){
  float val = Lm35();
  float temp = (val/1024.0)*5000;
  float cel = temp/10;
//  float frah = (cel*9)/5 +32;
  String celS = String(cel);
  Serial.print("Tempreture C = ");
  Serial.println(celS);
  server.send(200, "text/plane", celS); //--> Send LM35 value only to client ajax request
  
}
//--------------------------------------------------------------------------------void readPULSE()
// This subroutine is for reading the heart rate and calculating it to get the BPM value.
// To get a BPM value based on a heart rate reading for 10 seconds.
void readPULSE() {
  //----------------------------------------Process of reading heart rate.
  unsigned long currentMillisGetHR = millis();

  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;

    int PulseSensorHRVal = PULSE();

    if (PulseSensorHRVal > Threshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
      digitalWrite(LED_D1,HIGH);
    }

    if (PulseSensorHRVal < Threshold) {
      ThresholdStat = true;
      digitalWrite(LED_D1,LOW);
    }
  }
  //----------------------------------------

  //----------------------------------------The process for getting the BPM value.
  unsigned long currentMillisHR = millis();

  if (currentMillisHR - previousMillisHR >= intervalHR) {
    previousMillisHR = currentMillisHR;

    BPMval = cntHB * 6; //--> The taken heart rate is for 10 seconds. So to get the BPM value, the total heart rate in 10 seconds x 6.
    Serial.print("BPM : ");
    Serial.println(BPMval);
    String BPMvalS = String(BPMval);
    server.send(200, "text/plane", BPMvalS); //--> Send LM35 value only to client ajax request
    cntHB = 0;
  }
  //----------------------------------------
}
//--------------------------------------------------------------------------------
//----------------------------------------
//----------------------------------------Procedure for reading of BMP180 sensor
void readBMP180() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F;
  float altitude = bmp.readAltitude(101500);
  String BMPvalS = String(temperature)+" "+String(pressure)+" "+String(altitude);
  Serial.println(BMPvalS);
  server.send(200, "text/plane", BMPvalS); //--> Send BMP180 value only to client ajax request
}

//----------------------------------------
//----------------------------------------Setup
void setup(void){
  pinMode(pin_d3, INPUT); // Setup for leads off detection LO +
  pinMode(pin_d4, INPUT); // Setup for leads off detection LO -
  pinMode(pin_d0, OUTPUT);
  pinMode(pin_d5, OUTPUT);
  pinMode(pin_d6, OUTPUT);
  Serial.begin(9600);
  delay(500);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(LEDonBoard,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(LEDonBoard, HIGH); //--> Turn off Led On Board
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(LEDonBoard, LOW);
    delay(250);
    digitalWrite(LEDonBoard, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(LEDonBoard, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  //----------------------------------------If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (!MDNS.begin("KPECG")) {
    Serial.println("Error setting up MDNS responder!");
  }
  //----------------------------------------
  server.on("/", handleRoot); //--> Routine to handle at root location. This is to display web page.
  server.on("/readECG", readECG);  //--> Routine to handle the call procedure ECG
  server.on("/readLM35", readLM35);  //--> Routine to handle the call procedure LM35
  server.on("/readPULSE", readPULSE);  //--> Routine to handle the call procedure PULSE
  server.on("/readBMP180", readBMP180);  //--> Routine to handle the call procedure BMP180

  server.begin(); //--> Start server
  MDNS.addService("http", "tcp", 80);
  Serial.println("HTTP server started");
}
//----------------------------------------
//----------------------------------------Loop
void loop(){
  MDNS.update();
  server.handleClient();  //Handle client requests
}
//----------------------------------------
