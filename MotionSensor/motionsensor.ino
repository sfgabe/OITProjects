/*  This version contains deep sleep power saving features, turning off ESP between detects
    It will NOT wake unless PIR detects motion.
    Deep sleep wiring diagram is from rgrokett's similar project: https://github.com/rgrokett/ESP8266_PIRv2/blob/master/ESP8266_PIRv2.pdf
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";
const char* mqtt_server = "MQTT_SERVER";
const char* mqtt_username = "MQTT_USERNAME";
const char* mqtt_password = "MQTT_PASSWORD";
const int mqtt_port = 1883;

int LED = 0;          // built-in LED
int MOTION_DELAY = 15; // Delay in seconds between events to keep from flooding mqtt stream

/************* MQTT TOPICS (change these topics as you wish)  **************************/
const char* config_state_topic = "homeassistant/binary_sensor/motion_battery/config";
const char* switch_state_topic = "homeassistant/binary_sensor/motion_battery/state";
#define SENSORNAME "motion_battery" //change this to whatever you want to call your device

WiFiClient espClient;
PubSubClient client(espClient);

// Blink the LED
void blink() {
  digitalWrite(LED, LOW);
  delay(100);
  digitalWrite(LED, HIGH);
  delay(100);
}

/********************************** START SEND STATE*****************************************/
void sendState() {
  Serial.println("MOTION DETECTED");
  client.publish(config_state_topic, "{'name': 'motion_sensor', 'device_class': 'motion', 'state_topic': 'homeassistant/binary_sensor/motion_battery/state'}", true);
  delay(100);
  client.publish(switch_state_topic, "ON", true);
  delay(100);
  client.publish(config_state_topic, "", true);

  // Keep from flooding MQTT stream
  delay(MOTION_DELAY * 1000);
}


// CONNECT TO WIFI AND SEND MESSAGE
void setup() {
  Serial.begin(115200);
  delay(100);

  // prepare onboard LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

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

  client.setServer(mqtt_server, mqtt_port);

  // Blink onboard LED to signify its connected
  blink();
  blink();
  blink();
  
  // CONNECT TO MQTT
  if (!client.connected()) {
    reconnect();
  }

  // Send a Message to IFTTT
  sendState();

  // Sleep until PIR motion detection
  // See diagram for PIR wiring
  ESP.deepSleep(0);

}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "motiona_sensor-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    //please change following line to    if (client.connect(clientId,userName,passWord))
    if (client.connect(SENSORNAME, mqtt_username, mqtt_password))
    {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end sendMQTT()


// Empty LOOP never executes
void loop() {

}
