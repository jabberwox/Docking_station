#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>

class ADC {
	
	public:
	int readAnalog(int a); //use 1-4 for Analog inputs
	
};
