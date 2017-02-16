#include "dockingstation/dockingstation.hpp"
#include "dockingstation/adc.hpp"

using namespace std;

int main(int argc, char** argv) {


	// Announce this program to the ROS master as a "node" called "dockingstation_node"
	ros::init(argc, argv, "dockingstation_node");
	// Start the node resource managers (communication, time, etc)
	ros::start();
  
	ROS_INFO_STREAM("Dockingstation started!");

	Dockingstation ds;
	ADC adc;
	
	ds.setupGPIO();
	ds.initializeActuators();
	ds.setYellowLight(0); //power off yellow lamp
    ds.setGreenLight(0); 
    ds.setPlugActuator(0); 	
	ds.setHatchActuator(0);
//	while (bool test = true) {
//		ds.moveActuator(2);
//		float value = adc.readAnalog(2);
//		ROS_INFO_STREAM(value);
		//if (value = 2.5 ) {
		//	test = false;
		//	ds.moveActuator(0);
		//	break;
		//}
//	}
	
	while (ros::ok()) {
					

		while(ds.senseAnymal() == TRUE) {
//			ROS_INFO_STREAM("Anymal sensed");
			if(ds.allowFilling() == true) { //|| button pressed || overriden by external
			//Autonomy mode
					ds.setYellowLight(2); //light up yellow lamp	
		//			ds.setGreenLight(2);
		//			ds.setHatchActuator(1); 
		//			delay(5000);
		//			ds.setPlugActuator(1); 
		//			delay(5000);
		//			ds.setYellowLight(0); //light up yellow lamp	
					//if (ds.confirmContact() == TRUE) {


						//}
					//break;
					//}
				//case 1: //Manual mode (Buttons have to be pressed to start operation or teleoperation)
			}
		}
	ROS_INFO_STREAM("Anymal not sensed");
//	ds.setValve(0); 
//	ds.setYellowLight(0); //power off yellow lamp
//  ds.setGreenLight(0); 
    
    
	}
	
ros::shutdown();
return 0;
}

