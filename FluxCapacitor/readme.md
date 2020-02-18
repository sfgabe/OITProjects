## Options

There are two main scripts - .ino and .py - both are constrcuted with basically the same circuit

- 'flux_capacitor.py' is a python script that can be used by a Raspberry Pi with a trigger of your choosing. It looks cool as a RetroPi enclosure with a pi Zero, can be activated with startup or the push of a button.  Initially, I set it up a RetroPie for some classic video games, but didn't want to take up shelf space.
- 'flux_capacitor.ino' is an Arduino script that uses MQTT to control the light, great for fun notifications via Home Assistant, etc.

Video here: https://youtu.be/O8Gv1gEYWtU

## Construction

The 3d printed case uses a neopixel string as a mini Flux Capacitor from Back to the Future. STLs are here or on Thingiverse: https://www.thingiverse.com/thing:3562949

There's an opening on the bottom for the HDMI port and usb controller, etc.On the top are [two momentary push buttons](https://amzn.to/2UY3dog) and [a power LED](https://amzn.to/2PbP4Pc). On the Pi, I have the buttons mapped to a sleep/wake script and a script to toggle between the flux lights and some other patterns. 

The lights are made from a sliced and diced [neopixel string](https://amzn.to/2Xf2NYd). To get four of them lined up I had to cut and rewire horizontally in the right pattern.  - there's probably easier ways to do this but that's up to you.

## Electronics
- [RaspberryPi Zero W](https://amzn.to/2IhKa2l)
- [two momentary push buttons](https://amzn.to/2UY3dog)
- [neopixel string](https://amzn.to/2Xf2NYd)
- [a power LED](https://amzn.to/2PbP4Pc)
- [right angle header pins](https://amzn.to/2GhRV4L), if you are using [jumper cables](https://amzn.to/2DfQHX4) to put it all together
- If you're using this for RetroPie you'll need a smaller HDMI adapter, as the one that comes in the CanaKit is too big - [this one works](https://amzn.to/2UTvE6X)

## Printed Parts
This is a remix of the scale model flux capacitor. Any parts that aren't here can just be scaled up from [SconNL's Design](https://www.thingiverse.com/thing:3497663) by 250%.

- Enclosure
- Inside Plate
- Knobs
- Cover.stl [from the original design](https://www.thingiverse.com/thing:3497663/files), needs to be scaled up to 250%
- Rubber.stl [from the original design](https://www.thingiverse.com/thing:3497663/files), needs to be scaled up to 250%
- Either scaled up versions of the original knobs and hinges or few snips of yellow wires, paper clips, and some large red heat shrink to go over the caps for the knobby effect. I like the look of the wires and heat shrink better

## Other stuff
- Hot glue sticks - I tried a few ideas for the light diffusers, and the thing that looked best were small sized hot glue sticks, cut to size and wedged into the back plate.
- Some hot glue to keep the lights in place - they are stuck to a small piece of cardboard, but that will need to be glue to the back plate so they are lined up correctly.
- Small screws to secure the Pi in place

## Software
###On Raspberry Pi
- Everything you need to know about [RetroPie is here](https://retropie.org.uk/). I don't suggest using a PiZero for the more advanced games, but for 80s NES or Atari it works fine with no lag.
- There are lots of ways to setup buttons, I used [this one](https://github.com/LoveBootCaptain/ButtonPi) and set the script up as a service.
- For the sleep / wake I'm using [MultiSwitch for RetroPie](https://retropie.org.uk/forum/topic/17415/multi-switch-shutdown-script)

###On NodeMCU, Wemos D1 Mini, or another kind of ESP8266
- Mostly using the existing FastLED library, with the additions of a "Flux" option.
