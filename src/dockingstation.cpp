/*!
 * @file	dockingstation.cpp
 * @author	Hendrik Kolvenbach
 * @date	February, 2017
 */

#include "dockingstation/dockingstation.hpp"

namespace dockingstation {


Dockingstation::Dockingstation(any_node::Node::NodeHandlePtr nh) : any_node::Node(nh) {
}

Dockingstation::~Dockingstation() {
}

void Dockingstation::init() {
	this->setupGPIO();
	this->initializeActuators();
}

void Dockingstation::cleanup() {
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
		for (int t=0; t<10 ; t++) {
		digitalWrite(YELLOWLIGHTPIN, HIGH);
		delay(500);
		digitalWrite(YELLOWLIGHTPIN, LOW);
		delay(500);
	}
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
		for (int t=0; t<10 ; t++) {
		digitalWrite(GREENLIGHTPIN, HIGH);
		delay(500);
		digitalWrite(GREENLIGHTPIN, LOW);
		delay(500);
		}	
	}
}


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

bool Dockingstation::senseContact() {
	return !digitalRead(CONTACTPIN);
}

bool Dockingstation::initiateDocking() {
	this->setHatchActuator(1);
	this->setYellowLight(2);
	this->setPlugActuator(1);
	this->setYellowLight(2);
	
	bool connection = senseContact();
	
	if (connection == false) {
		for (int n=0; n < 3; n++) {
			ROS_INFO_STREAM("Contact cannot be established, Trying again");
			softPwmWrite(PLUGACTUATORPIN,185);
			softPwmWrite(PLUGACTUATORPIN,180);
				if (this->senseContact() == true) {
					connection = true;
					break;
				}
			}
			ROS_INFO_STREAM("Contact cannot be established, Aborting");
			this->setHatchActuator(0);
			this->setYellowLight(2);
			this->setPlugActuator(0);
			this->setYellowLight(2);
		}

}
			
void Dockingstation::monitorCurrent() {
	
	
}

bool Dockingstation::update(const any_worker::WorkerEvent& event) {
	
	while(this->senseAnymal() == TRUE) {
			bool docking = this->initiateDocking();
			if (docking == true) {
					//Do something
			}
		}
	ROS_INFO_STREAM("Anymal not sensed"); 
	
	
	
}


}// namespace Dockingstation
