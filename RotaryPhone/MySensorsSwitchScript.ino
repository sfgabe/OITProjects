/**
 * **************************************************************************************************
 * This project convertes a old phone into a doorbell and home assistant interface.                 *
 * Based on the fantastic write up here:
 * https://superkris.tweakblogs.net/blog/16935/old-school-phone-as-doorbell-and-domoitcs-interface! *
 * **************************************************************************************************
 */

// ##### mysensors setup ###### (Edit child ID numbers if you want)
#define ProjectName "RingARing" // Name that is vissible in controller
#define ProjectVersion "v0.1"   // Version that is visible in controller
#define MY_DEBUG               // Enable debug prints to serial monitor
#define MY_RADIO_NRF24         // Enable and select radio type attached
#define MY_NODE_ID 53          // HomeAssistant needs this
#define childIdDoor 1          // child ID number used by mysensors foor doorbell button
#define childIdRing_1 11       // child ID number used by mysensors to activate ringtone 1
#define childIdRing_2 12       // child ID number used by mysensors to activate ringtone 2
#define childIdRing_3 13       // child ID number used by mysensors to activate ringtone 3
#define childIdRing_4 14       // child ID number used by mysensors to activate ringtone 4
#define childIdRing_5 15       // child ID number used by mysensors to activate ringtone 5
#define childIdRingAlarm 16    // child ID number used by mysensors to activate alarm
#define childIdDail_1 101      // child ID number used by mysensors when the number 1 is dailed
#define childIdDail_2 102      // child ID number used by mysensors when the number 2 is dailed
#define childIdDail_3 103      // child ID number used by mysensors when the number 3 is dailed
#define childIdDail_4 104      // child ID number used by mysensors when the number 4 is dailed
#define childIdDail_5 105      // child ID number used by mysensors when the number 5 is dailed
#define childIdDail_6 106      // child ID number used by mysensors when the number 6 is dailed
#define childIdDail_7 107      // child ID number used by mysensors when the number 7 is dailed
#define childIdDail_8 108      // child ID number used by mysensors when the number 8 is dailed
#define childIdDail_9 109      // child ID number used by mysensors when the number 9 is dailed
#define childIdDail_10 110     // child ID number used by mysensors when the number 0 is dailed

// ###### Include libraries #####
#include <SPI.h>               // needed for NRF24
#include <MySensors.h>         // mysensors library
#include <Bounce2.h>           // for debouncing buttons and dail

// ###### I/O pin number setup ###### (edit pin numbers if you connected the electronics to diffrent I/O pins)
#define button 3               // input pin activated when doorbell button is pressed
#define edail 8                // input pin activated when dail is used (Enable Dail)
#define pdail 7                // input pin activated with each tick of the dail (Pulse Dial)
#define lbell 4                // output pin to swing bellh hamer left
#define rbell 5                // output Pin to swing bellh hamer right

// ###ga## Debouncer variables ##### (no not edit)
Bounce debouncerButton = Bounce(); // Create button debouncer for doorbell button 
Bounce debouncerEdail = Bounce();  // Create button debouncer for Enable Dail
Bounce debouncerPdail = Bounce();  // Create button debouncer for Pulse Dial 
bool valueButton = 0;              // Debounced I/O bit from doorbell button pin
bool oldValueButton = 0;           // Old value to compare to current value 
bool valueEdail = 0;               // Debounced I/O bit from enable dail pin
bool oldValueEdail = 0;            // Old value to compare to current value
bool valuePdail = 0;               // Debounced I/O bit from pusle dail pin
bool oldValuePdail = 0;            // Old value to compare to current value


// ##### Variables for dailing disk decoding ##### (do not edit)
int dailCount = 0;                 // dail counter (while running)
int newDailCount = 0;              // dail counter set when dailing is completed

// ##### Variables for ringtone generator ##### (do not edit)
bool ringPhone = 0;                // bit that enables the rining of the phone
bool coilPowered = 0;              // safety bit that checks if the coil is already powered
int RingCyclePhase = 0;            // Keeps track of current ring cycle phase (1-5)
int ring = 0;                      // counter for amount of ring cycles passed
int repeat = 0;                    // counter for amount of repeats passed
unsigned long currentMillis = 0;   // used to keep track of current time
unsigned long onPulseMillis = 0;   // timer start for on pulse used for ring bell coil
unsigned long offPulseMillis = 0;  // timer start for off pulse used for ring bell coil
unsigned long ringPauseMillis = 0; // timer start for pause between 2 ringing sessions

