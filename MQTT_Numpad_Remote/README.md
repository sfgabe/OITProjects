## Remote for Home Assistant with MQTT ##

Home Assistant is great, but I found sometimes I just wanted a physical button on hand to toggle lights and things on and off, so I built a 3x3 matrix mechanical keyboard that sends messages via MQTT to Home Assistant. There are 9 buttons, which can be set to do anything you can do through Home Assistant switches or scripts.

#### Parts: ####
- 9 [mechanical cherry mx style key switches](https://amzn.to/3764lM3)
- 9 [diodes for switches & wires](https://amzn.to/3gAZWUv)
- 9 [LEDs](https://amzn.to/3785W3Y) (optional)
- a [nodeMCU / ESP8266 board](https://amzn.to/2JWhQF0)
- a [HomeAssistant installation](https://www.home-assistant.io/) with an MQTT server enabled
- printed or CNC cut grid and enclosure - basic printed enclosure and handwire mechanical keyboard grid [from these files on Thingiverse](https://www.thingiverse.com/thing:4222157)

#### Assembly: ####
You can follow the basic handwiring guide for the [Void9](https://victorlucachi.ro/journal/void9-wiring-guide/) here. Wire the switches and diodes according to the tutorial _(up to step 7)_.

You can also add the LEDs through the slots in the switches and wire them in parallel with a resistor on the ground wire. There arent enough pins to control them individually, so the whole matrix lights on each keypress.

When it comes to wiring to the board, instead of using an Arduino ProMicro, you'll be using the NodeMCU / ESP8266.

![3x3_keygrid](https://github.com/sfgabe/OITProjects/blob/master/MQTT_Numpad_Remote/3x3_keygrid.jpg)
![3x3_dIodebend](https://github.com/sfgabe/OITProjects/blob/master/MQTT_Numpad_Remote/3x3_dIodebend.jpg)
![3x3wires](https://github.com/sfgabe/OITProjects/blob/master/MQTT_Numpad_Remote/3x3wires.jpg)
