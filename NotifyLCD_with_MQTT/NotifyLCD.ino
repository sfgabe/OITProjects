// Sends and recieves MQTT messages to be displayed on a 2004A 4 line LCD screen
//
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h> 
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // 0x27 is the address of the LCD display (20 characters, 4 lines)
                                    // LiquidCrystal_I2C lcd (0x27,20,4);

const char * ssid = "WIFIName";
const char * password = "WIFIPassword";
const char* mqtt_server = "mqtt-server-name.local";
const char* mqtt_username = "MQTTUsername";
const char* mqtt_password = "MQTTPassword";

WiFiClient espClient;
PubSubClient client (espClient);
long lastMsg = 0;
char msg [50];
int value = 0;

#define D4 2 // Define D4 for display of the Buildin LED
const byte LED_GPIO = D7;
const byte PIR_GPIO = D5; 
int PIR_VALUE = LOW;
int lastPirValue = 0; 

void setup_wifi () {
   delay (100);
   pinMode (D6, OUTPUT);
   digitalWrite (D6, HIGH);
   pinMode (PIR_GPIO, INPUT);
   pinMode (LED_GPIO, OUTPUT);
   digitalWrite (LED_GPIO, LOW);
   
   Serial.print ("Connecting to");
   Serial.println (ssid);
   WiFi.begin (ssid, password);
   while (WiFi.status() != WL_CONNECTED)
    {
      delay (500);
      Serial.print (".");
    }
   randomSeed (micros ());
   Serial.println ("");
   Serial.println ("WiFi connected");
   Serial.println ("IP address:");
   Serial.println (WiFi.localIP ());
}

void callback (char * topic, byte * payload, unsigned int length) {
String sTopic = String (topic);

  if (sTopic == "notifyscreen/line1") {
  Serial.print ("Command from MQTT broker is: "); // for debug
  Serial.print (topic);                           // for debug
  Serial.println ();                              // for debug
  Serial.print ("publish data is:");              // for debug
  lcd.display();
  {
  lcd.setCursor (0, 0); // sets the cursor to the first position in the display line 1 (0), first character (segment)
  lcd.print (F ("")); // delete line 1 (0) (20 characters), with "blank"
  lcd.setCursor (0, 0);
  }
  {
  for (int i = 0; i <length; i ++)
  {
    Serial.print ((char) payload [i]);
    lcd.setCursor (i, 0);
    lcd.write((char)payload[i]);
  }
}

}
  else if (sTopic == "notifyscreen/line2") {
    Serial.print ("Command from MQTT broker is: "); // for debug
    Serial.print (topic);                           // for debug
    Serial.println ();                              // for debug
    Serial.print ("publish data is:");              // for debug
    lcd.display();
    {
    lcd.setCursor (0, 1);
    lcd.print (F (""));
    lcd.setCursor (0, 1);
  }
{
    for (int i = 0; i <length; i ++)
    {
      Serial.print ((char) payload [i]);
     lcd.setCursor (i, 1);
      lcd.write ((char) payload [i]);
      }
  }

}
else if (sTopic == "notifyscreen/line3") {
  Serial.print ("Command from MQTT broker is: [");  // for debug
  Serial.print (topic);                             // for debug
  Serial.println ();                                // for debug
  Serial.print ("publish data is:");                // for debug
  lcd.display();
  {
  lcd.setCursor (0, 2);
  lcd.print (F (""));
  lcd.setCursor (0, 2);
}
  {
  for (int i = 0; i <length; i ++)
  {
    Serial.print ((char) payload [i]);
   lcd.setCursor (i, 2);
    lcd.write ((char) payload [i]);
    }
}

}
else if (sTopic == "notifyscreen/line4") {
  Serial.print ("Command from MQTT broker is: [");  // for debug
  Serial.print (topic);                             // for debug
  Serial.println ();                                // for debug
  Serial.print ("publish data is:");                // for debug
  lcd.display();
  {
  lcd.setCursor (0, 3);
  lcd.print (F (""));
  lcd.setCursor (0, 3);
}
  {
  for (int i = 0; i <length; i ++)
  {
    Serial.print ((char) payload [i]);
   lcd.setCursor (i, 3);
    lcd.write ((char) payload [i]);
    }
}
}
else if (sTopic == "notifyscreen/backlight") {
  payload[length] = '\0';
  String message = (char*)payload;
  Serial.print ("Command from MQTT broker is: ");  // for debug
  Serial.print (topic);                            // for debug
  Serial.print (". Publish data is: ");            // for debug
  Serial.println (message);                        // for debug
    if (message == "1") {
      lcd.display(); 
      lcd.backlight(); }
    else if (message == "0") {
      lcd.clear()
      delay(500);
      lcd.noBacklight();
      delay(500);
      lcd.noDisplay();}
    else {
      lcd.noBacklight();
      delay(500);
      lcd.noDisplay();}
}

else if (sTopic = "notifyscreen/errorlight") {
  payload[length] = '\0'; 
  String message = (char*)payload;
  Serial.print ("Command from MQTT broker is: ");  // for debug
  Serial.print (topic);                            // for debug
  Serial.print (". Publish data is: ");            // for debug
  Serial.println (message);                        // for debug
    if (message == "2")
      digitalWrite(LED_GPIO, HIGH);
    else
      digitalWrite(LED_GPIO, LOW);
}

  Serial.println ();
}

void reconnect () {
  while (! client.connected ())
  {
    Serial.print ("Attempting MQTT connection ...");  // for debug
    String clientId = "notifyscreen-";
    clientId += String(random(0xffff), HEX);

      if (client.connect(mqtt_server, mqtt_username, mqtt_password))
      {
      Serial.println ("connected");

      client.subscribe ("notifyscreen/line1");
      client.subscribe ("notifyscreen/line2");
      client.subscribe ("notifyscreen/line3");
      client.subscribe ("notifyscreen/line4");
      client.subscribe ("notifyscreen/backlight");
      client.subscribe ("notifyscreen/errorlight");
      client.publish ("notifyscreen/wake", "true");
      client.publish ("notifyscreen/motion", "false");
      digitalWrite (D4, LOW); // Switch on LED D1 MINI = low
    } else {
      Serial.print ("failed, rc =");
      Serial.print (client.state ());
      Serial.println ("try again in 5 seconds");

      delay (5000);
    }
  }
}

void setup () {
  pinMode (D4, OUTPUT);
  Serial.begin (9600);
  setup_wifi ();
  client.setServer (mqtt_server, 1883);
  lcd.init (); 
  lcd.backlight ();
  lcd.noBacklight (); 
}

void loop () {
  if (! client.connected ()) {
    reconnect ();
  }
  client.setCallback (callback);
  client.loop ();

    int PIR_VALUE = digitalRead(PIR_GPIO);
    if (PIR_VALUE != lastPirValue) {
      if (PIR_VALUE == HIGH) {
      String message = "MOTION";
      client.publish("notifyscreen/motion", message.c_str());
      }
      else {
      String message = "NO MOTION";
      client.publish("notifyscreen/motion", message.c_str());
      }
    Serial.println(PIR_VALUE); // for debug
    lastPirValue = PIR_VALUE;

    delay(5 * 1000);
  }
}