// ##### Variables for ringtone 1 ##### (Edit the values as you like)
int onPulse_1 = 20;                // time the coil is powered to swing the hammer in ms. Typical value 5-50ms. No point trying other values
int offPulse_1 = 10;               // time the coil is switched off in ms (SETTING THIS TIME TO LOW MIGHT DAMAGE THE FETS) typical vaule 5-50ms
int ringX_1 = 20;                  // amount of ring cycles (controlls the length of each ring) 
int ringPause_1 = 2500;            // time paused between the each "ring"
int repeatX_1 = 3;                 // amount of "rings"

// ##### Variables for ringtone 2 ##### (Edit the values as you like)
int onPulse_2 = 15;                // time the coil is powered to swing the hammer in ms. Typical value 5-50ms. No point trying other values
int offPulse_2 = 7;                // time the coil is switched off in ms (SETTING THIS TIME TO LOW MIGHT DAMAGE THE FETS) typical vaule 5-50ms
int ringX_2 = 4;                   // amount of ring cycles (controlls the length of each ring) 
int ringPause_2 = 200;             // time paused between the each "ring"
int repeatX_2 = 2;                 // amount of "rings"

// ##### Variables for ringtone 3 ##### (Edit the values as you like)
int onPulse_3 = 15;                // time the coil is powered to swing the hammer in ms. Typical value 5-50ms. No point trying other values
int offPulse_3 = 7;                // time the coil is switched off in ms (SETTING THIS TIME TO LOW MIGHT DAMAGE THE FETS) typical vaule 5-50ms
int ringX_3 = 4;                   // amount of ring cycles (controlls the length of each ring) 
int ringPause_3 = 200;             // time paused between the each "ring"
int repeatX_3 = 4;                 // amount of "rings"

// ##### Variables for ringtone 4 ##### (Edit the values as you like)
int onPulse_4 = 20;                // time the coil is powered to swing the hammer in ms. Typical value 5-50ms. No point trying other values
int offPulse_4 = 10;               // time the coil is switched off in ms (SETTING THIS TIME TO LOW MIGHT DAMAGE THE FETS) typical vaule 5-50ms
int ringX_4 = 40;                  // amount of ring cycles (controlls the length of each ring) 
int ringPause_4 = 0;               // time paused between the each "ring"
int repeatX_4 = 1;                 // amount of "rings"

// ##### Variables for ringtone 5 ##### (Edit the values as you like)
int onPulse_5 = 15;                // time the coil is powered to swing the hammer in ms. Typical value 5-50ms. No point trying other values
int offPulse_5 = 10;               // time the coil is switched off in ms (SETTING THIS TIME TO LOW MIGHT DAMAGE THE FETS) typical vaule 5-50ms
int ringX_5 = 7;                   // amount of ring cycles (controlls the length of each ring) 
int ringPause_5 = 650;             // time paused between the each "ring"
int repeatX_5 = 5;                 // amount of "rings"

// ##### Variables for alarm ##### (Edit the values as you like)
int onPulseAlarm = 20;            // time the coil is powered to swing the hammer in ms. Typical value 5-50ms. No point trying other values
int offPulseAlarm = 10;           // time the coil is switched off in ms (SETTING THIS TIME TO LOW MIGHT DAMAGE THE FETS) typical vaule 5-50ms
int ringXAlarm = 15;              // amount of ring cycles (controlls the length of each ring) 
int ringPauseAlarm = 500;         // time paused between the each "ring"
int repeatXAlarm = 1250;          // amount of "rings" With th other timing setting above 1250 rings is approx a 1/2 hour of alarm.

// ##### Variables for current ringtone ##### (No point in edditing. These values are overwritten by other programmed ringtones)
int onPulse = 0;                   // time the coil is powered to swing the hammer in ms. (set to 0 by default)
int offPulse = 0;                  // time the coil is switched off in ms.(set to 0 by default)
int ringX = 1;                     // amount of ring cycles. (set to 1 by default so code doesnt run)
int ringPause = 0;                 // time paused between the each "ring" (set to 0 by default)
int repeatX = 1;                   // amount of "rings" (set to 1 by default so code doesnt run)


