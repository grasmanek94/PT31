#ifndef HEADER_EV3Calibration_hxx
#define HEADER_EV3Calibration_hxx

#include <interfaces/ICalibration.hxx>

class EV3Calibration : public ICalibration
{
private:
	int grip_close_amount;
	float move_units_per_cm;
	float turn_left_error_correction;
	float turn_right_error_correction;

public:
	EV3Calibration();
	virtual ~EV3Calibration();
	void Reload();

	int GetGripCloseAmount() const;
	float GetMoveUnitsPerCentimeter() const;
	float GetLeftTurnCorrection() const;
	float GetRightTurnCorrection() const;

	bool Available() const;
};

#endif
