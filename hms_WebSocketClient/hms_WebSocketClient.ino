//----------------------------------------Include the NodeMCU ESP8266 Library
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Adafruit_BMP085.h>

//----------------------------------------SSID and Password of your WiFi router
const char* ssid = "Redmi 7s";
const char* password = "atstaea83h";
//const char* ssid = "OnePlus Nord2 5G";
//const char* password = "naren@1301";
//----------------------------------------
//---------------------------------------- Variabals
int pin_d0 = 16;
int pin_d3 = 0;
int pin_d4 = 13;
int pin_d5 = 14;
int pin_d6 = 12;
//----------------------------------------Variabals for the pulse sensor
//----------------------------------------
unsigned long previousMillisGetHR = 0; //--> will store the last time Millis (to get Heartbeat) was updated.
unsigned long previousMillisHR = 0; //--> will store the last time Millis (to get BPM) was updated.

const long intervalGetHR = 10; //--> Interval for reading heart rate (Heartbeat) = 10ms.
const long intervalHR = 10000; //--> Interval for obtaining the BPM value based on the sample is 10 seconds.

const int LED_D1 = 1; //--> LED to detect when the heart is beating. The LED is connected to PIN D1 (GPIO5) on the NodeMCU ESP12E.
int Threshold = 250; //--> Determine which Signal to "count as a beat" and which to ignore.

int cntHB = 0; //--> Variable for counting the number of heartbeats.
int BPMvalg = 0;
boolean ThresholdStat = true; //--> Variable for triggers in calculating heartbeats.
int BPMval = 0; //--> Variable to hold the result of heartbeats calculation.

//----------------------------------------

//----------------------------------------

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;
//----------------------------------------

// Server Details
String serverHost = "192.168.43.95";
int serverPort = 8080;
//String serverHost = "selfdoctor.com";
//int serverPort = 80;

// Data Sending Time
unsigned long CurrentMillis, PreviousMillis, DataSendingTime = (unsigned long) 1 * 1;

//ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

bool clientConnect = false;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

	switch(type) {
		case WStype_DISCONNECTED:
//			Serial.printf("[WSc] Disconnected!\n");
      clientConnect = false;
			break;
		case WStype_CONNECTED: {
			Serial.printf("[WSc] Connected to url: %s\n", payload);

			// send message to server when Connected
//			webSocket.sendTXT("Connected");
      clientConnect = true;
		}
			break;
		case WStype_TEXT:
			Serial.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			Serial.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);

			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
        case WStype_PING:
            // pong will be send automatically
            Serial.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            Serial.printf("[WSc] get pong\n");
            break;
    }

}

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
//----------------------------------------
//----------------------------------------Procedure for reading the ECG value of a AD8232 sensor
int readECG(){
  if((digitalRead(pin_d4) == 1)||(digitalRead(pin_d3) == 1)){
    Serial.println('!');
    return 0;
  }
  else{
    // send the value of analog input 0:
    Serial.println(AD8232());
    return AD8232();
  }
}

//----------------------------------------
//----------------------------------------Procedure for reading the ECG value of a LM35 sensor

float readLM35(){
  float val = Lm35();
  float temp = (val/1024.0)*5000;
  float cel = temp/10;
  return cel;
}

//----------------------------------------
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
  unsigned long currentMillisHR = millis();
  
  if (currentMillisHR - previousMillisHR >= intervalHR) {
    previousMillisHR = currentMillisHR;

    BPMval = cntHB * 6; //--> The taken heart rate is for 10 seconds. So to get the BPM value, the total heart rate in 10 seconds x 6.
    
    Serial.print("BPM : ");
    Serial.println(BPMval);
    
    cntHB = 0;
    
    BPMvalg = BPMval;
  } 
  //----------------------------------------
}
//----------------------------------------
//----------------------------------------Setup

void setup() {
  pinMode(pin_d3, INPUT); // Setup for leads off detection LO +
  pinMode(pin_d4, INPUT); // Setup for leads off detection LO -
  pinMode(pin_d0, OUTPUT);
  pinMode(pin_d5, OUTPUT);
  pinMode(pin_d6, OUTPUT);
	Serial.begin(9600);
  bmp.begin();
  delay(500);
  Serial.println("\n\nWelcome to Batch 2 Health Monitoring System Project");
	setup_wifi();

	// server address, port and URL
	webSocket.begin(serverHost, serverPort, "/", "data");

	// event handler
	webSocket.onEvent(webSocketEvent);

	// try ever 5000 again if connection has failed
	webSocket.setReconnectInterval(3000);

}

void loop() {
	webSocket.loop();
  send_data();
}
void send_data(){
  readPULSE();
  CurrentMillis = millis();
 if(CurrentMillis - PreviousMillis > DataSendingTime){
  PreviousMillis = CurrentMillis;
  if(clientConnect){
     DynamicJsonDocument doc(1024);
      doc["deviceKey"] = "SHMS6676";
      doc["ECG"] = String(readECG());
      doc["TEMP"] = String(readLM35());
      doc["PULSE"] = String(BPMvalg);
      doc["bmpP"] = String(bmp.readPressure() / 100.0F);
      doc["bmpT"] = String(bmp.readTemperature());
      doc["bmpSP"] = String(bmp.readSealevelPressure());
      doc["bmpA"] = String(bmp.readAltitude(101500));
      String msg;
      serializeJson(doc, msg);
//      Serial.print("Mesh Broadcast - ");
//      Serial.println(msg);
//    String data = "Random data from esp8266 " + String(random(1,999));
    Serial.println("\n\nSending data: " + msg);
    webSocket.sendTXT(msg.c_str());
  }
 }
}
void setup_wifi(){
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  
}
