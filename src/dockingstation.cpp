#include "dockingstation/dockingstation.hpp"
#include "dockingstation/adc.hpp"

Dockingstation::Dockingstation() {
	this->setupGPIO();
	this->initializeActuators();
}

Dockingstation::~Dockingstation() {
	ROS_INFO_STREAM("Shutting down");
	this->setHatchActuator(0);
	delay(5000);
	this->setPlugActuator(0);
	delay(5000);
	this->setValve(0); 
	this->setYellowLight(0); 
    this->setGreenLight(0); 
}

void Dockingstation::setupGPIO(){
	wiringPiSetupSys();
}

void Dockingstation::setValve(bool on) {
	digitalWrite(VALVEPIN, on);
}

void Dockingstation::initializeActuators() {
	digitalWrite(PLUGACTUATORPIN,LOW);
	softPwmCreate(PLUGACTUATORPIN,0,200);
	digitalWrite(HATCHACTUATORPIN,LOW);
	softPwmCreate(HATCHACTUATORPIN,0,200);
}	

void Dockingstation::setPlugActuator(bool plugactuator) {
switch (plugactuator) {
	case 0:
	softPwmWrite(PLUGACTUATORPIN,190); //190 is minimum 180 is maximum out, the rest is in between
	break;
	case 1:
	softPwmWrite(PLUGACTUATORPIN,180);
	break;
}
}

void Dockingstation::setHatchActuator(bool hatchactuator) {
switch (hatchactuator) {
	case 0:
	softPwmWrite(HATCHACTUATORPIN,190); //190 is minimum 180 is maximum out, the rest is in between
	break;
	case 1:
	softPwmWrite(HATCHACTUATORPIN,180);
	break;
}
}

bool Dockingstation::senseAnymal()  {
	return digitalRead(INDUCTIONPIN);
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
	case 2:		
		digitalWrite(YELLOWLIGHTPIN, HIGH);
		delay(blinktime/2);
		digitalWrite(YELLOWLIGHTPIN, LOW);
		delay(blinktime/2);
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
	case 2:
		digitalWrite(GREENLIGHTPIN, HIGH);
		delay(blinktime/2);
		digitalWrite(GREENLIGHTPIN, LOW);
		delay(blinktime/2);
//	}
	}}

//		if (thread_running == false) {
//		std::thread t1(Dockingstation::setGreenLightBlinking);
//		t1.detach();
		//break;
//	}
//	}
//}

/*
void Dockingstation::setGreenLightBlinking() {
	int	blinktime =	1000/BLINKFREQUENCY; //frequency to milliseconds
	thread_running = true;
	while(true) {
		digitalWrite(GREENLIGHTPIN, HIGH);
		delay(blinktime/2);
		digitalWrite(GREENLIGHTPIN, LOW);
		delay(blinktime/2);
	}
}
*/


bool Dockingstation::allowFilling()  {
	return digitalRead(MODEPIN); //returns the value of the manual/autonomous switch for charging
}


void Dockingstation::gasFilling() {
	ADC ad;
	//ROS_INFO_STREAM(ADC::readPressure());
	if(this->allowFilling() == true) {
		if(ad.readPressure() <= FILLINGPRESSURE) { //if the pressure difference is below 0.4 bar, open the valve
			this->setValve(1);
			this->setGreenLight(1);
			} 
		if(ad.readPressure() > FILLINGPRESSURE) {
			this->setValve(1);
			this->setGreenLight(0);
		} 
	}
}

bool Dockingstation::senseDocking() {
	return !digitalRead(CONTACTPIN);
}

void Dockingstation::initiateDocking() {
	this->setHatchActuator(1);
	delay(5000);
	this->setPlugActuator(1);
	delay(5000);
	
	bool connection = senseDocking();
	
	if (connection == false) {
		for (int n=0; n < 3; n++) {
			ROS_INFO_STREAM("Contact cannot be established, Trying again");
			softPwmWrite(PLUGACTUATORPIN,185);
			softPwmWrite(PLUGACTUATORPIN,180);
				if (this->senseDocking() == true) {
					connection = true;
					break;
				}
			}
			ROS_INFO_STREAM("Contact cannot be established, Aborting");
			this->setHatchActuator(0);
			delay(5000);
			this->setPlugActuator(0);
			delay(5000);
		}

	}
			
	
