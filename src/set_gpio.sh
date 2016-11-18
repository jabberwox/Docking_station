#!bin/bash
#Script to configure GPIO pins

echo -e This script sets the GPIO pins, you need to have wiringPi installed for this

gpio unexportall
#gpio readall
gpio export 13 out #valve
gpio export 19 out #servo
gpio export 21 in #inductionsensor
gpio export 20 in #User Mode select
gpio export 12 out #yellowlightpin

gpio readall

echo -e Pins set
