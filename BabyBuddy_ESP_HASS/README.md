## Bottle Scale for BabyBuddy and Home Assistant with ESPHome ##

I set up [Home Assistant](https://www.home-assistant.io/) to integrate with [BabyBuddy]() with the [Baby Buddy Home Assistant Integration](https://github.com/jcgoette/baby_buddy_homeassistant). I wanted a way to measure milk / formula consumption without using the app all the time, so I built a small kitchen scale with ESPHome to log the difference between a full and empty bottle.

#### Parts: ####
- 1 [Digital Load Cell Weight Sensor HX711 AD Converter Breakout Module](https://amzn.to/3mdvzYm)
- 2 [Push buttons](https://amzn.to/3GfdGAu)
- 3 [LEDs](https://amzn.to/3785W3Y)
- a [nodeMCU / ESP8266 board](https://amzn.to/2JWhQF0)
- a [HomeAssistant installation](https://www.home-assistant.io/) with an ESPHome integration
- some kind of enclosure - I just made a little wooden box with some CNC carved labels
 
#### Assembly: ####

![Bottlescale Fritzing](https://github.com/sfgabe/OITProjects/blob/master/BabyBuddy_ESP_HASS/bottlescale-esphome.png)
