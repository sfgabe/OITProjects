/*********
 * Works with NodeMCU (https://amzn.to/2UdY5cX), 12V mini water pump https://amzn.to/2MJ3fws, and vertical switch float https://amzn.to/2Zu8xhv
 * Note: pump and float work independently, not as a switch 
********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "YourNetwork";
const char* password = "YourPassword";
#define FLOAT_SENSOR  12     // the number of the float sensor
#define FLOAT_LED  14     // the number of the float LED pin
#define PUMP_LED  15     // the number of the pump LED pin
String waterlevel="";

ESP8266WebServer server(80);
String webSite="";
int relay = 13; 

void setup(void){
  
  webSite +="<title>Water Pump</title>\n";
  webSite+="<button onclick='myFunction()'>HOME</button>";
  webSite+="<script>function myFunction() {location.reload();}</script>";
  webSite +="<h1>ESP8266 Web Server</h1><p>Pump #1 <a href=\"pump1on\"><button>ON</button></a>&nbsp;<a href=\"pump1off\"><button>OFF</button></a>&nbsp;<a href=\"checklevel\"><button>CHECK LEVEL</button></a></p>\n";

  // preparing GPIOs
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(PUMP_LED, OUTPUT);
  pinMode(FLOAT_LED, OUTPUT);
  pinMode(FLOAT_SENSOR, INPUT_PULLUP);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(relay, LOW);
  digitalWrite(PUMP_LED, LOW);
  digitalWrite(FLOAT_LED, LOW);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
 if (mdns.begin("waterpump", WiFi.localIP())) {
   Serial.println("MDNS responder started");
  }
    server.on("/", [](){
    server.send(200, "text/html", webSite);
    });
  server.on("/checklevel", [](){
      if(digitalRead(FLOAT_SENSOR) == HIGH) 
        { // Flashes light and give text reponse if its low
          digitalWrite(FLOAT_LED, HIGH);
          waterlevel = "Low";
          server.send(200, "text/html", waterlevel);  
          delay(1000); 
          digitalWrite(FLOAT_LED, LOW); 
          delay(1000); 
          digitalWrite(FLOAT_LED, HIGH); 
          delay(1000); 
          digitalWrite(FLOAT_LED, LOW); 
          delay(1000); 
          digitalWrite(FLOAT_LED, HIGH); 
          delay(1000); 
          digitalWrite(FLOAT_LED, LOW);
         } 
      else 
        {
          digitalWrite(FLOAT_LED, LOW);
          waterlevel = "OK";
          server.send(200, "text/html", waterlevel); 
          digitalWrite(PUMP_LED, HIGH); // Green light for ok
          delay(2500); 
          digitalWrite(PUMP_LED, LOW); 
         }  
  });
  server.on("/pump1on", [](){
    server.send(200, "text/html", webSite);
    digitalWrite(relay, HIGH);
    digitalWrite(PUMP_LED, HIGH); // Light goes on while pumping
    delay(15000); // safety switch - stays on for 15 second before it shuts off
    digitalWrite(relay, LOW);
    digitalWrite(PUMP_LED, LOW);
  });
  server.on("/pump1off", [](){
    server.send(200, "text/html", webSite);
    digitalWrite(relay, LOW);
    digitalWrite(PUMP_LED, LOW);
  });
    
  server.begin();
  Serial.println("HTTP server started");
}

 
void loop(void){
  server.handleClient();
  if(WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(relay, LOW);
    digitalWrite(PUMP_LED, LOW);
    digitalWrite(FLOAT_LED, HIGH); // Both lights flashing means it's disconnected
    Serial.println("");
    Serial.print("Wifi is disconnected!");
    Serial.println("");
    Serial.print("Reconnecting");
    Serial.println("");
    digitalWrite(PUMP_LED, HIGH);
    digitalWrite(FLOAT_LED, LOW);
    //WiFi.begin(ssid,password);
    
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    digitalWrite(relay,LOW);
    digitalWrite(PUMP_LED, LOW);
    digitalWrite(FLOAT_LED, LOW);
  }

} 
