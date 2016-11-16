#include "dockingstation/dockingstation.hpp"


void Dockingstation::setValve(bool on) {
  digitalWrite(VALVEPIN, on);
}

void Dockingstation::setupGPIO(){
  wiringPiSetupSys();
}

void Dockingstation::setServo(int pwm) {
	digitalWrite(SERVOPIN,LOW);
	softPwmCreate(SERVOPIN,0,200);
	softPwmWrite(SERVOPIN,pwm);
}

//void Dockingstation::setActuator(int pwm) {
//	digitalWrite(ACTUATORPIN,LOW);
//	softPwmCreate(ACTUATORPIN,0,200);
//	softPwmWrite(ACTUATORPIN,180);
//}
