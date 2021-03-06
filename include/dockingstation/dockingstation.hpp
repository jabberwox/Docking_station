/*!
 * @file	dockingstation.hpp
 * @author	Hendrik Kolvenbach
 * @date	February, 2017
 */

#include <wiringPi.h>
#include <softPwm.h>
#include <ros/ros.h>
#include "any_node/any_node.hpp"
#include "dockingstation/adc.hpp"

//Pin definition for GPIO, use BCM number
//
//+-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+
//| BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
//+-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
//|     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
//|   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5V      |     |     |
//|   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
//|   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT5 | TxD     | 15  | 14  |
//|     |     |      0v |      |   |  9 || 10 | 1 | ALT5 | RxD     | 16  | 15  |
//|  17 |   0 | GPIO. 0 |   IN | 0 | 11 || 12 | 0 | ALT0 | GPIO. 1 | 1   | 18  |
//|  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
//|  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
//|     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
//|  10 |  12 |    MOSI | ALT0 | 0 | 19 || 20 |   |      | 0v      |     |     |
//|   9 |  13 |    MISO | ALT0 | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
//|  11 |  14 |    SCLK | ALT0 | 0 | 23 || 24 | 1 | OUT  | CE0     | 10  | 8   |
//|     |     |      0v |      |   | 25 || 26 | 0 | IN   | CE1     | 11  | 7   |
//|   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
//|   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
//|   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
//|  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
//|  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 0 | IN   | GPIO.27 | 27  | 16  |
//|  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
//|     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
//+-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
//| BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
//+-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+

#define VALVEPIN 13 //Electromechanical valve 
#define PLUGACTUATORPIN 19 //Plug linear actuator
#define HATCHACTUATORPIN 6 //Hatch linear actuator
#define INDUCTIONPIN 21 //Induction sensor 
#define MODEPIN 20 //Mode selection switch 
#define BUTTONPIN 16 //Button
#define YELLOWLIGHTPIN 8 //Yellow signal light
#define GREENLIGHTPIN 7 //Yellow signal light
#define CONTACTPIN 12 //Successful docking to ANYmal

#define BLINKFREQUENCY 10 //in HZ
#define FILLINGPRESSURE 0.2 //defines the pressure difference between transducer inlet and outlet

namespace dockingstation {

class Dockingstation : public  any_node::Node { 
	public:

	Dockingstation() = delete; // constructor needs to take a shared_ptr to a ros::Nodehandle instance.
	Dockingstation(any_node::Node::NodeHandlePtr nh);
	
	virtual ~Dockingstation();
	virtual void init();
	virtual void cleanup();
	virtual bool update(const any_worker::WorkerEvent& event);
	
	bool senseAnymal(); //Reads the Induction Sensor (XS130B3PAL2). Returns "true" for metal detection.
    bool initiateDocking();
    void gasFilling();
    void monitorCurrent();
    
	private:
	void setupGPIO(); //Initialising GPIO pins
	void initializeActuators();
	void setPlugActuator(bool plugactuator); //Moves the plug actuator 
	void setHatchActuator(bool hatchactuator); //Moves the hatch actuator
	bool senseContact();
	bool allowFilling(); //Reads the value from the gas filling operation switch. Returns "true" for manual mode.
	void setValve(bool s); //Sets the electromechanical valve (Festo CP18-M1H-3GL-QS-10). Send "true" for open.
	void setYellowLight(int state); //Statemachine for the yellow light 0: yellow off, 1: yellow on, 2: yellow blink
	void setGreenLight(int state); //Statemachine for the red light 0: red off, 1: red on, 2: red blink
	
};
}// namespace Dockingstation
