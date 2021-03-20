////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated December 30, 2020
// Modified by Laurel Kinahan Jan 29, 2021
////////////////////////////////////////////////////////////////
//#include "stdafx.h"

//#include "Serial.h" 
#include "CControlPi.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>



using namespace std;

void print_menu(void);


void main()
{
    CControl controller;
    controller.init_com(3);
    string cmd;

    while (cmd != "0")
    {
        print_menu();
        cin >> cmd;

        if (cmd == "1") // Command for   
            controller.get_analog();
        else if (cmd == "2") // Command for 
            controller.get_digital();
        else if (cmd == "3") // Command for 
            controller.get_button();
        else if (cmd == "4") // Command for 
            controller.get_servo();
        else if (cmd == "0")
            break;
        else
            cout << "\nInvalid command, try again...\n"; 
    }
}


void print_menu()
{
    cout << "\n**************************************";
    cout << "\n*   ELEX4618 Lab 3 Embedded Control  *";
    cout << "\n* Select a test operation to perform *";
    cout << "\n**************************************";
    cout << "\n(1) Analog Test - test joystick outputs";
    cout << "\n(2) Digital I/O Test - test LED status";
    cout << "\n(3) Pushbutton Test - test counter debounce";
    cout << "\n(4) Servo Test - test servo position";
    cout << "\n(0) Exit";
    cout << "\nCMD> ";
}