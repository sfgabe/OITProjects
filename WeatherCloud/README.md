![WeatherCloud Gif](https://github.com/sfgabe/OITProjects/blob/master/WeatherCloud/weathercloud.gif?raw=true)

# WeatherCloud for Home Assistant

Simple hack for creating a cute weather notification cloud using Home Assistant's Dark Sky Sensor and MQTT

## Items needed:
- [Addressable WS2812b LED Strip](https://amzn.to/2pdQLTb)
- [ESP8266 NodeMCU Board](https://amzn.to/2OI6TXE)
- Cloud Lightbox from Flying Tiger Store [I think it's the same as this one](https://amzn.to/32q3dha)
- [Micro USB cord](https://amzn.to/32bolba)
- HomeAssistant installation with [DarkSky Sensor](https://www.home-assistant.io/integrations/darksky/) and [MQTT](https://www.home-assistant.io/integrations/mqtt)
- You'll need to do a little bit of soldering to connect the lights to the board, so you should have what you need to do that too

## Programming
- Load the weathercloud.ino onto the NodeMCU with Arduino
- Add the attached bits to your light.yaml and automation.yaml files in HomeAssistant

## Wiring
![WeatherCloud Fritzing Diagram](https://github.com/sfgabe/OITProjects/blob/master/WeatherCloud/weathercloud.png?raw=true)
You may not need the extra ground connection, but if you experience flickering or non-lighting LEDs towards the end, this will probably fix it. I used a string of 16 LEDs in 4 sections and wired it as shown.

## Assembling
You'll need to carefully pry open the lightbox with a knife or plastic spudger and take out the existing LEDs which are probably just taped to the back (these are just white and won't work for this project, save them for something else). It may also have a battery connection and a switch, which I just removed. 

Once you have an empty box you can just stick your LEDs and NodeMCU in there and run the USB cord through where the switch used to be. You may have to experiment with placement so you dont get weird shadows from the wiring.

## Alerts
Here's some of the presets, the rest is up to you!

![WeatherCloud Samples](https://github.com/sfgabe/OITProjects/blob/master/WeatherCloud/weathercloud.jpg?raw=true)
