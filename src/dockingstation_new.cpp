/*!
 * @file	dockingstation.cpp
 * @author	Hendrik Kolvenbach
 * @date	February, 2017
 */

#include "dockingstation/dockingstation.hpp"


namespace dockingstation {

int pi;
bool sensed = false;
bool docked = false;
//int i2c;

Dockingstation::Dockingstation(any_node::Node::NodeHandlePtr nh) : any_node::Node(nh) {
}

Dockingstation::~Dockingstation() {
}

void Dockingstation::init() {
//	screenPublisher.notify(notification::level::LEVEL_INFO, "Initialize", "Initialize dockingstation node"); 
	statusPublisher = advertise<dockingstation_msgs::DockingstationStatus> ("dockingstation", "/dockingstation/status", 1, false);
	infoPublisher = advertise<std_msgs::String> ("dockingstation", "/dockingstation/info", 1, false);
	this->setupGPIO();
	this->initializeActuators();
//	docked = 0;
}

void Dockingstation::cleanup() {
	ROS_INFO_STREAM("Shutting down");
	set_PWM_dutycycle(pi, PLUGACTUATORPIN, ACTUATORIN);
	this->setYellowLight(2, DELAYPLUG);
	set_PWM_dutycycle(pi, HATCHACTUATORPIN, ACTUATOROUT); //For some reason the servos only work with 18-19ms signals, other than described in the library where its 0.5-2.5 us. Found out after 8h debugging.
	this->setYellowLight(2, DELAYHATCH);

	this->setValve(0); 
	this->setYellowLight(0,0); 
    this->setGreenLight(0,0); 
}

void Dockingstation::setupGPIO(){

	pi = pigpio_start(0,0);
	if (pi < 0) {
		ROS_INFO_STREAM("Couldnt load gpio library");
	} 
//	ROS_INFO_STREAM(pi);
//	int i2c = i2c_open(pi, 1, 0x48,0);
//	if (i2c<0) {
//		ROS_INFO_STREAM(i2c);
//	}
	set_mode(pi, GREENLIGHTPIN, PI_OUTPUT);
	set_mode(pi, YELLOWLIGHTPIN, PI_OUTPUT);
	set_mode(pi, VALVEPIN, PI_OUTPUT);
	set_mode(pi, BUTTONPIN, PI_INPUT);

	set_mode(pi, INDUCTIONPIN, PI_INPUT);
	set_mode(pi, MODEPIN, PI_INPUT);
	set_mode(pi, CONTACTPIN, PI_INPUT);

	gpio_write(pi, YELLOWLIGHTPIN, 0);
	gpio_write(pi, GREENLIGHTPIN, 0);
	gpio_write(pi, VALVEPIN, 0);
}

void Dockingstation::setValve(bool state) {
	gpio_write(pi, VALVEPIN, state);
}

void Dockingstation::initializeActuators() {
	ROS_INFO_STREAM("Booting up and setting in initial configuration"); 
	set_PWM_frequency(pi, PLUGACTUATORPIN, 50);
	set_PWM_range(pi, PLUGACTUATORPIN, 20000);
	set_PWM_dutycycle(pi, PLUGACTUATORPIN, 19000);
	this->setYellowLight(2, 18);
	set_PWM_frequency(pi, HATCHACTUATORPIN, 50);
	set_PWM_range(pi, HATCHACTUATORPIN, 20000);
	set_PWM_dutycycle(pi, HATCHACTUATORPIN, 18000); //For some reason the servos only work with 18-19ms signals, other than described in the library where its 0.5-2.5 us. Found out after 8h debugging.
	this->setYellowLight(2, 5);
}	

void Dockingstation::setPlugActuator(bool plugactuator) {
switch (plugactuator) {
	case 0:
	set_PWM_dutycycle(pi, PLUGACTUATORPIN, ACTUATORIN); //19000 plug fully in 
	this->setYellowLight(2, DELAYPLUG);
	break;
	case 1:
	set_PWM_dutycycle(pi, PLUGACTUATORPIN, ACTUATOROUT); //plug 18000 fully out 
	this->setYellowLight(2, DELAYPLUG);
	break;
}
}

void Dockingstation::setHatchActuator(bool hatchactuator) {
switch (hatchactuator) {
	case 0:
	set_PWM_dutycycle(pi, HATCHACTUATORPIN, ACTUATOROUT); //19000 hatch opened
	this->setYellowLight(2, DELAYHATCH);
	break;
	case 1:
	set_PWM_dutycycle(pi, HATCHACTUATORPIN, ACTUATORIN); //19000 hatch closed
	this->setYellowLight(2, DELAYHATCH);
	break;
}
}

bool Dockingstation::senseAnymal()  {
	return gpio_read(pi, INDUCTIONPIN);
}

void Dockingstation::setYellowLight(int state, int seconds) {
switch (state) {
	case 0:
		gpio_write(pi, YELLOWLIGHTPIN, 0);
		break;
	case 1:
		gpio_write(pi, YELLOWLIGHTPIN, 1);
		break;
	case 2:		
		for (int t=0; t < seconds ; t++) {
		gpio_write(pi, YELLOWLIGHTPIN, 1);
		usleep(500000);
		gpio_write(pi, YELLOWLIGHTPIN, 0);
		usleep(500000);
	}
}
}

void Dockingstation::setGreenLight(int state, int seconds) {
switch (state) {
	case 0:
		gpio_write(pi, GREENLIGHTPIN, 0);
		break;
	case 1:
		gpio_write(pi, GREENLIGHTPIN, 1);
		break;
	case 2:
		for (int t=0; t<seconds ; t++) {
		gpio_write(pi, GREENLIGHTPIN, 1);
		usleep(500000);
		gpio_write(pi, GREENLIGHTPIN, 0);
		usleep(500000);
		}	
	}
}

bool Dockingstation::allowFilling()  {
	return !gpio_read(pi, MODEPIN); //returns the value of the manual/autonomous switch for charging
}

void Dockingstation::gasFilling() {
	while (this->allowFilling() == true && this->readPressure() <= FILLINGPRESSURE) { //if the pressure difference is below 0.4 bar, open the valve	 
			this->setValve(1);
			this->setGreenLight(1,0);
			} 
		if(this->readPressure() > FILLINGPRESSURE) {
			this->setValve(0);
			this->setGreenLight(0,0);
		} 
}


bool Dockingstation::senseContact() {
	return !gpio_read(pi, CONTACTPIN);
}

bool Dockingstation::initiateDocking() {
	
	docked = this->senseContact();
	
	if (docked == true) { //check if Anymal is already docked
		ROS_INFO_STREAM("inside the check");
		return 1;
	}
	
	this->setHatchActuator(1);
	this->setPlugActuator(1);
	docked = this->senseContact();

	
	if (this->senseContact() == false && this->senseContact() == true) { //Retry docking manoever for two times
		for (int n=0; n < 2; n++) {
			ROS_INFO_STREAM("Contact cannot be established, Trying again");
			
			set_PWM_dutycycle(pi, PLUGACTUATORPIN, 18200); // plug a little bit back 
			this->setYellowLight(2, 8);
			set_PWM_dutycycle(pi, PLUGACTUATORPIN, ACTUATOROUT); // plug outside
			this->setYellowLight(2, 8);
				if (this->senseContact() == true) { //Exit loop if managed to dock
					this->setYellowLight(1, 0);
					docked = true;
					return 1;
				}	
			}
			set_PWM_dutycycle(pi, PLUGACTUATORPIN, ACTUATORIN); // plug inside
			this->setYellowLight(2, DELAYPLUG);
		}
			
	if (docked == false) {
		return 0; //send unsuccesful return
		
	} else  {
		return 1; //send successful return

	}
}

float Dockingstation::readPressure() {
	ADC adc;
	float value = adc.readAnalog(PRESSURE); 
	value =  value * 1.0/2.0 - 0.5; // this is the output in bar (or 100 kPa) relative to input pressure (for 250 Ohm)
	return(value);
}

float Dockingstation::readCurrent() {
	ADC adc;	
	float value = adc.readAnalog(CURRENT); 
// value =  value * ; // insert conversion
	return(value);
}


bool Dockingstation::update(const any_worker::WorkerEvent& event) {
	
	info.data = "Anymal not sensed";
	infoPublisher.publish(info);
	
	dockingstation_msgs::DockingstationStatus status;
	status.pressure = 10; 
	statusPublisher.publish(status);	
	
	

	if(this->senseAnymal() == 1) {
		ROS_INFO_STREAM("Anymal sensed");
		
		this->initiateDocking();
		
		
		if(docked == true) {
			ROS_INFO_STREAM("Anymal docked");
			this->setGreenLight(1, 0);
			
			while (this->senseAnymal() == 1 && this->senseContact() == true && this->allowFilling() == true && this->readPressure() <= FILLINGPRESSURE) { //if the pressure difference is below 0.4 bar, open the valve	
				//ROS_INFO_STREAM("Inside filling");  
				dockingstation_msgs::DockingstationStatus status;
				status.pressure = this->readPressure(); 
				statusPublisher.publish(status);
				this->setValve(1);
				this->setYellowLight(1,0);
				}
				
			this->setValve(0);
			this->setYellowLight(0,0); 
			this->setGreenLight(0,0);

			
			} else {
		ROS_INFO_STREAM("Failed to dock, aborting");
	
		}
	}
}

}// namespace Dockingstation
