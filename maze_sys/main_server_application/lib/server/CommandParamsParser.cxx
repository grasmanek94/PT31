//Requires AT LEAST boost 1.56 (Hey DEBIAN, what about updating once in 50 years?)
#include <string>
#include <boost/lexical_cast.hpp>

#include "CommandParamsParser.hxx"

bool ParseInput::DoParse(char f, std::string& input, size_t& current_pos, unsigned char delim)
{
	long _l;
	unsigned long _ul;
	long long _ll;
	unsigned long long _ull;
	float _f;
	double _d;
	long double _ld;
	HexTo<long> _hl;
	HexTo<unsigned long> _hul;
	HexTo<long long> _hll;
	HexTo<unsigned long long> _hull;

	if (input.size() && current_pos != std::string::npos)
	{
		switch (f)
		{
			case 's':

			data.push_back(input);
			input.clear();
			return true;

			case 'x':
			case 'X':
			case 'm':
			case 'M':
			case 'i':
			case 'l':
			case 'd':
			case 'I':
			case 'D':
			case 'L':
			case 'u':
			case 'U':
			case 'f':
			case 'F':
			case 'z':
			case 'Z':
			case 'q':
			case 'Q':
			{
				std::string use;
				current_pos = input.find(delim);
				if (current_pos == std::string::npos)
				{
					use.assign(input);
					input.clear();
				}
				else
				{
					use.assign(input.substr(0, current_pos++));
					input.erase(input.begin(), input.begin() + current_pos);
				}
				if (use.size())
				{
					switch (f)
					{
					case 'x':
						if (boost::conversion::try_lexical_convert<HexTo<long>>(use, _hl))
						{
							data.push_back(_hl.value);
							return true;
						}
						return false;
					case 'X':
						if (boost::conversion::try_lexical_convert<HexTo<unsigned long>>(use, _hul) && use[0] != '-')
						{
							data.push_back(_hul.value);
							return true;
						}
						return false;
					case 'm':
						if (boost::conversion::try_lexical_convert<HexTo<long long>>(use, _hll))
						{
							data.push_back(_hll.value);
							return true;
						}
						return false;
					case 'M':
						if (boost::conversion::try_lexical_convert<HexTo<unsigned long long>>(use, _hull) && use[0] != '-')
						{
							data.push_back(_hull.value);
							return true;
						}
						return false;
					case 'i':
					case 'l':
					case 'd':
						if (boost::conversion::try_lexical_convert<long>(use, _l))
						{
							data.push_back(_l);
							return true;
						}
						return false;
					case 'I':
					case 'D':
					case 'L':
						if (boost::conversion::try_lexical_convert<unsigned long>(use, _ul) && use[0] != '-')
						{
							data.push_back(_ul);
							return true;
						}
						return false;
					case 'u':
						if (boost::conversion::try_lexical_convert<long long>(use, _ll))
						{
							data.push_back(_ll);
							return true;
						}
						return false;
					case 'U':
						if (boost::conversion::try_lexical_convert<unsigned long long>(use, _ull) && use[0] != '-')
						{
							data.push_back(_ull);
							return true;
						}
						return false;
					case 'f':
						if (boost::conversion::try_lexical_convert<float>(use, _f))
						{
							data.push_back(_f);
							return true;
						}
						return false;
					case 'F':
						if (boost::conversion::try_lexical_convert<float>(use, _f) && use[0] != '-')
						{
							if (_f >= 0.0f)
							{
								data.push_back(_f);
								return true;
							}
						}
						return false;
					case 'z':
						if (boost::conversion::try_lexical_convert<double>(use, _d))
						{
							data.push_back(_d);
							return true;
						}
						return false;
					case 'Z':
						if (boost::conversion::try_lexical_convert<double>(use, _d) && use[0] != '-')
						{
							if (_d >= 0.0)
							{
								data.push_back(_d);
								return true;
							}
						}
						return false;
					case 'q':
						if (boost::conversion::try_lexical_convert<long double>(use, _ld))
						{
							data.push_back(_ld);
							return true;
						}
						return false;
					case 'Q':
						if (boost::conversion::try_lexical_convert<long double>(use, _ld) && use[0] != '-')
						{
							if (_ld >= 0.0)
							{
								data.push_back(_ld);
								return true;
							}
						}
						return false;
					}
				}
				return false;
			}

			case 'w':

			{
				std::string use;
				current_pos = input.find(delim);
				if (current_pos == std::string::npos)
				{
					use.assign(input);
					input.clear();
				}
				else
				{
					use.assign(input.substr(0, current_pos++));
					input.erase(input.begin(), input.begin() + current_pos);
				}
				data.push_back(use);
				return true;
			}
			return false;
		}
	}
	return false;
}

ParseInput::ParseInput(std::string _format, std::string input, unsigned char delim)
	: current_param(0), format(_format)
{
	delimeter = delim;
	ExecuteManualParse(input, delimeter);
}

ParseInput::ParseInput(unsigned char delim)
	: current_param(0), format("")
{
	delimeter = delim;
}

void ParseInput::ExecuteManualParse(std::string input, unsigned char delim)
{
	delimeter = delim;
	current_param = 0;
	current_param_get = -1;
	if (input.size() && format.size())
	{
		data.clear();
		size_t current_pos = 0;
		for (auto f : format)
		{
			if (DoParse(f, input, current_pos, delimeter))
			{
				++current_param;
			}
			else
			{
				break;
			}
		}
	}
}

unsigned long ParseInput::Good()
{
	return current_param;
}

void ParseInput::SetFormat(std::string _format, unsigned char delim)
{
	delimeter = delim;
	format.assign(_format);
}