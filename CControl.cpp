////////////////////////////////////////////////////////////////
// CControl.cpp
// Member function definitions for class CControl that serially 
//     controls an embedded system
// Created Jan 29, 2021 by Laurel Kinahan 
////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "CControl.h"
#include <conio.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

Serial _com;

////////////////////////////////////////////////////////////////
// Int Com
////////////////////////////////////////////////////////////////
void CControl::init_com(int comport)
{
    string comport_str = "COM" + to_string(comport);
    _com.open("COM3");
}

////////////////////////////////////////////////////////////////
// Get Data
////////////////////////////////////////////////////////////////
bool CControl::get_data(int type, int channel, int& result)
{
    if (_com.is_open())
    {
        string tx_str;
        string rx_str;
        string type_str;
        string channel_str;
        tx_str = "G " + (type_str = to_string(type)) + " " + (channel_str = to_string(channel)) + "\n";

        // temporary storage
        char buff[2];

        while (!kbhit);
        {
            // Send TX string
            _com.write(tx_str.c_str(), tx_str.length());

            // start timeout count
            double start_time = cv::getTickCount();

            buff[0] = 0;
            // Read 1 byte and if an End Of Line then exit loop
            // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
            while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
            {
                if (_com.read(buff, 1) > 0)
                {
                    rx_str = rx_str + buff[0];
                }
            }

            vector <string> results;
            istringstream iss(rx_str);
            for (string rx_str; iss >> rx_str;)
                results.push_back(rx_str);

            result = stoi(results[MSP432PORT]);
            _com.flush();
        }
    }
}

////////////////////////////////////////////////////////////////
// Set Data
////////////////////////////////////////////////////////////////
bool CControl::set_data(int type, int channel, int val)
{
    string tx_str = "S " + to_string(type) + " " + to_string(channel) + " " + to_string(val) + "\n";
    string rx_str;

    // temporary storage
    char buff[2];

    // Send TX string
    _com.write(tx_str.c_str(), tx_str.length());
    Sleep(10); // wait for ADC conversion, etc. May not be needed?

    // start timeout count
    double start_time = cv::getTickCount();

    buff[0] = 0;
    // Read 1 byte and if an End Of Line then exit loop
    // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
    {
        if (_com.read(buff, 1) > 0)
        {
            rx_str = rx_str + buff[0];
        }
    }
    _com.flush();
}

////////////////////////////////////////////////////////////////
// Get Analog
////////////////////////////////////////////////////////////////
void CControl::get_analog()
{
    POINT position; //point position.x??

    int analog_result_X;
    int analog_result_Y;
    int analog_result_Z;
    int percent_X;
    int percent_Y;
    int percent_Z;


    // Exit loop if user presses any key
    while (!kbhit())
    {
        get_data(ANALOG, JOY_X, analog_result_X);   // get_data to send string and receive vertical joystick position
        get_data(ANALOG, JOY_Y, analog_result_Y);   // get_data to send string and receive horizontal joystick position
        get_data(ANALOG, sqrt((JOY_X*JOY_X)+(JOY_Y*JOY_Y)), analog_result_Z);
        float percent_X = (float)analog_result_X / ADC_MAX; // convert vertical adc value to %
        float percent_Y = (float)analog_result_Y / ADC_MAX; // convert horizontal adc value to %
        //float percent_Z = (float)analog_result_Z / ADC_MAX; // convert  adc value to %
        cout << "ANALOG TEST: X = " << analog_result_X << " (" << percent_X << "%)    "; // output vertical position
        cout << "ANALOG TEST: Y = " << analog_result_Y << " (" << percent_Y << "%)    \n"; // output vertical position
        //cout << "ANALOG TEST: Z = " << analog_result_Z << "(" << percent_Z << "%)\n";   // output vertical position
    }
}

////////////////////////////////////////////////////////////////
// Get Digital
////////////////////////////////////////////////////////////////
void CControl::get_digital()
{
    int digital_result;

    // Exit loop if user presses any key
    while (!kbhit())
    {
        get_data(DIGITAL, BUTTON_1, digital_result);    // get_data to send string and receive pushbutton status
        set_data(DIGITAL, RGB_BLU, !digital_result);    // set_data to send string changing state of LED when pushbutton pressed
        cout << "DIGITAL TEST: BUTTON STATE = \n" << digital_result;  // output state
    }
}

////////////////////////////////////////////////////////////////
// Get Button
////////////////////////////////////////////////////////////////
void CControl::get_button(void)
{
    int count = 0;
    int previous_digital_result;
    int current_digital_result;

    // Exit loop if user presses any key
    while (!kbhit())
    {
        get_data(DIGITAL, BUTTON_2, previous_digital_result);   // get_data to send string and receive pushbutton status
        set_data(DIGITAL, RGB_GRN, !previous_digital_result);   // set_data to send string changing state of LED when pushbutton pressed
        double start_time = 0;
        if (!previous_digital_result)   // If pushbutton status is high, aka not pressed
        {
            start_time = cv::getTickCount();    // start timer
        }
        // debounce pushbutton
        while ((cv::getTickCount() - start_time) / cv::getTickFrequency() < 0.2) // while button is pressed within 0.5 seconds
        {
            get_data(DIGITAL, BUTTON_2, current_digital_result); // current pushbutton status
            if (current_digital_result != previous_digital_result)
            {
                count++;    //increment count
                cout << "\nBUTTON TEST:" << count; // output count number
                break;
            }
        }
    }
}

////////////////////////////////////////////////////////////////
// Get Servo
////////////////////////////////////////////////////////////////
void CControl::get_servo()
{
    while (!kbhit())
    {
        for (int angle = SERVO_MAX; angle >= SERVO_MIN; angle --)
        {
            set_data(SERVO, SERVO_PORT, angle);
            cout << "SERVO TEST: ANGLE = \n" << angle;
        }
        for (int angle = SERVO_MIN; angle <= SERVO_MAX; angle ++)
        {
            set_data(SERVO, SERVO_PORT, angle);
            cout << "SERVO TEST: angle = \n" << angle;
        }
    }
}
