#ifndef SPLITTER_STRING_SPLITTER_H
#define SPLITTER_STRING_SPLITTER_H
#include <vector>
#include <string>

namespace Splitter
{
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
	std::vector<std::string> split(const std::string &s, char delim);
};
#endif