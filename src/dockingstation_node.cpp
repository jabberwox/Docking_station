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
			switch (ds.senseManualOperationMode()) {
				
				case 1: //Autonomy mode
					ds.setYellowLight(2); //light up yellow lamp	
					ds.setGreenLight(2);
					ds.setHatchActuator(1); 
					delay(5000);
					ds.setPlugActuator(1); 
					delay(5000);
					ds.setYellowLight(1); //light up yellow lamp	
					ds.setGreenLight(1);
					//if (ds.confirmContact() == TRUE) {
					std::cout << adc.readPressure() << std::endl;
						if(adc.readPressure() <= 0.4) { //if the pressure difference is below 0.4 bar, open the valve
							ds.setValve(1);
						} 
						if(adc.readPressure() > 0.4) {
							ds.setValve(0);
						}
					//break;
					//}
				//case 1: //Manual mode (Buttons have to be pressed to start operation or teleoperation)
			}
		}
	ROS_INFO_STREAM("Anymal not sensed");
	ds.setValve(0); 
	ds.setYellowLight(0); //power off yellow lamp
    ds.setGreenLight(0); 
    
    
	}
	
ros::shutdown();
return 0;
}

