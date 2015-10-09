#include <string>
#include <fstream>
#include <string>
#include <sstream>

#include "CalibrationLoader.hxx"

size_t CalibrationLoader::Load(const std::string& config_file, std::map<std::string, std::string>& config_data)
{
	std::ifstream config(config_file);
	/* Example config file content:
		grip_close_amount=2250
		move_units_per_cm=29.4117
		turn_left_error_correction=0.96
		turn_right_error_correction=1.02
	*/

	if (config.is_open())
	{
		//put the data into the map with the pattern key=value
		std::string line;
		while (std::getline(config, line))
		{
			std::istringstream is_line(line);
			std::string key;
			if (std::getline(is_line, key, '='))
			{
				std::string value;
				if (std::getline(is_line, value))
				{
					config_data[key] = value;
				}
			}
		}
		config.close();

		return config_data.size();
	}

	return 0;
}