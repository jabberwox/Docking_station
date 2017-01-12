#include "dockingstation/dockingstation.hpp"
#include "dockingstation/adc.hpp"

void Dockingstation::setupGPIO(){
	wiringPiSetupSys();
}

void Dockingstation::setValve(bool on) {
	digitalWrite(VALVEPIN, on);
}

void Dockingstation::initializeServo() {
	digitalWrite(SERVOPIN,LOW);
	softPwmCreate(SERVOPIN,0,200);
}	

void Dockingstation::setServo(int pwm) {
	softPwmWrite(SERVOPIN,pwm);
}

bool Dockingstation::senseAnymal()  {
	return digitalRead(INDUCTIONPIN);
}

bool Dockingstation::senseManualOperationMode()  {
	return digitalRead(MODEPIN); //returns the value of the manual/autonomous switch for charging
}

void Dockingstation::setYellowLight(int state) {
int	blinktime =	1000/BLINKFREQUENCY; //frequency to milliseconds
switch (state) {
	case 0:
		digitalWrite(YELLOWLIGHTPIN, LOW);
		break;
	case 1:
		digitalWrite(YELLOWLIGHTPIN, HIGH);
		break;
//	case 2:
	}
}

void Dockingstation::setGreenLight(int state) {
int	blinktime =	1000/BLINKFREQUENCY; //frequency to milliseconds
switch (state) {
	case 0:
		digitalWrite(GREENLIGHTPIN, LOW);
		break;
	case 1:
		digitalWrite(GREENLIGHTPIN, HIGH);
		break;
//	case 2:
	}
}

void Dockingstation::moveActuator(int mode) {
switch (mode) {
	case 0:
	ROS_INFO_STREAM("case 0");
	digitalWrite(MOTORIN1PIN, LOW);
	digitalWrite(MOTORIN2PIN, LOW);	
	break;
	case 1:
	ROS_INFO_STREAM("case 1");
	digitalWrite(MOTORIN1PIN, LOW);
	digitalWrite(MOTORIN2PIN, HIGH);
	break;
	case 2:
	ROS_INFO_STREAM("case 2");
	digitalWrite(MOTORIN1PIN, HIGH);
	digitalWrite(MOTORIN2PIN, LOW);
	break;
}
}

			
//void Dockingstation::setActuator(int direction) {
//	digitalWrite(ACTUATORPIN,LOW);
//	softPwmCreate(ACTUATORPIN,0,200);
//	softPwmWrite(ACTUATORPIN,180);
//}
