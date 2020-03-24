# Renter's Light Switch #
Turn your lights on and off remotely and still get your security deposit back! 

![Lightswitch1](/RentersLightSwitch/lightswitch1.png "Lightswitch1")

This is a switchplate for adding a mechanical micro servo to decora/paddle style lightswitches. There's still enough room to operate the switch manually, so you can still use it the regular way too (but how boring!).

You'll need:
- [printed switchplate](/RentersLightSwitch/lightswitch-servo-plate.stl) or on [Thingiverse here](https://www.thingiverse.com/thing:3289916) or duct tape I guess?
- [SG90 servo](https://amzn.to/2GtNXtw)
- [ESP8266 / NodeMCU / WEMOS D1 / etc. ](https://amzn.to/3drUMI8)
- [MQTT with Home Assistant](https://www.home-assistant.io/integrations/mqtt/) (or some other MQTT server setup)

## Printing and Assembly ##
Prints best lying flat, with supports. It doesn't matter if the supports make it look scrappy because they're on the back.

The SG90 slips right in from the back, and can be secured with small screws or M2x12mm bolts (there is just enough room for the nuts to slide behind the servo, though you may need tweezers). 

![Lightswitch3](/RentersLightSwitch/lightswitch3.jpg "Lightswitch3")

_Note: Don't add the servo arm until you've tested the code because it needs to find its "neutral" axis without hitting the switch._

## Wiring ##

| SG90 | NodeMCU | PIR Sensor |
------- | -------- | ------------
| S | D1 |  | 
| (+)  | Vin |  | 
| (-) |  Gr |  | 
|  | 3.3v  | (+) |
| |  Gr  | (-) |
| | D5 | S | 

![Lightswitch_Fritz](/RentersLightSwitch/lightswitch_fritz.png "Lightswitch Fritz")
                 
## Upload and Test ##
Upload the code with Arduino, however you usually do that, install the switchplate and hide the board somewhere near the lightswitch (mine conveniently runs behind a cabinet) and power up.

Once it starts up the servo should find it's default "neutral" rotation, now you can add the two sided servo arm that is neither on nor off.

## Setup for Home Assistant ##

__Switch for light in switches.yaml__
```
- platform: mqtt
  name: Light Switch
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
  name: Light Switch Motion
  state_topic: 'lightswitch/current'
  value_template: '{{ value_json.motion }}'
  payload_on: "motion detected"
  payload_off: "standby"
  device_class: motion
```

*Note: Home assistant sends MQTT messages as strings, which unfortunatly this JSON library doesnt parse. You may need to adjust for your switches [with this handy chart](https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html). For example, my switch works with ON at 51 degrees (ascii payload "3") and OFF at 80 degrees (ascii payload "P").*

__Automation for motion detect light__
I decided to keep the motion detection and the switch as two separate entities for more control, but if you want a basic motion triggered light add this to automations.yaml.
```
- alias: switch_motion_trigger_on
  trigger:
    platform: state
    entity_id: binary_sensor.light_switch_motion
    to: 'on'
  action:
    service: switch.turn_on
    entity_id: switch.light_switch
    
- alias: switch_motion_trigger_off
  trigger:
    platform: state
    entity_id: binary_sensor.light_switch_motion
    to: 'off'
    for:
      minutes: 10
  action:
    service: switch.turn_off
    entity_id: switch.light_switch

```
