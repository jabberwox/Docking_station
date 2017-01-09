#!bin/bash
#Script to configure GPIO pins

echo -e This script sets the GPIO pins, you need to have wiringPi installed for this

gpio unexportall
#gpio readall
gpio export 13 out #valve
gpio export 16 in #pushbuttonpin
gpio export 19 out #servo
gpio export 21 in #inductionsensor
gpio export 20 in #modeswitchpin
gpio export 12 out #connectionpin
gpio export 7 out #greenlightpin
gpio export 8 out #yellowlightpin
gpio export 5 out #motordriver IN2
gpio export 6 out #motordriver IN1	


gpio readall

echo -e Pins set
