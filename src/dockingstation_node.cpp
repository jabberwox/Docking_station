#include "dockingstation/dockingstation.hpp"
#include "dockingstation/adc.hpp"

int main(int argc, char** argv) {
  // Announce this program to the ROS master as a "node" called "dockingstation_node"
  ros::init(argc, argv, "dockingstation_node");
  // Start the node resource managers (communication, time, etc)
  ros::start();
  
  ROS_INFO_STREAM("Dockingstation started!");
  Dockingstation ds;
  ADC adc;
 
  ds.setupGPIO();
//  ds.setServo(175);
//  ds.setServo(190);
  
  while (ros::ok()) {

	if(adc.readPressure() <= 0.05) {
		ds.setValve(1);
	}
	else
	ds.setValve(0);
}
  ros::shutdown();
  return 0;
}

