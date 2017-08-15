# Many Hands PIR Arduino code
This project uses an Arduino UNO or compatible board (specifically, it was developed on a Duinotech UNO).

## Disclaimer
You must accept that You and You Alone are Responsible for your safety and safety of others in any endeavor in which you engage. While this code is provided in hopes that it will enable some Arduino enthusiasts and inventors to keep experimenting and inventing, You are Responsible for verifying its accuracy and applicability to your project. You are Responsible for knowing your limitations of knowledge and experience. If you do any work with "mains power" such as 120 or 240 VAC power wiring, you should consult a Licensed Electrician.


## Getting started
This README contains a short description with the essential bits of hardware only.

A full description of the setup can be found on [my blog](https://jora.io/2017/02/28/many-hands-at-secret-garden-festival/).


### Prerequisites
* [Arduino IDE 1.8.3](https://www.arduino.cc/en/Main/Software)
* [Arduino UNO compatible board](https://store.arduino.cc/usa/arduino-uno-rev3)
* [PIR Sensor](https://www.jaycar.com.au/arduino-compatible-pir-motion-detector-module/p/XC4444)
* [Relays](https://www.jaycar.com.au/arduino-compatible-8-channel-relay-board/p/XC4418)


### Installing
* Clone the repository
* Connect PIR to pin 3
* Connect relay to pins 4,5,6
* Run the code
* Trigger the PIR - the on board LED will turn on when this is activated

## Modifying the code
By using the [Many Hands Flash Generator Utility](https://bitbucket.org/jtradford/many-hands-flashgen), compatible code for flashing the lights can be generated.

This is then copied into the setupRoutines() function in many-hands-pir.ino.


## Version
This will not get a full release.

## Authors
* **Joseph Radford** - *Initial work*

##Acknowledgements
* [PIRSense](http://playground.arduino.cc/Code/PIRsense)
* [The disclaimer](https://arduino-info.wikispaces.com/Electrical_Safety_and_Disclaimer)
