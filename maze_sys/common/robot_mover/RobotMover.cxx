#define _USE_MATH_DEFINES
#include <math.h>
//#include <pthread.h>//std::system_error exception device not connected function not implemented wtf??
#include <thread>
#include <JumpPointSearch/JPS.hxx>
#include <interfaces/IRobot.hxx>
#include "RobotMover.hxx"

RobotMover* _mover = NULL;

static IRobot* robot;
volatile static bool moving;
volatile static IRobot* thread_data_robot;
volatile static JPS::PathVector* thread_data_path;

volatile static bool moveReady;

const static double RAD2DEG = (180.0 / M_PI);

inline static float Distance(float x_1, float y_1, float x_2, float y_2)
{
	return sqrt(pow(x_2 - x_1, 2.0) + pow(y_2 - y_1, 2.0));
}

/*
//Throw protection so when thrown a variable can still be assigned a certain value as if it were normally destructed
template<typename T>
class EnsureVariableValue
{
private:
	T* var;
	T val;
public:
	EnsureVariableValue(T* var, T val)
		: var(var), val(val)
	{ }

	~EnsureVariableValue()
	{
		*var = val;
	}
};
*/
//Here happens the move magic
//need to be procedural because you can't (well it is possible but....) thread a class member
//THREAD A CLASS MEMBER, doesn't that sound kinda... INSANE?!
//You know, you have a leg, now let's move the leg, while your leg moves the bones don't ;) Get it?
//No? Close this file now and try to say the letter B wile your mouth is shut. Lemme know when you accomplish this.
//Then we will see how we will deal with class member threading.
void ThreadRobotFollowPath()
{
	if (thread_data_robot && thread_data_path && moveReady)
	{
		moving = true;
		//EnsureVariableValue<volatile bool> protector(&moving, false);
		moveReady = false;

		IRobot* robot = (IRobot*)thread_data_robot;
		JPS::PathVector& path = *(JPS::PathVector*)thread_data_path;
		thread_data_robot = NULL;
		thread_data_path = NULL;

		for (JPS::PathVector::iterator it = path.begin(); it != path.end(); )
		{
			sPosition robot_pos = robot->DriveControl()->GetPos();
			const float POSITION_PRECISION = 5.0;//cm
			float distance = Distance(robot_pos.x, robot_pos.y, it->x, it->y);
			if (distance > POSITION_PRECISION)
			{
				float angle_between_robot_and_next_point = RAD2DEG * atan2(it->y - robot_pos.y, it->x - robot_pos.x);
				float angle_to_turn = angle_between_robot_and_next_point - robot_pos.a;
				if (robot->DriveControl()->Turn(100, IDriveControl::DirectionLeft, 0.5, angle_to_turn) != IDriveControl::ExitCodeNormal)
				{
					//abort if Stop()'ed for any reason
					break;
				}
				if (robot->DriveControl()->Move(100, distance) != IDriveControl::ExitCodeNormal)
				{
					//abort if Stop()'ed for any reason
					break;
				}
			}
			else
			{
				++it;
			}
		}
		moving = false;
	}
	//return NULL;
}

RobotMover::RobotMover(IRobot* _robot)
{
	if (_mover)
	{
		throw std::exception();//only one instance of this class is allowed
	}
	_mover = this;

	robot = _robot;
}

RobotMover::~RobotMover()
{
	_mover = NULL;
}

bool RobotMover::RobotGoTo(JPS::PathVector* path)
{
	if (!moving && !moveReady)
	{
		thread_data_robot = robot;
		thread_data_path = path;
		moveReady = true;
		//return pthread_create(&thread, NULL, &ThreadRobotFollowPath, NULL) == 0;
		std::thread t(ThreadRobotFollowPath);
		t.detach();

		return true;
	}
	return false;
}

bool RobotMover::IsMoving()
{
	return moving;
}
