#include "dockingstation/dockingstation.hpp"

void Dockingstation::setupGPIO(){
	wiringPiSetupSys();
}

void Dockingstation::setValve(bool on) {
	digitalWrite(VALVEPIN, on);
}

void Dockingstation::setServo(int pwm) {
	digitalWrite(SERVOPIN,LOW);
	softPwmCreate(SERVOPIN,0,200);
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

void Dockingstation::setRedLight(int state) {
int	blinktime =	1000/BLINKFREQUENCY; //frequency to milliseconds
switch (state) {
	case 0:
		digitalWrite(REDLIGHTPIN, LOW);
		break;
	case 1:
		digitalWrite(REDLIGHTPIN, HIGH);
		break;
//	case 2:
	}
}
			
//void Dockingstation::setActuator(int direction) {
//	digitalWrite(ACTUATORPIN,LOW);
//	softPwmCreate(ACTUATORPIN,0,200);
//	softPwmWrite(ACTUATORPIN,180);
//}
