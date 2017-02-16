// Quick and dirty implementation of the ADC
// originated from: https://dbsnyder471.files.wordpress.com/2013/08/adtest_c.doc

//      Trying to read ADS1015
//      I2C 0x48
//run
//      gpio load i2c
//before using

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <iostream>

#define PRESSURE 0 //Pressure Sensor connected to Analog In 0
#define CURRENT 1 //Resistance is connected to Analog In 1

/* ADS1015 Registers */
#define ADCONFIG 1
#define ADCONVERT 0
#define ADLOW 2
#define ADHIGH 3
 
/* ADS1015 MUX Settings */
#define MODEDIFF 0  /* Defferential Mode */
#define MODESINGLE 1 /* Single Ended Mode */
 
/* Single Ended Channels */
#define A0 0
#define A1 1
#define A2 2
#define A3 3                
 
/* Differential Channels */
#define A01 0
#define A03 1
#define A13 2
#define A23 3
 
/* Scale Settings - Programable Gain Amplifier */
#define FS6   0  /* 6.114V Full Scale */
#define FS4   1  /* 4.096V Full Scale */
#define FS2   2  /* 2.048V Full Scale */
#define FS1   3  /* 1.012V Full Scale */
#define FS05  4  /* 0.512V Full Scale */
#define FS025 5  /* 0.256V Full Scale */
 
/* Data Rate */
#define SPS128   0  /* 128 samples/second  8ms*/
#define SPS250   1  /* 240 samples/second  4ms*/
#define SPS490   2  /* 490 samples/second  2.04ms*/
#define SPS920   3  /* 920 samples/second  1.09ms*/
#define SPS1600  4  /* 1600 samples/second  0.625ms*/
#define SPS2400  5  /* 2400 samples/second  0.420ms*/
#define SPS3300  6  /* 3300 samples/second  0.304ms*/
 
/* DOM: Device Operating Mode */
#define SINGLESHOT 1  /* Power down Single Shot mode */
#define CONTINUOUS  0  /* Continuous Convertion */


class ADC {
	
	public:
	float readAnalog(int a); //use 1-4 for Analog inputs. Mostly for debug.
	float readPressure(); //to read Festo SPTW-P2R-G14-A-M12 (0-2 bar relative pressure, 4-20mA over 250 Ohms resistor): Returns delta_p in bar
	float readCurrent(); //to read current floating to the robot: Returns Amps
	
	private:
	int swapbytes(int data);
	int ConfigAD(int cfg, int mode, int chan);
	int ConfigPGA(int cfg, int mode);
	int ConfigRate(int cfg, int mode);
	int ConfigDOM(int cfg, int mode); /* CONTINUOUS or SINGLESHOT */
	int fd;  /* I2C File descriptor */
	int ADS1015Addr = 0x48;  /* device address */
	char reg;
	float value;
	int sta;
	int cfg;
	int uslptm=1000; /* delay (us) for conversion */
	int i; 
	double volts, vfs;
	int ADScale[4] = {FS4, FS6, FS6, FS1};
};
