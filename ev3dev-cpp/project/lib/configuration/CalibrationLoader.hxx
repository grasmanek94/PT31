#ifndef HEADER_CalibrationLoader_hxx
#define HEADER_CalibrationLoader_hxx

#include <string>
#include <map>

namespace CalibrationLoader
{
	/* Example configuration file:
		variable_name=123
		other_float_Variable=42.42
	*/
	size_t Load(const std::string& config_file, std::map<std::string, std::string>& config_data);
};

//parse (string)key["item_name"]->value to (typeof(item_name))item_name if it exists in the data map
#define ICALIBRATION_CONFIG_DATA_ENTRY_PROCESS(config_data, item_name) \
	{ \
		std::map<std::string,std::string>::iterator found(config_data.find(#item_name)); \
		if (found != config_data.end()) \
		{ \
			std::istringstream(found->second) >> item_name; \
		} \
	}

#endif