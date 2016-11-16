#include "dockingstation/dockingstation.hpp"
#include "dockingstation/adc.hpp"

// Standard C++ entry point
int main(int argc, char** argv) {
  // Announce this program to the ROS master as a "node" called "dockingstation_node"
  ros::init(argc, argv, "dockingstation_node");
  // Start the node resource managers (communication, time, etc)
  ros::start();
  ROS_INFO_STREAM("Dockingstation started!");
  
  
  Dockingstation ds;
  ADC adc;
  
  
  ds.setupGPIO();
  ds.setValve(1); //set Valve bool 1 or 0
  delay(200);
  ds.setValve(0);
  
//  while(TRUE) {
//  int test;
//  std::cin >> test; 
//  ds.setServo(test);
//}
//  while(1) {
//    adc.readPressure(); // read 0 = pressure transduce, 1= FSR 1, 2=FSR 2
    printf("Pressure: %f bar \n", adc.readPressure());
//  delay(200);
//}
  // Process ROS callbacks until receiving a SIGINT (ctrl-c)
  ros::spin();
  // Stop the node's resources
  ros::shutdown();
  // Exit tranquilly
  return 0;
}
