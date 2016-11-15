#include <ros/ros.h>
#include <wiringPi.h>


class Dockingstation {

	public:
	void setupGPIO();
	void setValve(bool s);

};
