![Flux Capacitor](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/Images/67d1cc4485dca4827851127ebf528743_preview_featured.jpg)

## Options
There are two main scripts - .ino and .py - both are constructed with basically the same circuit

- **flux_capacitor.py** is a python script that can be used by a Raspberry Pi with a trigger of your choosing. It looks cool as a RetroPi enclosure with a pi Zero, can be activated with startup or the push of a button.  Initially, I set it up a RetroPie for some classic video games, but didn't want to take up shelf space.
- **flux_capacitor.ino** is an Arduino script that uses MQTT to control the light, great for fun notifications via Home Assistant, etc.

Video here: https://youtu.be/O8Gv1gEYWtU

## Programming
### On NodeMCU, Wemos D1 Mini, or another kind of ESP8266
- Mostly using the existing FastLED library, with the additions of a "Flux" option.

### With ESP8266 and HomeAssistant
- Use as [an MQTT Light](https://www.home-assistant.io/integrations/light.mqtt/)

### On Raspberry Pi

![Pi Zero Fritzing](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/Images/40b634577ea5f081fd2842161f3c59b6_preview_featured.jpg)

- Everything you need to know about [RetroPie is here](https://retropie.org.uk/). I don't suggest using a PiZero for the more advanced games, but for 80s NES or Atari it works fine with no lag.
- There are lots of ways to setup buttons, I used [this one](https://github.com/LoveBootCaptain/ButtonPi) and set the script up as a service.
- For the sleep / wake I'm using [MultiSwitch for RetroPie](https://retropie.org.uk/forum/topic/17415/multi-switch-shutdown-script)

![Pi Zero Construction](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/Images/d2b5ca33bd970f64a6301fa75ae2eb22_preview_featured.jpg)

## Construction

The 3d printed case uses a neopixel string as a mini Flux Capacitor from Back to the Future. STLs are here or on Thingiverse: https://www.thingiverse.com/thing:3562949

There's an opening on the bottom for the HDMI port, usb controller, or power wires. On the top is space for [two momentary push buttons](https://amzn.to/2UY3dog) and [a power LED](https://amzn.to/2PbP4Pc). On the Pi, I have the buttons mapped to a sleep/wake script and a script to toggle between the flux lights and some other patterns. On the ESP8266, I just have a status LED since it's controlled externally.

The lights are made from a sliced and diced [neopixel string](https://amzn.to/2Xf2NYd). To get four of them lined up I had to cut and rewire horizontally in the right pattern. It will be easier to skip the extra wiring and use [a condensed neopixel strip](https://amzn.to/38Ey1xP), but it's what I had around.

### Electronics
- [RaspberryPi Zero W](https://amzn.to/2IhKa2l) _or_ [NodeMCU](https://amzn.to/2OZJRuT) _or_ [WemosD1 Mini](https://amzn.to/2P5WLr6)
- [neopixel string](https://amzn.to/2Xf2NYd)
- [two momentary push buttons](https://amzn.to/2UY3dog) - _optional_
- [a power LED](https://amzn.to/2PbP4Pc) - _optional_
- To fit a Pi Zero you'll need [right angle header pins](https://amzn.to/2GhRV4L)
- and some headers and [jumper cables](https://amzn.to/2DfQHX4) to put it all together
- If you're using this for RetroPie you'll need a smaller HDMI adapter, as the one that comes in the CanaKit is too big - [this one works](https://amzn.to/2UTvE6X)

### Printed Parts
This is a remix of the scale model flux capacitor. Any parts that aren't here can just be scaled up from [SconNL's Design](https://www.thingiverse.com/thing:3497663) by 250%.

- [Enclosure](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/STL/flux-cap-enclosure.stl)
- [Inside Plate](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/STL/flux-cap-inside-plate.stl)
- [Knobs](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/STL/flux-cap-knobs-print-3.stl)
- [Cover.stl](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/STL/Cover_250-percent.stl) [from the original design](https://www.thingiverse.com/thing:3497663/files), needs to be scaled up to 250%
- [Rubber.stl](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/STL/Rubber_250-percent.stl) [from the original design](https://www.thingiverse.com/thing:3497663/files), needs to be scaled up to 250%
- Either scaled up versions of the original knobs and hinges or few snips of yellow wires, paper clips, and some large red heat shrink to go over the caps for the knobby effect. I like the look of the wires and heat shrink better

### Other stuff
- [Hot glue sticks](https://amzn.to/2SHnNaJ) - I tried a few ideas for the light diffusers, and the thing that looked best were small sized hot glue sticks, cut to size and wedged into the back plate.
- Some hot glue to keep the lights in place - they are stuck to a small piece of cardboard, but that will need to be glue to the back plate so they are lined up correctly.
- Small screws to secure the board in place
- Clear plastic for the window, I just cut it our from a document protector 
- Printed ["Shield eyes from light" sticker](https://github.com/sfgabe/OITProjects/blob/master/FluxCapacitor/STL/flux.pdf) *or make your own with the correct spelling!*
- Bits of red and yellow heat shrink add a nice touch to the connector knobs