// ##### setup MySensors message containers #####
MyMessage msgDoor(childIdDoor,V_STATUS);        // message container used for doorbell button
MyMessage msgRing_1(childIdRing_1,V_STATUS);    // message cointainer for ringtone 1
MyMessage msgRing_2(childIdRing_2,V_STATUS);    // message cointainer for ringtone 2
MyMessage msgRing_3(childIdRing_3,V_STATUS);    // message cointainer for ringtone 3
MyMessage msgRing_4(childIdRing_4,V_STATUS);    // message cointainer for ringtone 4
MyMessage msgRing_5(childIdRing_5,V_STATUS);    // message cointainer for ringtone 5
MyMessage msgAlarm (childIdRingAlarm,V_STATUS); // message cointainer for ringtone 5
MyMessage msgDail_1(childIdDail_1,V_STATUS);    // message container used when the number 1 is dailed
MyMessage msgDail_2(childIdDail_2,V_STATUS);    // message container used when the number 2 is dailed
MyMessage msgDail_3(childIdDail_3,V_STATUS);    // message container used when the number 3 is dailed
MyMessage msgDail_4(childIdDail_4,V_STATUS);    // message container used when the number 4 is dailed
MyMessage msgDail_5(childIdDail_5,V_STATUS);    // message container used when the number 5 is dailed
MyMessage msgDail_6(childIdDail_6,V_STATUS);    // message container used when the number 6 is dailed
MyMessage msgDail_7(childIdDail_7,V_STATUS);    // message container used when the number 7 is dailed
MyMessage msgDail_8(childIdDail_8,V_STATUS);    // message container used when the number 8 is dailed
MyMessage msgDail_9(childIdDail_9,V_STATUS);    // message container used when the number 9 is dailed
MyMessage msgDail_10(childIdDail_10,V_STATUS);  // message container used when the number 0 is dailed

void setup()  
{  
//##### I/O pin function setup #####
  pinMode(button,INPUT);    //set the already defined I/O pin as input
  pinMode(edail, INPUT);    //set the already defined I/O pin as input
  pinMode(pdail, INPUT);    //set the already defined I/O pin as input
  pinMode(lbell, OUTPUT);   //set the already defined I/O pin as output
  pinMode(rbell, OUTPUT);   //set the already defined I/O pin as output
 
// ##### Debouncer setup #####
  debouncerButton.attach(button);
  debouncerButton.interval(5);
  debouncerEdail.attach(edail);
  debouncerEdail.interval(5);
  debouncerPdail.attach(pdail);
  debouncerPdail.interval(5);
}

// ##### Function of MySensors that presents all attached sensors to the controller #####
void presentation() { 
  sendSketchInfo(ProjectName, ProjectVersion);  // Send the sketch version information to the gateway and Controller
  present(childIdDoor, S_BINARY,"Ring Button");               // Present doorbell button as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdRing_1, S_BINARY,"Ringtone 1");             // Present ringtone 1 as a binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdRing_2, S_BINARY,"Ringtone 2");             // Present ringtone 2 as a binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdRing_3, S_BINARY,"Ringtone 3");             // Present ringtone 3 as a binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdRing_4, S_BINARY,"Ringtone 4");             // Present ringtone 4 as a binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdRing_5, S_BINARY,"Ringtone 5");             // Present ringtone 5 as a binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdRingAlarm, S_BINARY,"Ring Alarm");          // Present alarm as a binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_1, S_BINARY,"Rotary Dial 1");             // Present the dailing of number 1 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_2, S_BINARY,"Rotary Dial 2");             // Present the dailing of number 2 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_3, S_BINARY,"Rotary Dial 3");             // Present the dailing of number 3 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_4, S_BINARY,"Rotary Dial 4");             // Present the dailing of number 4 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_5, S_BINARY,"Rotary Dial 5");             // Present the dailing of number 5 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_6, S_BINARY,"Rotary Dial 6");             // Present the dailing of number 6 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_7, S_BINARY,"Rotary Dial 7");             // Present the dailing of number 7 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_8, S_BINARY,"Rotary Dial 8");             // Present the dailing of number 8 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_9, S_BINARY,"Rotary Dial 9");             // Present the dailing of number 9 as binary switch
  wait(200);                                                  // This stops HASS from losing nodes
  present(childIdDail_10, S_BINARY,"Rotary Dial 0");            // Present the dailing of number 0 as binary switch
}

