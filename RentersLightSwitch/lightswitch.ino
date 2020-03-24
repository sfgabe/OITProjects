/* Works with NODEMCU, SG90 servo, and micro PIR sensor */
/* Printable light switch plate here: https://www.thingiverse.com/thing:3289916 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>

// Update these with values suitable for your network.
const char* ssid = "WIFINETWORK";
const char* password = "WIFIPASSWORD";
const char* mqtt_server = "MQTTSERVER.local";
const char* mqtt_username = "MQTT_USERNAME";
const char* mqtt_password = "MQTT_PASSWORD";
const int mqtt_port = 1883;
const int BUFFER_SIZE = 300;
bool stateOn = false;
int pirValue;
int pirStatus;
int stateStatus;
String switchStatus;
String motionStatus;
char message_buff[100];


#define PIN_SERVO  D1 
#define PIN_PIR  D5

const char* on_cmd = "ON";
const char* off_cmd = "OFF";

/**************************** FOR OTA **************************************************/
#define SENSORNAME "lightswitch" //change this to whatever you want to call your device
#define OTApassword "OTA_PASSWORD" //the password you will need to enter to upload remotely via the ArduinoIDE
int OTAport = 8266;

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* switch_state_topic = "lightswitch/current";
const char* switch_set_topic = "lightswitch/set";

Servo myservo;  // create servo object to control a servo
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Command from MQTT broker : [");
  Serial.print(topic);
  Serial.print(" Message is:");
  for(int i=0;i<length;i++)
  {
    if((int)payload[i]>194||(int)payload[i]<0)
    break;
    myservo.write((int)payload[i]);
    delay(1000);
    myservo.write(65);// tell servo to go to neutral
    Serial.print((int)payload[i]);//print the rotation angle for debug
    Serial.print("]");
    Serial.println();
  
    if (payload[0] == 51 && stateStatus != 1) {
      switchStatus = "OFF";
      sendState();
      stateStatus = 1;
    }

    if (payload[0] == 80 && stateStatus != 2) {
      switchStatus = "ON";
      sendState();
      stateStatus = 2;
    }
  }
}//end callback

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "lightswitch-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(SENSORNAME, mqtt_username, mqtt_password))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe(switch_set_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

/* JSON */
bool processJson(char* message) {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }

  if (root.containsKey("state")) {
    if (strcmp(root["state"], on_cmd) == 0) {
      stateOn = true;
    }
    else if (strcmp(root["state"], off_cmd) == 0) {
      stateOn = false;
    }
  }
}

/* SEND STATE */
void sendState() {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.createObject();

  root["state"] = (String)switchStatus;
  root["motion"] = (String)motionStatus;

  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));

  Serial.println(buffer);
  client.publish(switch_state_topic, buffer, true);
}

/* CHECK SENSOR */
bool checkBoundSensor(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PIR, INPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  myservo.attach(PIN_SERVO);  // attaches the servo on pin D1 to the servo object

/* OTA SETUP */
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

/* WIFI SETUP */
  Serial.println("Ready");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  ArduinoOTA.handle();
    //PIR CODE
    pirValue = digitalRead(PIN_PIR); //read state of the

/* PIR STATUS */
    if (pirValue == LOW && pirStatus != 1) {
      motionStatus = "standby";
      sendState();
      pirStatus = 1;
    }

    else if (pirValue == HIGH && pirStatus != 2) {
      motionStatus = "motion detected";
      sendState();
      pirStatus = 2;
    }
}
