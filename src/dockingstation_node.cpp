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
	ds.initializeServo();
	

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
			//ROS_INFO_STREAM("Anymal sensed");
			switch (ds.senseManualOperationMode()) {
				
				case 1: //Autonomy mode
					ds.setYellowLight(1); //light up yellow lamp
					ds.setServo(180); //190 is minimum 180 is maximum out, the rest is in the middle
					//ds.setServo(190); //200 for fully open
					//ds.openLatch()
					//ds.moveActuator()
					//if (ds.confirmContact() == TRUE) {
					std::cout << adc.readPressure() << std::endl;
						if(adc.readPressure() <= 0.4) { //if the pressure difference is below 0.4 bar, open the valve
							ds.setValve(1);
						} 
						if(adc.readPressure() > 0.4) {
							ds.setValve(0);
						}
					
					//}
					
				//case 1: //Manual mode (Buttons have to be pressed to start operation or teleoperation)

			}

		}

	ds.setValve(0); 
	ds.setYellowLight(0); //power off yellow lamp
	}
	
ros::shutdown();
return 0;
}

