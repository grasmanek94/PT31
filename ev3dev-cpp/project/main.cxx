#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <atomic>
#include <string>

#define NO_LINUX_HEADERS
#ifndef NO_LINUX_HEADERS
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#define KEY_RELEASE 0
#define KEY_PRESS   1
#define KEY_REPEAT  2
#endif

#include <ev3/EV3Calibration.hxx>
#include <ev3/EV3GripControl.hxx>
#include <ev3/EV3Sensing.hxx>
#include <ev3/EV3DriveControl.hxx>
#include <ev3/EV3Robot.hxx>

int main(int argc, char** argv)
{
	IRobot* robot = new EV3Robot();

	if (argc != 6)
	{
		std::cout << "Need 5 params: speed direction bias degrees distance" << std::endl;
		return 0;
	}

	int speed = std::stoi(argv[1]);
	int direction = std::stoi(argv[2]);
	float bias = std::stof(argv[3]);
	float degrees = std::stof(argv[4]);
	float distance = std::stof(argv[5]);

	std::cout << "S: " << speed << " D: " << direction << " B: " << bias << " *: " << degrees << " S: " << distance << std::endl;

	try
	{
		robot->GripControl()->Close();
		robot->DriveControl()->Turn(speed, (IDriveControl::Direction)direction, bias, degrees);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		robot->DriveControl()->Move(speed, distance);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		robot->DriveControl()->Turn(speed, IDriveControl::DirectionRight, 0.5, 180.0);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		robot->DriveControl()->Move(speed, distance);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		robot->DriveControl()->Turn(speed, IDriveControl::DirectionLeft, 0.5, 180.0);
		robot->GripControl()->Open();
	}
	catch (const std::exception& ex)
	{
		std::cout << "Exception occured: " << ex.what() << std::endl;
	}

	std::cout << "Exitting..." << std::endl;
	return 0;
}
