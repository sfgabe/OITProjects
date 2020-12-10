/**** works on NODEMCU  with handwired 3x3 matrix and single line matrix LEDs ****/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#include <Keypad.h>

//Update these with values suitable for your network.
const char* wifi_ssid = "Wifi_SSID"; //type your WIFI information inside the quotes
const char* wifi_password = "Wifi_Password";
const char* mqtt_server = "mqtt_server";
const char* mqtt_username = "mqtt_username";
const char* mqtt_password = "mqtt_password";
const int mqtt_port = 1883;

/**************************** FOR OTA **************************************************/
#define SENSORNAME "numpad" //change this to whatever you want to call your device
#define OTApassword "simple" //the password you will need to enter to upload remotely via the ArduinoIDE
int OTAport = 8266;

/************* MQTT TOPICS (change these topics as you wish)  **************************/
#define mqtt_state "numpad/reading/current"
#define mqtt_command "numpad/reading/set"
const char* MQTT_LIGHT_STATE_TOPIC = "numpad/light/current";
const char* MQTT_LIGHT_COMMAND_TOPIC = "numpad/light/set";

#define cmnd_on "On"
#define cmnd_off "Off"
#define state_on "Online"
#define state_off "Offline"
const char* LIGHT_ON = "J";
const char* LIGHT_OFF = "K";

const PROGMEM uint8_t LED_PIN = 2;
boolean m_light_state = false; // light is turned off by default

const byte n_rows = 3;
const byte n_cols = 3;

char keys[n_rows][n_cols] = {
  {'A', 'B', 'C'},
  {'D', 'E', 'F'},
  {'G', 'H', 'I'}
};

byte colPins[n_rows] = {14, 12, 13};
byte rowPins[n_cols] = {5, 4, 0};

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);

long lastMsg = 0;
char msg[50];

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  // init the led
  analogWriteRange(255);
  setLightState();

     //OTA SETUP
  ArduinoOTA.setPort(OTAport);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(SENSORNAME);

  // No authentication by default
  ArduinoOTA.setPassword((const char *)OTApassword);

  ArduinoOTA.onStart([]() {
    Serial.println("Starting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

}

// function called to publish the state of the light (on/off)
void publishLightState() {
  if (m_light_state) {
    client.publish(MQTT_LIGHT_STATE_TOPIC, LIGHT_ON, true);
  } else {
    client.publish(MQTT_LIGHT_STATE_TOPIC, LIGHT_OFF, true);
  }
}

// function called to turn on/off the light
void setLightState() {
  if (m_light_state) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("INFO: Turn light on...");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("INFO: Turn light off...");
  }
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker is : ");
  Serial.print(topic);
  char p = payload[0];
  long now = millis();
  int status;
  char myKey = myKeypad.getKey();

  Serial.print(p);
  if (strcmp(topic,MQTT_LIGHT_COMMAND_TOPIC)==0) {
  // test if the payload is equal to "ON" or "OFF"
  if (p == 'J') {
    if (m_light_state != true) {
      m_light_state = true;
      setLightState();
      publishLightState();
    }
  } else if (p == 'K') {
    if (m_light_state != false) {
      m_light_state = false;
      setLightState();
      publishLightState();
    }
  }
}
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();
  long now = millis();
  int status;
  char myKey = myKeypad.getKey();

  if (myKey != NULL) {
       String msg="Button status: ";
       if(myKey=='A' )
       {
          msg= "A";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
         }
       else if (myKey=='B' )
       {
        msg= "B";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='C' )
       {
        msg= "C";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='D' )
       {
        msg= "D";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='E' )
       {
        msg= "E";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='F' )
       {
        msg= "F";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='G' )
       {
        msg= "G";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='H' )
       {
        msg= "H";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
       else if (myKey=='I' )
       {
        msg= "I";
         char message[58];
         msg.toCharArray(message,58);
         digitalWrite(LED_PIN, HIGH);
         delay(300);
         digitalWrite(LED_PIN, LOW);
         Serial.println(message);
        client.publish(mqtt_state, message);
       }
     }
  else if (myKey != NULL){
  client.setCallback(callback);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "MQTTNumpad-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_command);
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC);
      client.publish(mqtt_state, state_on);
      publishLightState();
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
