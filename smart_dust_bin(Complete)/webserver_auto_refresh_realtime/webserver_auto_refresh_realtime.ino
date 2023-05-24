#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo myservo; 

// Replace with your network credentials
const char* ssid = "Redmi 7s"; //Enter Wi-Fi SSID
const char* password =  "atstaea83h"; //Enter Wi-Fi Password

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

String page = "";
String text = "";
double data;
void setup(void) {
  pinMode(16, OUTPUT);
  pinMode(5, INPUT);
  pinMode(4, OUTPUT);
  pinMode(2, INPUT);
  
  delay(1000);
  myservo.attach(13);
  myservo.write(0);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/data.txt", []() {
    text = (String)data +"%";
    server.send(200, "text/html", text);
  });
//  server.on("/data1.txt", []() {
//    text1 = (String)data1 +"cm";
//    server.send(200, "text/html", text1);
//  });
  
  server.on("/", []() {
    page = "<!DOCTYPE html>\r\n";
    page += "<html lang=\"en\">\r\n";
    page += "<head>\r\n";
    page += "<title>ACEIT</title>\r\n";
    page += "<meta charset=\"utf-8\">\r\n";
    page += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n";
    page += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css\">\r\n";
    page += "<link rel=\"icon\" type=\"image/x-icon\" href=\"https://image3.mouthshut.com/images/imagesp/925717652s.jpg\">\r\n";
    page += "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.6.0/jquery.min.js\"></script>\r\n";
    page += "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js\"></script>\r\n";
    page += "</head>\r\n";
    page += "<body>\r\n";

    page += "<div class=\"container\">\r\n";
    page += "<img src=\"https://www.aryacollege.in/assets/img/Logo.svg\" class=\"mt-5 pt-4\" >\r\n";
    page += "<h1 class=\"text-center text-primary\" style=\"margin-top:55px; margin-bottom: 45; font-weight:800;\">Smart Dust Bin And Garbage Monitoring System<h1>\r\n";
    page += "<h2>Dust Bin Status</h2>\r\n";
    page += "<div class=\"progress\">\r\n";
    page += "<div class=\"progress-bar\" id=\"data\" role=\"progressbar\" aria-valuenow=\"70\" aria-valuemin=\"0\" aria-valuemax=\"100\" style=\"0%\">\r\n";
    page += "<span id=\"datashow\"></span>\r\n";
    page += "</div>\r\n";
    page += "</div><hr />\r\n";
    page += "<div class=\"IoT\" style=\"display:flex; flex-wrap: nowrap; justify-content:space-between; align-items: center; align-content:space-around; flex-direction: row;\">\r\n";
    page += "<div class=\"maker\">\r\n";
    page += "<h1>Khailendra Prasad</h1>\r\n";
    page += "<h2>Branch: Electronics And Communication Engineering</h2>\r\n";
    page += "<h2>Roll. No. : 19EAREC066</h2>\r\n";
    page += "</div>\r\n";
    page += "<div class=\"item\">\r\n";
    page += "<h1>Using Appliances</h1>\r\n";
    page += "<pre>1. NodeMCU8266</pre>\r\n";
    page += "<pre>2. Two HC-SR04(ultrasonic sensor)</pre>\r\n";
    page += "<pre>3. Servo Moter</pre>\r\n";
    page += "</div>\r\n";
    page += "</div>\r\n";
    page += "</div>\r\n";
    page += "<script>\r\n";
    page += "var x = setInterval(function() {loadData(\"data.txt\",updateData)}, 1000);\r\n";
    page += "function loadData(url, callback){\r\n";
    page += "var xhttp = new XMLHttpRequest();\r\n";
    page += "xhttp.onreadystatechange = function(){\r\n";
    page += " if(this.readyState == 4 && this.status == 200){\r\n";
    page += " callback.apply(xhttp);\r\n";
    page += " };\r\n";
    page += "};\r\n";
    page += "xhttp.open(\"GET\", url, true);\r\n";
    page += "xhttp.send();\r\n";
    page += "}\r\n";
    page += "function updateData(){\r\n";
    page += " document.getElementById(\"data\").style.width = this.responseText;\r\n";
    page += " document.getElementById(\"datashow\").innerHTML = this.responseText + \"Complete\";\r\n";
    page += "var full = parseFloat(this.responseText);\r\n";
    page += "if(full >= 70){\r\n";
    page += "\tdocument.getElementById(\"data\").classList.add(\"progress-bar-danger\");\r\n";    
    page += "}\r\n";
    page += "else if(full >= 60){\r\n";
    page += "\tdocument.getElementById(\"data\").classList.add(\"progress-bar-warning\");\r\n";
    page += "}\r\n";
    page += "else if(full >= 50){\r\n";
    page += "\tdocument.getElementById(\"data\").classList.add(\"progress-bar-info\");\r\n";
    page += "}\r\n";
    page += "else if(full >= 40){\r\n";
    page += "document.getElementById(\"data\").classList.add(\"progress-bar-success\");\r\n";
    page += "}\r\n";
    page += "}\r\n";
    page += "</script>\r\n";
    page += "</body>\r\n";
    page += "</html>\r\n";
    server.send(200, "text/html", page);
  });

  server.begin();
  Serial.println("Web server started!");
}

void loop(void) {
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
      delay(1000);
      myservo.write(0);
  }else{
      delay(1000);
      myservo.write(180); 
  }
    
  Serial.println(dis1);
  Serial.println(dis2);
  data = ((21 - dis2)*100)/21;
  server.handleClient();
}
