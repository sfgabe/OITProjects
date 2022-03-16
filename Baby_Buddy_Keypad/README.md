## Quick Entry Keypad for BabyBuddy and Home Assistant with ESPHome ##
 
I set up [Home Assistant](https://www.home-assistant.io/) to integrate with [BabyBuddy](https://github.com/babybuddy/babybuddy) with the [Baby Buddy Home Assistant Integration](https://github.com/jcgoette/baby_buddy_homeassistant). I made a quick little keypad for adding data to baby buddy through a NodeMCU with [ESPHome](https://esphome.io/).

Its a simple 8 switch keypad. In my use case its used for 3 different lighting scenes (bright, medium, nightlight) and four baby buddy data points (solid diaper, wet diaper, start sleep, end sleep).

<img src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_1.jpg" data-canonical-src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_1.jpg" width="400" height="371" /> 
<img src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_2.jpg" data-canonical-src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_2.jpg" width="400" height="400" /> 
<img src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_3.jpg" data-canonical-src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_3.jpg" width="400" height="400" /> 
<img src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_4.jpg" data-canonical-src="https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/babybuddy_keypad_4.jpg" width="400" height="279" /> 

#### Parts: ####
- 8 [Key Switches](https://amzn.to/3i8RcX6)
- 8 [Key Caps](https://amzn.to/3KHNKyS)
- a [nodeMCU / ESP8266 board](https://amzn.to/2JWhQF0)
- a [HomeAssistant installation](https://www.home-assistant.io/) with an ESPHome integration
- some kind of enclosure - I just made a little wooden box with a table top CNC but theres some printable options on thingiverse too
 
#### Assembly: ####

![Keypad Fritzing](https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/keypad-esphome.png)

#### Programming: ####
- You'll need to be comfortable with [ESPHome](https://esphome.io/) and loading that code onto your Esp8266. The [included ESPHome code](https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/esphome_keypad.yaml) should work out of the box with just your wifi information. 
- Add the [Baby Buddy HA Integration](https://github.com/jcgoette/baby_buddy_homeassistant) if you haven't already. 
- Add the Home Assistant YAML code to your HA configs. You'll need to add to [automations.yaml](https://github.com/sfgabe/OITProjects/blob/master/Baby_Buddy_Keypad/automations.yaml)
- You'll need to figure out your baby's entity ID number/name from HA for the automations to log correctly, and decide what notes will be helpful, and change the YAML as needed. You can figure this out by making a test service call from the HA developer section and see what the YAML shows you for entity_id
- Restart Home Assistant and you should be able to log data right away. 
