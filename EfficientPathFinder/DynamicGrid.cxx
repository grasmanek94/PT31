#include <vector>
#include <string>

#include "DynamicGrid.hxx"
#include "JPS.hxx"
#include "StringSplitter.hxx"

DynamicGrid::DynamicGrid(size_t width, size_t height)
	: width(width), height(height)
{
	SetMinimumPos(width - 1, height - 1);
}

bool DynamicGrid::operator()(size_t x, size_t y) const
{
	if (x >= width || y >= height)
	{
		return false;
	}

	return !environment[x][y];//if 0 then walkable
}

bool DynamicGrid::SetMinimumPos(size_t x, size_t y)
{
	bool size_changed = false;

	if (x >= width || environment.size() != width)
	{
		width = x + 1;
		environment.resize(width);
		size_changed = true;
	}

	if (y >= height || environment[0].size() != height)
	{
		height = y + 1;
		size_changed = true;
	}

	if (size_changed)
	{
		for (std::vector<std::vector<bool>>::iterator it = environment.begin(); it != environment.end(); ++it)
		{
			it->resize(height);
		}
	}

	return size_changed;
}

size_t DynamicGrid::GetWidth() const
{
	return width;
}

size_t DynamicGrid::GetHeight() const
{
	return height;
}

JPS::Position DynamicGrid::GetSize() const
{
	return JPS::Pos(width, height);
}

bool DynamicGrid::SetPassable(JPS::Position pos, bool passable)
{
	bool size_changed = SetMinimumPos(pos.x, pos.y);
	environment[pos.x][pos.y] = !passable;
	return size_changed;
}

void DynamicGrid::MakeWholeGridImpassable()
{
	for (std::vector<std::vector<bool>>::iterator it = environment.begin(); it != environment.end(); ++it)
	{
		std::fill(it->begin(), it->end(), true);
	}
}

bool DynamicGrid::JumpNavigate(JPS::Position start, JPS::Position target, JPS::PathVector& path) const
{
	return JPS::findPath(path, *this, start.x, start.y, target.x, target.y, 0, 0);
}

bool DynamicGrid::FullNavigate(JPS::Position start, JPS::Position target, JPS::PathVector& path) const
{
	return JPS::findPath(path, *this, start.x, start.y, target.x, target.y, 1, 0);
}

void DynamicGrid::FromASCII(const std::string& map_data, char wall)
{
	std::vector<std::string> map;
	Splitter::split(map_data, '\n', map);

	size_t w = map[0].size();
	size_t h = map.size();
	SetMinimumPos(w - 1, h - 1);

	MakeWholeGridImpassable();
	for (size_t y = 0; y < h; ++y)
	{
		for (size_t x = 0; x < w; ++x)
		{
			environment[x][y] = map[y][x] == wall;
		}
	}
}

std::string DynamicGrid::ToASCII(JPS::PathVector& path) const
{
	std::vector<std::string> map;
	map.resize(GetHeight());
	for (std::vector<std::string>::iterator it = map.begin(); it != map.end(); ++it)
	{
		it->assign(GetWidth(), ' ');
	}

	for (size_t y = 0; y < GetHeight(); ++y)
	{
		for (size_t x = 0; x < GetWidth(); ++x)
		{
			map[y][x] = environment[x][y] ? '|' : ' ';
		}
	}

	for (JPS::PathVector::iterator it = path.begin(); it != path.end(); ++it)
	{
		//size_t y = it->y;
		//size_t x = it->x;
		//if (x < width && y < height)
		//{
		map[it->y][it->x] = '#';
		//}
	}

	std::string output;

	for (std::vector<std::string>::iterator it = map.begin(); it != map.end(); ++it)
	{
		output += *it + "\n";
	}

	return output;
}