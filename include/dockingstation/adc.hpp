#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>

#define PRESSURE 0 //Pressure Sensor connected to Analog In 0


class ADC {
	
	public:
	float readAnalog(int a); //use 1-4 for Analog inputs
	float readPressure(); //to read Festo SPTW-P2R-G14-A-M12 (0-2 bar relative pressure, 4-20mA over 250 Ohms resistor)
};