void loop() {
  currentMillis = millis();                           // update timer

// ##### debouncer updater ##### 
  debouncerButton.update();                           // Update debouncer for doorbell button
  valueButton = debouncerButton.read();               // Set current value of doorbell button 
  debouncerEdail.update();                            // Update debouncer for enable dail
  valueEdail = debouncerEdail.read();                 // Set current value of enable dail
  debouncerPdail.update();                            // Update debouncer for pulse dail 
  valuePdail = debouncerPdail.read();                 // Set current value of pulse dail

// ##### Mysensors code to check doorbell button and sent message ##### 
  if (valueButton != oldValueButton) {                // Check if the value of the button has changed 
     send(msgDoor.set(valueButton==HIGH ? 1 : 0));    // Transmit the new value
     if (valueButton == HIGH ){                       // If the buttin was high
        onPulse = onPulse_1;                          // Write varialble of selected ringtone to the variable of the current ringtone
        offPulse = offPulse_1;                        // Write varialble of selected ringtone to the variable of the current ringtone
        ringX = ringX_1;                              // Write varialble of selected ringtone to the variable of the current ringtone
        ringPause = ringPause_1;                      // Write varialble of selected ringtone to the variable of the current ringtone
        repeatX = repeatX_1;                          // Write varialble of selected ringtone to the variable of the current ringtone
        ringPhone = HIGH;                             // Set the phone ring bit to high so the ringing can begin
       }
     oldValueButton = valueButton;                    // Change old value so this doenst loop
  }

// ##### Mysensors code to read dail counter and sent message as one individual button #####
  switch (newDailCount) {         // Check the current vallue of the completed counter
     case 1:                      // if value is equal to 1 
       send(msgDail_1.set(1));    // Transmit ON message for dail switch 1 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_1.set(1));    // Transmit ON message for dail switch 1 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_1.set(0));    // Transmit OFF message for dail switch 1. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_1.set(0));    // Send an extra just in case
       break;                     // end of case
     case 2:                      // if value is equal to 2 
       send(msgDail_2.set(1));    // Transmit ON message for dail switch 2
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_2.set(1));    // Transmit ON message for dail switch 2 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_2.set(0));    // Transmit OFF message for dail switch 2. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_2.set(0));    // Send an extra just in case
       break;                     // end of case
     case 3:                      // if value is equal to 3 
       send(msgDail_3.set(1));    // Transmit ON message for dail switch 3
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_3.set(1));    // Transmit ON message for dail switch 3 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_3.set(0));    // Transmit OFF message for dail switch 3. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_3.set(0));    // Send an extra just in case
       break;                     // end of case
     case 4:                      // if value is equal to 4 
       send(msgDail_4.set(1));    // Transmit ON message for dail switch 4
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_4.set(1));    // Transmit ON message for dail switch 4 
       wait(300);                 // This stops HASS from losing messages 
       send(msgDail_4.set(0));    // Transmit OFF message for dail switch 4. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_4.set(0));    // Send an extra just in case
       break;                     // end of case
     case 5:                      // if value is equal to 5 
       send(msgDail_5.set(1));    // Transmit ON message for dail switch 5 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_5.set(1));    // Transmit ON message for dail switch 5 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_5.set(0));    // Transmit OFF message for dail switch 5. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_5.set(0));    // Send an extra just in case
       break;                     // end of case
     case 6:                      // if value is equal to 6 
       send(msgDail_6.set(1));    // Transmit ON message for dail switch 6 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_6.set(1));    // Transmit ON message for dail switch 6 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_6.set(0));    // Transmit OFF message for dail switch 6. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_6.set(0));    // Send an extra just in case
       break;                     // end of case       
     case 7:                      // if value is equal to 7 
       send(msgDail_7.set(1));    // Transmit ON message for dail switch 7 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_7.set(1));    // Transmit ON message for dail switch 7 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_7.set(0));    // Transmit OFF message for dail switch 7. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_7.set(0));    // Send an extra just in case
       break;                     // end of case       
     case 8:                      // if value is equal to 8 
       send(msgDail_8.set(1));    // Transmit ON message for dail switch 8 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_8.set(1));    // Transmit ON message for dail switch 8 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_8.set(0));    // Transmit OFF message for dail switch 8. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_8.set(0));    // Send an extra just in case
       break;                     // end of case       
     case 9:                      // if value is equal to 9 
       send(msgDail_9.set(1));    // Transmit ON message for dail switch 9 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_9.set(1));    // Transmit ON message for dail switch 9 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_9.set(0));    // Transmit OFF message for dail switch 9. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_9.set(0));    // Send an extra just in case
       break;                     // end of case       
     case 10:                     // if value is equal to 10 
       send(msgDail_10.set(1));   // Transmit ON message for dail switch 10 
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_10.set(1));    // Transmit ON message for dail switch 10 
       wait(300);                 // This stops HASS from losing messages
       send(msgDail_10.set(0));   // Transmit OFF message for dail switch 10. Some home automation software prefers this.
       wait(100);                 // This stops HASS from losing messages
       send(msgDail_10.set(0));    // Send an extra just in case
       break;                     // end of case       
  }
  newDailCount = 0;                // Reset the completed counter so this doesnt loop

  
