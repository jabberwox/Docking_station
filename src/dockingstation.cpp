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
//void Dockingstation::setActuator(int pwm) {
//	digitalWrite(ACTUATORPIN,LOW);
//	softPwmCreate(ACTUATORPIN,0,200);
//	softPwmWrite(ACTUATORPIN,180);
//}
