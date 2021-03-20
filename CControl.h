////////////////////////////////////////////////////////////////
// CControl.h
// Definintion of class CControl
// Member function are defined in CControl.cpp
// Created Jan 29, 2021 by Laurel Kinahan 
////////////////////////////////////////////////////////////////
#pragma once

#include "Serial.h"

#define MSP432PORT 3
#define ADC_MAX 1024
#define JOY_X 2
#define JOY_Y 26
#define BUTTON_1 33
#define BUTTON_2 32
#define RGB_BLU 37
#define RGB_GRN 38
#define SERVO_PORT 0
#define SERVO_MIN 0
#define SERVO_MAX 181


/**
*
* @brief Used to serially control an Embedded System MSP432
*
* Class CControl initializes the comport, sends and collects data form the MSP432
* class also debounces pushbutton and scales analog input to a percentages of its range
*
* @author Laurel Kinahan
*
*/

class CControl
{
private:
	Serial _com; ///< allows for serial port communications

	/**
	* enum for serial communication type
	*/
	enum TYPE
	{
		DIGITAL = 0, ///< 0
		ANALOG, ///< 1
		SERVO ///< 2
	};

public:
	/*
	* @brief initialization serial port to com port in parameter
	*
	* @param comport The com port number to comment to
	* @return none
	*/
	void init_com(int comport);

	/*
	* @brief Gets data from channel parameter and returns data to result parameter address
	*
	* @param type Type of serial communication
	* @param Channel Desired channel on Embedded System
	* @param result Referenced address for returned result
	* @return true if successful, false if failed or timed out
	*/
	bool get_data(int type, int channel, int &result);

	/*
	* @brief Sets data to channel parameter on Embedded System
	*
	* @param type Type of serial communication
	* @param Channel Desired channel on Embedded System
	* @return true if successful, false if failed or timed out
	*/
	bool set_data(int type, int channel, int val);

	/*
	* @brief Gets data from analog channel and returns data to result parameter address as a % of range
	*
	* @param type Type of channel will always be ANALOG
	* @param Channel Desired channel number on Embedded System
	* @param raw_data Raw analog data from receive buffer
	* @param result Referenced address for returned result
	* @return percent value of A0 result
	*/
	void get_analog(void);

	/*
	* @brief Gets data from digital channel and returns data to result parameter address as a % of range
	*
	* @param type Type of channel will always be ANALOG
	* @param Channel Desired channel number on Embedded System
	* @param raw_data Raw analog data from receive buffer
	* @param result Referenced address for returned result
	* @return percent value of A0 result
	*/
	void get_digital(void);

	/*
	* @brief Gets button status and returns to result address
	*
	* @param type Type of serial communication
	* @param Channel Desired channel on Embedded System
	* @param result Referenced address for returned result
	* @return true if successful, false if failed or timed out
	*/
	void get_button(void);

	/**
	* @brief Enables access to the servo's angle
	*
	* @param none
	* @return none
	*/
	void get_servo(void);

	/**
	*enum for button state
	*/
	enum BUTTON
	{
		PRESSED = 0, ///< 0
		NOT_PRESSED ///< 1
	};
};
