# MQTT Notification Board

This is an MQTT notification board for use with Home Assistant. I use it to monitor my aquarium stats from Home Assistant and Reef-Pi. It can be motion activated with a PIR sensor or triggered to display a light if there is an important change (error, etc).

![](https://github.com/sfgabe/OITProjects/blob/master/NotifyLCD_with_MQTT/InShot_20210227_024037147.jpg)

**Parts**
- [NodeMCU](https://amzn.to/3dRN0tK)
- [2004a LCD with SDA/SCL backpack board](https://amzn.to/3dLF3Xc)
- [LED](https://amzn.to/3bKENox)
- [220 Ohm resistor](https://amzn.to/3r0BGzo)
- [Mini PIR Sensor](https://amzn.to/2ZV0lJH)
- [Dupont wires](https://amzn.to/3q0LNCQ)
- Printed stand (optional), [available on Thingiverse](https://www.thingiverse.com/thing:4329402) and some hot glue to affix sensor and light

![Fritz Notify Board](https://github.com/sfgabe/OITProjects/blob/master/NotifyLCD_with_MQTT/mqtt-notify-board_bb.png)

**Steps**

- Modify the NotifyLCD.ino file with your MQTT details in Arduino and upload to the NodeMCU.
- Wire up according to the Fritzing diagram above. You can make it neat by soldering the LED and resistor connection but you could also chain them with two of the dupont wires for a solderless build. Hot glue helps to make everything tidy as well.
- Add the code as specified into your Home Assistant config for binary_sensors.yaml, switches.yaml, and automations.yaml .
- Restart Home Assistant.
- Assemble into the stand or wherever you are putting it. You may need to adjust the potentiometer on the backpack board to get the best character display brightness.
- Enjoy the wealth of information at the wave of your fingertips!

![](https://github.com/sfgabe/OITProjects/blob/master/NotifyLCD_with_MQTT/20210222_231437.jpg)
