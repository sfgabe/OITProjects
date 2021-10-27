## Bottle Scale for BabyBuddy and Home Assistant with ESPHome ##

I set up [Home Assistant](https://www.home-assistant.io/) to integrate with [BabyBuddy](https://github.com/babybuddy/babybuddy) with the [Baby Buddy Home Assistant Integration](https://github.com/jcgoette/baby_buddy_homeassistant). I wanted a way to measure formula consumption without using the app all the time, so I built a small kitchen scale with [ESPHome](https://esphome.io/) to log the difference between a full and empty bottle.

![Bottlescale Pic 2](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/20211027_003419.jpg)
![Bottlescale Pic 1](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/20211027_003453.jpg)

#### How it works: ####
- While preparing a bottle, put the full bottle on the scale for a few seconds. The white LED will light up when it has read a significant change in weight.
- Press the "Full" button to record the weight in Home Assistant. The yellow LED will light for a moment.
- Feed the baby! 
- When baby is done, put the empty bottle on the scale for a few seconds. The white LED will light again.
- Press the "Empty" button to record the weight in Home Assistant. The green LED will light for a moment.
- After a minute, Baby Buddy will record the new feeding, with start and end time, and amount consumed. The time delay in the automation is to prevent multiple recordings as you are juggling bottles and babies.

![Bottlescale screenshot](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/Screenshot_20211027-003738_Chrome.png)

#### Parts: ####
- 1 [Digital Load Cell Weight Sensor HX711 AD Converter Breakout Module](https://amzn.to/3mdvzYm)
- 2 [Push buttons](https://amzn.to/3GfdGAu)
- 3 [LEDs](https://amzn.to/3785W3Y)
- a [nodeMCU / ESP8266 board](https://amzn.to/2JWhQF0)
- a [HomeAssistant installation](https://www.home-assistant.io/) with an ESPHome integration
- some kind of enclosure - I just made a little wooden box with some CNC labels
 
#### Assembly: ####

![Bottlescale Fritzing](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/bottlescale-esphome.png)

#### Programming: ####
- You'll need to be comfortable with [ESPHome](https://esphome.io/) and loading that code onto your Esp8266. The [included ESPHome code](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/esphome_bottlescale.yaml) should work out of the box with just your wifi information. You will also want to calibrate the scale to your setup by using some known measurements. Refer to the esphome HX711 docs for how to do that.
- Add the [Baby Buddy HA Integration](https://github.com/jcgoette/baby_buddy_homeassistant) if you haven't already. 
- Add the Home Assistant YAML code to your HA configs. You'll need to add to add to [sensors.yaml](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/sensors.yaml), [automations.yaml](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/automations.yaml), and [input_number.yaml](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/input_number.yaml)
- You'll need to figure out your baby's entity ID number/name for the automations to log correctly, and decide what notes will be helpful, and change the YAML as needed. 
- Restart Home Assistant and you should be able to log auto log bottle measurements right away. 