// ###### Code for checking enable dail and sending state trough serial  ######  
  if (valueEdail != oldValueEdail && valueEdail == LOW) {          // Check if enable dail has changed AND if its currently its currently activated
     Serial.println("dail is activated...");                        // If so sent message 
     oldValueEdail = valueEdail;}                                   // And change old value so this doenst loop
     else if (valueEdail != oldValueEdail && valueEdail == HIGH) {   // Check if enable dail has changed AND if its currently its currently deactivated
     Serial.println("dail is deactivated...");                      // If so sent message
     newDailCount = dailCount;                                      // Write the counted pulses to the New Dail Count
     dailCount = -1;                                                 // Reset the dail count for next dail 
     oldValueEdail = valueEdail;                                    // And change old value so this doenst loop
  }

// ###### Code for checking pusle dail and sending state trough serial ######
  if (valuePdail != oldValuePdail && valueEdail == LOW) {          // Check if dail pulse has changed AND if currently its currently activated
     if (valuePdail == HIGH) {                                       // Only take action when the signal goes from high to low to prevent double count
        dailCount++;                                                 // If the conditions are met increase counter by 1
        Serial.print("Tick! Total ammout of pulses: ");              // Serial print a messagge saying a pulse was detected
        Serial.println (dailCount);                                  // Serial print a the current value of the counter
     }
       oldValuePdail = valuePdail;                                  // Change old value so this doenst loop  
  }

