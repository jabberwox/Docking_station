// Quick and dirty implementation of the ADC
// originated from: https://dbsnyder471.files.wordpress.com/2013/08/adtest_c.doc

// ADTest.c
//      Trying to read ADS1015
//      I2C 0x48
//run
//      gpio load i2c
//before using
 
#include "dockingstation/adc.hpp"

int fd;  /* I2C File descriptor */
int ADS1015Addr = 0x48;  /* device address */
 
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
 
char reg;
float value;
int sta;
int cfg;
int uslptm=1000; /* delay (us) for conversion */
int swapbytes(int data);
int ConfigAD(int cfg, int mode, int chan);
int ConfigPGA(int cfg, int mode);
int ConfigRate(int cfg, int mode);
int ConfigDOM(int cfg, int mode); /* CONTINUOUS or SINGLESHOT */
int i; 
 
double volts, vfs;
int ADScale[4] = {FS4, FS6, FS6, FS1};
 


int ADC::readAnalog(int a){
	
  fd = wiringPiI2CSetup(ADS1015Addr);
	if (fd<0) {
      printf("i2c error: fd<0  fd=%d  ... abend. ",fd);
      return 1;
	}
		 
  cfg = 0x8300;
  cfg = ConfigAD(cfg,MODESINGLE,a);
  cfg = ConfigPGA(cfg,ADScale[a]);
  sta = wiringPiI2CWriteReg16(fd,ADCONFIG,cfg);
  usleep(uslptm);
  value = wiringPiI2CReadReg16(fd,ADCONVERT);
  value = swapbytes(value);
  printf("ADS1015 Chan A%d: 0x%04x  %f V \n",i,value, (vfs*value)/0x8000);
//  std::cout << value << std::endl;
//  return(value);
}

float ADC::readPressure(){
	
  fd = wiringPiI2CSetup(ADS1015Addr);
	if (fd<0) {
      printf("i2c error: fd<0  fd=%d  ... abend. ",fd);
      return 1;
	} 
  cfg = 0x8300;
  cfg = ConfigAD(cfg,MODESINGLE,PRESSURE);
  cfg = ConfigPGA(cfg,ADScale[PRESSURE]);
  sta = wiringPiI2CWriteReg16(fd,ADCONFIG,cfg);
  usleep(uslptm);
  value = wiringPiI2CReadReg16(fd,ADCONVERT);
  value = swapbytes(value);

  value = (vfs*value)/0x8000; //convert to voltage
  value =  value * 2.0/2.4  - 0.5; // this is the output in bar (or 100 kPa) relative to ambient pressure (for 150 Ohm)
//  float f = 0.0035;
//  std::cout  << value << " bar" << std::endl;
//  printf("ADS1015 Chan %f bar \n", value);
//  printf("Pressure: %f bar \n", value);
  return(value);
}
int swapbytes(int d)
{
      int o;
 
      o= d & 0x00ff;
      o= o * 0x0100;
      o= o+ (d /0x0100);
      return o;
}
 
int ConfigAD(int cfg, int mode, int chan)
{
      int val;
      val = cfg;
      /* mode 0 - Differental */
      /* mode 1 - SingleEnded */
      /* <0 or other - leave as is */
      switch (mode) {
      case 0:
            val = val & 0xffbf;
            break;
      case 1:
            val = val | 0x0040;
            break;
      default:    /* no change */
            break;
      }
 
//    /* Channel
//    /*    0 - A0
//    /*    1 - A1
//    /*    2 - A2
//    /*    3 - A3
//    /***************/
 
      switch (chan) {
      case 0:  /* A0 or A01 */
            val = val & 0xffcf;
            val = val | 0x0000;
            break;
      case 1:     /* A1 or A03 */
            val = val & 0xffcf;
            val = val | 0x0010;
            break;
      case 2:     /* A2 or A12 */
            val = val & 0xffcf;
            val = val| 0x0020;
            break;
      case 3:     /* A3 or A23 */
            val = val & 0xffcf;
            val = val | 0x0030;
            break;
      default: /* no change */
            break;
      }
      return val;
}
 
int ConfigPGA(int cfg, int mode)
{
      int val;
      double sf;
      int clrPGA = 0xfff1;
 
      val = cfg;
      sf = vfs;
 
      switch (mode) {
      case FS6:
            val = val & clrPGA;
            val = val | 0X0000;
            sf = 6.144;
            break;
      case FS4:
            val = val & clrPGA;
            val = val | 0X0002;
            sf = 4.096;
            break;
      case FS2:
            val = val & clrPGA;
            val = val | 0X0004;
            sf = 2.048;
            break;
      case FS1:
            val = val & clrPGA;
            val = val | 0X0006;
            sf = 1.024;
            break;
      case FS05:
            val = val & clrPGA;
            val = val | 0X0008;
            sf = 0.512;
            break;
      case FS025:
            val = val & clrPGA;
            val = val | 0X000a;
            sf = 256;
            break;
      default:
            val = val & clrPGA;
            val = val | 0X0000;
            sf = 6.144;
            break;
      }
 
      vfs = sf;
      return val;
}
 
int ConfigRate(int cfg, int mode)
{
      int val;
      int clrSPS = 0x1fff;
 
      val = cfg; 
      switch (mode) {
      case SPS128:     
            val = val & clrSPS;
            val = val | 0x1fff;
            break;
      case SPS250:     
            val = val & clrSPS;
            val = val | 0x3fff;
            break;
      case SPS490:     
            val = val & clrSPS;
            val = val | 0x5fff;
            break;
      case SPS920:     
            val = val & clrSPS;
            val = val | 0x7fff;
            break;
      case SPS2400:    
            val = val & clrSPS;
            val = val | 0xbfff;
            break;
      case SPS3300:    
            val = val & clrSPS;
            val = val | 0xdfff;
            break;
 
      case SPS1600:     /* Power Up Default */
      default:
            val = val & clrSPS;
            val = val | 0x9fff;
            break;
      }
 
      return val;
 
}
 
int ConfigDOM(int cfg, int mode) /* CONTINUOUS or SINGLESHOT */
{
      int val;
 
      val = cfg;
 
      switch (mode) {
      case CONTINUOUS:
            val = val & 0xfffe;
            break;
      case SINGLESHOT:
      default:
            val = val | 0x0001;
            break;
      }
 
      return val;
}
 
 
 
