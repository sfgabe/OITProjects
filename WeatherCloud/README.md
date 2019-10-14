# WeatherCloud for Home Assistant

Simple hack for creating a cute weather notification cloud using Home Assistant's Dark Sky Sensor and MQTT

Items needed:
- [Addressable WS2812b LED Strip](https://amzn.to/2pdQLTb)
- [ESP8266 NodeMCU Board](https://amzn.to/2OI6TXE)
- Cloud Lightbox from Flying Tiger Store [I think it's the same as this one](https://amzn.to/32q3dha)
- [Micro USB cord](https://amzn.to/32bolba)

## Wiring
![WeatherCloud Fritzing Diagram](https://github.com/sfgabe/OITProjects/blob/master/WeatherCloud/weathercloud.png?raw=true)
You may not need the extra ground connection, but if you experience flickering or non-lighting LEDs towards the end, this will probably fox it. I used a string of 16 LEDs in 4 sections and wired it as shown.