// ##### ringtone code #####
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
// WARNING    "lbell" and "rbell" can NEVER be high at the same time. If this does happen your FET's will short and can go up in smoke!    WARNING
// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
// 
// This code must run within the void loop and can not use any loops of its own. This makes the code below a lot more complex, but alows
// the rest of the code to keep running while the bell is ringing. This way the other functions stay are not paused
  if (ringPhone == HIGH && ring <= ringX && RingCyclePhase != 5) {            // activeate ringing if ringPhone bit is set, the set amount of rings isnt reached, and the ringCyclePhase is not 5 (waiting)
     if (coilPowered == LOW && RingCyclePhase == 0) {                         // only activate coil if the coil is powered down and the ring cycle is complete
        coilPowered = HIGH;                                                   // enable safety bit that can be checked if coil is powered 
        RingCyclePhase = 1;                                                   // set ring cycle phase to 1 (coil powered to move hammer left)
        digitalWrite(lbell, HIGH);                                            // power coil to swing hammer into left poistion
        onPulseMillis = currentMillis;                                        // set start for timer for the on pulse 
        Serial.println ("left bell high");                                    // // *****Debug code***** Enable to see what is happening on serial monitor
     }
     if (currentMillis - onPulseMillis >= onPulse && RingCyclePhase == 1) {   // Check if the set time (onPulse) has passed and the ring cycle is still in phase 1
        digitalWrite(lbell, LOW);                                             // power down coil
        coilPowered = LOW;                                                    // disable safety bit that can be checked if coil is powered 
        RingCyclePhase = 2;                                                   // set ring cycle phase to 2 (coil powered down after moving hammer left)              
        offPulseMillis = currentMillis;                                       // set start for timer for the off pulse
        Serial.println ("left bell low");                                      // // *****Debug code***** Enable to see what is happening on serial monitor
     }
     if (currentMillis - offPulseMillis >= offPulse && RingCyclePhase == 2 && coilPowered == LOW) {  // check if the set time (offPulse) has passed and the ring cycle is still in phase 2
        coilPowered = HIGH;                                                   // enable safety bit that can be checked if coil is powered
        RingCyclePhase = 3;                                                   // set ring cycle phase to 3 (coil powered to move hammer right)
        digitalWrite(rbell, HIGH);                                            // power coil to swing hammer into right poistion
        onPulseMillis = currentMillis;                                        // set start for timer for the on pulse 
        Serial.println ("right bell high");                                   // *****Debug code***** Enable to see what is happening on serial monitor
     }
     if (currentMillis - onPulseMillis >= onPulse && RingCyclePhase == 3) {   // check if the set time (onPulse) has passed and the ring cycle is still in phase 3
        digitalWrite(rbell, LOW);                                             // power down coil
        coilPowered = LOW;                                                    // disable safety bit that can be checked if coil is powered  
        RingCyclePhase = 4;                                                   // set ring cycle phase to 4 (coil powered down after moving hammer left)              
        offPulseMillis = currentMillis;                                       // set start for timer for the off pulse
        Serial.println ("right bell low");                                    // Debug code, enable to see what is happening on serial monitor
     }
     if (currentMillis - offPulseMillis >= offPulse && RingCyclePhase == 4) { // check if the set time (offPulse) has passed and the ring cycle is still in phase 4
        RingCyclePhase = 0;                                                   // set ring cycle phase to 0 (ring cycle completed)           
        ring++;                                                               // add 1 count to the "ring" counter    
        Serial.print("numbers of rings : ");                                  // *****Debug code***** Enable to see what is happening on serial monitor
        Serial.println (ring);                                                // *****Debug code***** Enable to see what is happening on serial monitor
     }         
  }     
  if (ring >= ringX){                                                         // if the amount of "ring" is the same as the amount set in "ringX"....
     ring = 0;                                                                // reset ring counter
     RingCyclePhase = 5;                                                      // set ring cycle phase to 5 (wait to start new ring cycle)       
     Serial.println ("Ring cylce 5 reached (pause)");                         // *****Debug code***** Enable to see what is happening on serial monitor
     ringPauseMillis = currentMillis;                                         // set start for timer for the pause
  }  
  if (currentMillis - ringPauseMillis >= ringPause && RingCyclePhase == 5) {  // if the set pause time has passed and the ring cycle is actualy in its pause phase
     RingCyclePhase = 0;                                                      // set ringCyclePhase back to 0 so the ringing can resume again
     Serial.println ("End off pause, start ringing again");                   // *****Debug code***** Enable to see what is happening on serial monitor
     repeat++;                                                                // add 1 count to the "repeat" counter   
  }   
  if (repeat >= repeatX){                                                     // if the amount of repeat is the same as the amount set in "repeatX"....
     repeat = 0;                                                              // reset the repeat counter
     Serial.println ("All ringing done.");                                    // *****Debug code***** Enable to see what is happening on serial monitor 
     ringPhone = LOW;                                                         // ringtone is done. disable ringphone bit so the ringing stops 
  }      
}


void receive(const MyMessage &message){                                // Mysensors code that checks for messages form the mysensors network
  
//  ##### Mysensors code for activating ringtone 1 #####
   if (message.type == V_STATUS && message.sensor == childIdRing_1 && ringPhone != HIGH) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      onPulse = onPulse_1;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      offPulse = offPulse_1;                                           // Write varialble of selected ringtone to the variable of the current ringtone
      ringX = ringX_1;                                                 // Write varialble of selected ringtone to the variable of the current ringtone
      ringPause = ringPause_1;                                         // Write varialble of selected ringtone to the variable of the current ringtone
      repeatX = repeatX_1;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      ringPhone = HIGH;                                                // Set the phone ring bit to high so the ringing can begin
      send(msgRing_1.set(0));                                          // Transmit OFF message for ringtone. Some home automation software prefers this.
      Serial.println ("Received message to play ringtone 1");          // *****Debug code***** Enable to see what is happening on serial monitor 
   }
   
//  ##### Mysensors code for activating ringtone 2 #####
   if (message.type == V_STATUS && message.sensor == childIdRing_2 && ringPhone != HIGH && message.getBool() == 1) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      onPulse = onPulse_2;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      offPulse = offPulse_2;                                           // Write varialble of selected ringtone to the variable of the current ringtone
      ringX = ringX_2;                                                 // Write varialble of selected ringtone to the variable of the current ringtone
      ringPause = ringPause_2;                                         // Write varialble of selected ringtone to the variable of the current ringtone
      repeatX = repeatX_2;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      ringPhone = HIGH;                                                // Set the phone ring bit to high so the ringing can begin
      send(msgRing_2.set(0));                                          // Transmit OFF message for ringtone. Some home automation software prefers this.
      Serial.println ("Received message to play ringtone 2");          // *****Debug code***** Enable to see what is happening on serial monitor 
   }
   
