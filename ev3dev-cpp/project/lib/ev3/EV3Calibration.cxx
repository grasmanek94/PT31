#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include <configuration/CalibrationLoader.hxx>

#include "EV3Calibration.hxx"

void EV3Calibration::Reload()
{
	std::map<std::string, std::string> config_data;
	if (CalibrationLoader::Load("./ev3_calibration.txt", config_data))
	{
		ICALIBRATION_CONFIG_DATA_ENTRY_PROCESS(config_data, grip_close_amount);
		ICALIBRATION_CONFIG_DATA_ENTRY_PROCESS(config_data, move_units_per_cm);
		ICALIBRATION_CONFIG_DATA_ENTRY_PROCESS(config_data, turn_left_error_correction);
		ICALIBRATION_CONFIG_DATA_ENTRY_PROCESS(config_data, turn_right_error_correction);
	}
}

EV3Calibration::EV3Calibration()
	//defaults
	: grip_close_amount(2250),			//for closing the grip
	move_units_per_cm(500.0 / 17.0),	//moved units / moved centimeters
	turn_left_error_correction(0.96),	//Turn(67, DirectionLeft,  0.5, 90.0)
	turn_right_error_correction(1.02)	//Turn(67, DirectionRight, 0.5, 90.0)
{
	Reload();
}

int EV3Calibration::GetGripCloseAmount() const
{
	return grip_close_amount;
}

float EV3Calibration::GetMoveUnitsPerCentimeter() const
{
	return move_units_per_cm;
}

float EV3Calibration::GetLeftTurnCorrection() const
{
	return turn_left_error_correction;
}

float EV3Calibration::GetRightTurnCorrection() const
{
	return turn_right_error_correction;
}

bool EV3Calibration::Available() const
{
	return true;
}
