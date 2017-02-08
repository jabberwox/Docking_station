#!bin/bash
#Script to configure GPIO pins in BCM

echo -e This script sets the GPIO pins, you need to have wiringPi installed for this

gpio unexportall
#gpio readall
gpio export 13 out #valve
gpio export 16 in #pushbuttonpin
gpio export 19 out #plug actuator
gpio export 6 out #hatch actautor
gpio export 21 in #inductionsensor
gpio export 20 in #modeswitchpin
gpio export 12 in #connectionpin
gpio export 7 out #greenlightpin
gpio export 8 out #yellowlightpin

gpio readall

echo -e Pins set
