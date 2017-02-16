#include "dockingstation/adc.hpp"

float ADC::readAnalog(int a){
	
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
  value = (vfs*value)/0x8000;
//  printf("ADS1015 Chan A%d: 0x%04x  %f V \n",i,value, (vfs*value)/0x8000);
//  std::cout << value << std::endl;
  return(value);
}

float ADC::readPressure(){
	
  value = this->readAnalog(PRESSURE); 
  value =  value * 1/2-0.5; // this is the output in bar (or 100 kPa) relative to input pressure (for 250 Ohm)
  return(value);
}

float ADC::readCurrent(){
	
  value = this->readAnalog(CURRENT); 
// value =  value * 1/2-0.5; // insert conversion

  return(value);
}

int ADC::swapbytes(int d)
{
      int o;
 
      o= d & 0x00ff;
      o= o * 0x0100;
      o= o+ (d /0x0100);
      return o;
}
 
int ADC::ConfigAD(int cfg, int mode, int chan)
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
 
int ADC::ConfigPGA(int cfg, int mode)
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
 
int ADC::ConfigRate(int cfg, int mode)
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
 
int ADC::ConfigDOM(int cfg, int mode) /* CONTINUOUS or SINGLESHOT */
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
 
 
 
