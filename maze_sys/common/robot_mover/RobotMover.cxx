#define _USE_MATH_DEFINES
#include <math.h>
//#include <pthread.h>
#include <thread>
#include <JumpPointSearch/JPS.hxx>
#include <interfaces/IRobot.hxx>
#include "RobotMover.hxx"
#include <iostream>

namespace RobotMover
{
	//You might be tempted to change this to bool moving... because.. 
	//We fully agree that this should not be a pointer
	//But if you change it to bool moving, the program crashes.
	bool* moving = NULL;
	IRobot* thread_data_robot;
	JPS::PathVector* thread_data_path;
	bool* moveReady = NULL;

	bool IsMoving()
	{
		return *moving;
	}

	inline float Distance(float x_1, float y_1, float x_2, float y_2)
	{
		return sqrt(pow(x_2 - x_1, 2.0f) + pow(y_2 - y_1, 2.0f));
	}

	void ThreadRobotFollowPath()
	{
		if (thread_data_robot && thread_data_path && *moveReady && !IsMoving())
		{
			*moving = true;
			*moveReady = false;
			IRobot* robot = (IRobot*)thread_data_robot;
			JPS::PathVector* path = (JPS::PathVector*)thread_data_path;
			thread_data_robot = NULL;
			thread_data_path = NULL;
			for (JPS::PathVector::iterator it = path->begin(); it != path->end(); )
			{
				sPosition robot_pos = robot->DriveControl()->GetPos();
				const float POSITION_PRECISION = 4.9;//cm
				float distance = Distance(robot_pos.x, robot_pos.y, it->x, it->y);
				if (distance > POSITION_PRECISION)
				{
					const static float RAD2DEG = (180.0 / M_PI);
					float angle_between_robot_and_next_point = RAD2DEG * atan2(it->y - robot_pos.y, it->x - robot_pos.x);
					float angle_to_turn = robot_pos.a - angle_between_robot_and_next_point;

					if (angle_to_turn > 180.0)
					{
						angle_to_turn -= 360.0;
					}
					else if (angle_to_turn < -180.0)
					{
						angle_to_turn += 360.0;
					}

					if (robot->DriveControl()->Turn(90, IDriveControl::DirectionLeft, 0.5, angle_to_turn) != IDriveControl::ExitCodeNormal)
					{
						//abort if Stop()'ed for any reason
						break;
					}
					if (robot->DriveControl()->Move(65, distance) != IDriveControl::ExitCodeNormal)
					{
						//abort if Stop()'ed for any reason
						break;
					}
					//robot->DriveControl()->SetPos(sPosition{ (float)it->x, (float)it->y, 0.0, 0.0 });
				}
				else
				{
					++it;
				}
			}
			*moving = false;
			delete path;
		}
		if (thread_data_path)
		{
			delete thread_data_path;
			thread_data_path = NULL;
		}
		//return NULL;
	}

	bool Follow(IRobot* robot, const JPS::PathVector& path)
	{
		if (!moving)
		{
			moving = new bool;
			moveReady = new bool;
		}

		if (!IsMoving() && !*moveReady)
		{
			thread_data_robot = robot;
			thread_data_path = new JPS::PathVector(path);
			*moveReady = true;
			//return pthread_create(&thread, NULL, &ThreadRobotFollowPath, NULL) == 0;
			std::thread t(ThreadRobotFollowPath);
			t.detach();
			return true;
		}
		return false;
	}
}