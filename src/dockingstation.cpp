#include "dockingstation/dockingstation.hpp"

void Dockingstation::setValve(bool on) {
  digitalWrite(13, on);
}

void Dockingstation::setupGPIO(){
  wiringPiSetupSys();
}
