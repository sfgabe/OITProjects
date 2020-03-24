# Renter's Light Switch #
Turn your lights on and off remotely and still get your security deposit back! 

This is a switchplate for adding a mechanical micro servo to decora/paddle style lightswitches. There's still enough room to operate the switch manually, so you can still use it the regular way too (but how boring!).

You'll need:
- [printed switchplate](https://www.thingiverse.com/thing:3289916) or duct tape I guess?
- [SG90 servo](https://amzn.to/2GtNXtw)
- [ESP8266 / NodeMCU / WEMOS D1 / etc. ](https://amzn.to/3drUMI8)
- [MQTT with Home Assistant](https://www.home-assistant.io/integrations/mqtt/) (or some other MQTT server setup)

## Printing and Assembly ##
Prints best lying flat, with supports. It doesn't matter if the supports make it look scrappy because they're on the back.

The SG90 slips right in from the back, and can be secured with small screws or M2x12mm bolts (there is just enough room for the nuts to slide behind the servo, though you may need tweezers). 

_Note: Don't add the servo arm until you've tested the code because it needs to find its "home" axis without hitting the switch._

## Wiring ##

| SG90 | NodeMCU | PIR Sensor |
------- | -------- | ------------
| S | D1 |  | 
| (+)  | Vin |  | 
| (-) |  Gr |  | 
|  | 3.3v  | (+) |
| |  Gr  | (-) |
| | D5 | S | 
                 
## Upload and Test ##
Upload the code with Arduino, however you usually do that, install the switchplate and hide the board somewhere near the lightswitch (mine conveniently runs behind a cabinet) and power up.
Once it starts up the servo should find it's default "neutral" rotation, now you can add the two sided servo arm.

## Setup for Home Assistant ##

__Switch for light in switches.yaml__
```
- platform: mqtt
  name: Entry Light
  command_topic: "lightswitch/set"
  state_topic: "lightswitch/current"
  state_on: "ON"
  payload_on: "P"
  state_off: "OFF"
  payload_off: "3"
  value_template: '{{ value_json.state }}'
```

__Sensor for motion sensor in binary_sensor.yaml__
```
- platform: mqtt
  name: entry_motion
  state_topic: 'lightswitch/current'
  value_template: '{{ value_json.motion }}'
  payload_on: "motion detected"
  payload_off: "standby"
  device_class: motion
```


*Note: Home assistant sends MQTT messages as strings, which unfortunatly this JSON library doesnt parse. You may need to adjust for your switches [with this handy chart](https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html). For example, my switch works with ON at 51 degrees (ascii payload "3") and OFF at 80 degrees (ascii payload "P").*