//  ##### Mysensors code for activating ringtone 3 #####
   if (message.type == V_STATUS && message.sensor == childIdRing_3 && ringPhone != HIGH && message.getBool() == 1) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      onPulse = onPulse_3;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      offPulse = offPulse_3;                                           // Write varialble of selected ringtone to the variable of the current ringtone
      ringX = ringX_3;                                                 // Write varialble of selected ringtone to the variable of the current ringtone
      ringPause = ringPause_3;                                         // Write varialble of selected ringtone to the variable of the current ringtone
      repeatX = repeatX_3;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      ringPhone = HIGH;                                                // Set the phone ring bit to high so the ringing can begin
      send(msgRing_3.set(0));                                          // Transmit OFF message for ringtone. Some home automation software prefers this.
      Serial.println ("Received message to play ringtone 3");          // *****Debug code***** Enable to see what is happening on serial monitor 
   }

//  ##### Mysensors code for activating ringtone 4 #####
   if (message.type == V_STATUS && message.sensor == childIdRing_4 && ringPhone != HIGH && message.getBool() == 1) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      onPulse = onPulse_4;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      offPulse = offPulse_4;                                           // Write varialble of selected ringtone to the variable of the current ringtone
      ringX = ringX_4;                                                 // Write varialble of selected ringtone to the variable of the current ringtone
      ringPause = ringPause_4;                                         // Write varialble of selected ringtone to the variable of the current ringtone
      repeatX = repeatX_4;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      ringPhone = HIGH;                                                // Set the phone ring bit to high so the ringing can begin
      send(msgRing_4.set(0));                                          // Transmit OFF message for ringtone. Some home automation software prefers this.
      Serial.println ("Received message to play ringtone 4");          // *****Debug code***** Enable to see what is happening on serial monitor 
   }

//  ##### Mysensors code for activating ringtone 5 #####
   if (message.type == V_STATUS && message.sensor == childIdRing_5 && ringPhone != HIGH && message.getBool() == 1) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      onPulse = onPulse_5;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      offPulse = offPulse_5;                                           // Write varialble of selected ringtone to the variable of the current ringtone
      ringX = ringX_5;                                                 // Write varialble of selected ringtone to the variable of the current ringtone
      ringPause = ringPause_5;                                         // Write varialble of selected ringtone to the variable of the current ringtone
      repeatX = repeatX_5;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      ringPhone = HIGH;                                                // Set the phone ring bit to high so the ringing can begin
      send(msgRing_5.set(0));                                          // Transmit OFF message for ringtone. Some home automation software prefers this.
      Serial.println ("Received message to play ringtone 5");          // *****Debug code***** Enable to see what is happening on serial monitor 
   }

//  ##### Mysensors code for activating alarm #####
   if (message.type == V_STATUS && message.sensor == childIdRingAlarm && message.getBool() == 1) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      onPulse = onPulseAlarm;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      offPulse = offPulseAlarm;                                           // Write varialble of selected ringtone to the variable of the current ringtone
      ringX = ringXAlarm;                                                 // Write varialble of selected ringtone to the variable of the current ringtone
      ringPause = ringPauseAlarm;                                         // Write varialble of selected ringtone to the variable of the current ringtone
      repeatX = repeatXAlarm;                                             // Write varialble of selected ringtone to the variable of the current ringtone
      ringPhone = HIGH;                                                   // Set the phone ring bit to high so the ringing can begin
      Serial.println ("enable alarm signal;");                            // *****Debug code***** Enable to see what is happening on serial monitor 
   }
   if (message.type == V_STATUS && message.sensor == childIdRingAlarm && message.getBool() == 0) {  // Check if the incomming message is a of same as declared (V_status) and check the child ID 
      repeat = repeatXAlarm;
      send(msgRing_1.set(0));                                          // Transmit OFF message for ringtone. Some home automation software prefers this.
      Serial.println ("alarm signal disabled;");                            // *****Debug code***** Enable to see what is happening on serial monitor 
   }

}
