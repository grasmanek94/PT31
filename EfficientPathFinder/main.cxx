#include <iostream>
#include <string>

#include "DynamicGrid.hxx"
#include "MazeMap.hxx"

DynamicGrid grid(1, 1);//let's assume each 1x1 is 1x1cm, we have 2.5x2.5 meters here with 1cm precision, in reality we will probably use smaller precision (more than 1cm)

int main(int argc, char **argv)
{
	JPS::PathVector path;

	grid.FromASCII(mazeMap);

	JPS::Position start(JPS::Pos(1, 1));
	JPS::Position target(JPS::Pos(1018, 978));
	bool found = grid.FullNavigate(start, target, path);
	if(found)
	{
		std::cout << grid.ToASCII(path) << std::endl;
	}
	else
	{
		path.push_back(start);
		path.push_back(target);
		std::cout << grid.ToASCII(path) << std::endl;
		std::cout << "Path * NOT * found!" << std::endl;
	}

	return 0;
}
