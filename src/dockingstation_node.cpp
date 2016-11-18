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
  
	while (ros::ok()) {
		
		switch (ds.senseManualOperationMode()) {
		
		case 0: //Autonomy mode
		
			if(ds.senseAnymal() == TRUE) {
				ROS_INFO_STREAM("Anymal sensed");
				if(adc.readPressure() <= 0.05) {
					ds.setValve(1);
				}
				else
					ds.setValve(0);
			}
			else
				ds.setValve(0);

			
		
		case 1: //Manual mode (Buttons have to be pressed to start operation or teleoperation)
		
		
		
		default: //
			break;
		}
  
}
ros::shutdown();
return 0;
}

