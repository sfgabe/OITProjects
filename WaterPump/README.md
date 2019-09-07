# Waterpump

This is a local server connected pump and water level sensor for topping off an aquarium at regular intervals (ATO). The float switch acts as a sensor - I've have unreliable results as float sensors get old so I want to have more control over whether a top off is needed. Also for this reason, the pump will shut off after 30 seconds, as a guarantee against server disconnection or accidental presses while the pump is on. You can adjust that timing in the code as needed.

## Parts list:
- **[NodeMCU](https://amzn.to/2UdY5cX)** with the [waterpump.ino](https://github.com/sfgabe/OITProjects/blob/master/WaterPump/waterpump.ino) loaded onto it, test by going to the ip address and making sure it's working as a server before hooking everything up
- **[12V mini water pump](https://amzn.to/2MJ3fws)** - this one is narrow and fits nicely in a 5 gallon water cooler bottle
- **[vertical switch float](https://amzn.to/2Zu8xhv)**
- **[2 LEDs](https://amzn.to/2PQMou5)**
- **[mini buck converter](https://amzn.to/2PQR9E4)** - this is not needed if you don't mind two separate plugs, one for the pump and one for the nodemcu
- **[12v power supply and barrel connector](https://amzn.to/2MX6NLP)**
- If you don't have one you'll need a [multimeter](https://amzn.to/2A0HnVe) to adjust the buck converter. You'll want the pump to be getting 12v and the NodeMCU board to be getting 5v-9v (it has on onboard regulator as long as you are using the VIN pin)
- Some kind of enclosure, the linked-to parts fit snugly in [this 3d printed box](https://www.thingiverse.com/thing:3581474) but any project box will do.

![WaterPump Fritzing Diagram](https://github.com/sfgabe/OITProjects/blob/master/WaterPump/waterpump.png?raw=true)
