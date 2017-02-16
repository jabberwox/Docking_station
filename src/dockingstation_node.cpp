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
	
	while (ros::ok()) {
					

		while(ds.senseAnymal() == TRUE) {
			bool docking = ds.initiateDocking();
			if (docking == true) {
				
					//ds.
			}
		}
	ROS_INFO_STREAM("Anymal not sensed"); 
	}
	
ros::shutdown();
return 0;
}

