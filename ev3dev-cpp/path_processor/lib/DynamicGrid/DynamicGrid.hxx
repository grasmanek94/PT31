#ifndef DYNAMIC_GRAPH_H
#define DYNAMIC_GRAPH_H

#include <vector>
#include <string>
#include <JumpPointSearch/JPS.hxx>

class DynamicGrid
{

private:
	std::vector<std::vector<bool>> environment;

	size_t width;
	size_t height;

public:
	DynamicGrid(size_t width, size_t height);
	DynamicGrid(const std::string& map_data, char wall = '#');

	//10000x10000 ~= 12.30 MB of memory
	//returns if position is walkable
	bool operator()(size_t x, size_t y) const;

	//set the minimum accessible position, eg (0, 0)
	//returns true if size of grid got enlarged
	bool SetMinimumPos(size_t x, size_t y);

	size_t GetWidth() const;
	size_t GetHeight() const;
	JPS::Position GetSize() const;

	//returns true if size of grid got enlarged
	bool SetPassable(JPS::Position pos, bool passable);
	void MakeWholeGridImpassable();

	bool JumpNavigate(JPS::Position start, JPS::Position target, JPS::PathVector& path) const;
	bool FullNavigate(JPS::Position start, JPS::Position target, JPS::PathVector& path) const;

	void FromASCII(const std::string& map_data, char wall = '#');

	std::string ToASCII(JPS::PathVector& path
		//, char wall = '#'/*-79 178*/, char solution = '×'/*-98 158*/
		) const;

};

#endif