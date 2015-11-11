//by Rafa³ 'grasmanek94' Grasman

#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include <iostream>

class Node
{
public:
	typedef std::pair<size_t, size_t> Location;
private:
	int _Cost;
	bool _Closed;
	Node* _Parent;
	std::set<Node*> _Edges;
	Location _Location;
public:

	Node(void)
		: _Cost(0), _Closed(false), _Parent(NULL)
	{}
	~Node(void)
	{}

	void SetLocation(size_t x, size_t y)
	{
		_Location.first = x;
		_Location.second = y;
	}

	Location GetLocation() const
	{
		return _Location;
	}

	bool Passable() const
	{
		return _Edges.size() > 0;
	}

	void Connect(Node* node)
	{
		if (node != this)
		{
			_Edges.insert(node);
		}
	}

	void Disconnect(Node* node)
	{
		_Edges.erase(node);
	}

	void Disconnect()
	{
		_Edges.clear();
	}

	void SetClosed(bool closed)
	{
		_Closed = closed;
	}
	void SetParent(Node* parent)
	{
		_Parent = parent;
	}

	int GetCost()
	{
		return _Cost;
	}

	void SetCost(int cost)
	{
		_Cost = cost;
	}

	bool GetClosed(void)
	{
		return _Closed;
	}

	Node* GetParent(void)
	{
		return _Parent;
	}

	std::set<Node*> GetEdges(void) const
	{
		return _Edges;
	}

	void reset(void)
	{
		_Cost = 0;
		_Closed = false;
		_Parent = NULL;
	}
};

class Grid
{
public:
	typedef std::pair<size_t, size_t> dimensions;
	enum Direction
	{
		DUP = 1,
		DDOWN = 2,
		DLEFT = 4,
		DRIGHT = 8,
		DALL = DUP | DDOWN | DLEFT | DRIGHT
	};
private:
	bool inList(Node* pNode, std::vector<Node*>* pList)
	{
		for (std::vector<Node*>::iterator i = pList->begin(); i != pList->end(); ++i)
		{
			if ((*i) == pNode)
			{
				return true;
			}
		}
		return false;
	}

	void reset(void)
	{
		for (std::set<Node*>::iterator i = mNodes.begin(); i != mNodes.end(); ++i)
		{
			(*i)->reset();
		}
	}

	std::set<Node*> mNodes;

	size_t width;
	size_t height;

	std::vector<std::vector<Node>> nodes;

	void ConnectWithNeightbours(size_t x, size_t y, Direction direction = DALL)
	{
		if (x < width && y < height)
		{
			if (x > 0 && direction | DLEFT)
			{
				nodes[x][y].Connect(&nodes[x - 1][y]);
			}

			if (x + 1 < width && direction | DRIGHT)
			{
				nodes[x][y].Connect(&nodes[x + 1][y]);
			}

			if (y > 0 && direction | DUP)
			{
				nodes[x][y].Connect(&nodes[x][y - 1]);
			}

			if (y + 1 < height && direction | DDOWN)
			{
				nodes[x][y].Connect(&nodes[x][y + 1]);
			}
		}
	}

	void DisconnectWithNeightbours(size_t x, size_t y)
	{
		if (x < width && y < height)
		{
			//for (auto& n : nodes[x][y].GetEdges())
			std::set<Node*> edges = nodes[x][y].GetEdges();
			for (std::set<Node*>::iterator it = edges.begin(); it != edges.end(); ++it)
			{
				Node* n = *it;
				n->Disconnect(&nodes[x][y]);
			}
			nodes[x][y].Disconnect();
		}
	}
public:

	Grid(size_t width, size_t height)
		: width(width), height(height)
	{
		nodes.resize(width);
		//for (auto &i : nodes)
		for (std::vector<std::vector<Node>>::iterator it = nodes.begin(); it != nodes.end(); ++it)
		{
			//i.resize(height);
			it->resize(height);
		}

		for (size_t x = 0; x < width; ++x)
		{
			for (size_t y = 0; y < height; ++y)
			{
				nodes[x][y].SetLocation(x, y);
				ConnectWithNeightbours(x, y);
				mNodes.insert(&nodes[x][y]);
			}
		}
	}

	void Set(size_t x, size_t y, bool block)
	{
		if (block)
		{
			DisconnectWithNeightbours(x, y);
		}
		else
		{
			ConnectWithNeightbours(x, y);
		}
	}

	void FromASCII(const std::string& data, const std::string& passable = "|", const std::string& wall = "=")
	{
		if (data.length() != width * height)
		{
			throw;
		}

		for (size_t x = 0; x < width; ++x)
		{
			for (size_t y = 0; y < height; ++y)
			{
				char input = data[width * y + x];
				if (input == passable[0])
				{
					ConnectWithNeightbours(x, y);
				}
				else if (input == wall[0])
				{
					DisconnectWithNeightbours(x, y);
				}
			}
		}
	}

	std::string ToASCII(char delim = '\0', char passable = '|', char wall = '=', char route = ' ', /*const std::vector<Node*> &path = std::vector<Node*>()*/ std::vector<Node*>* path = NULL) const
	{
		std::vector<std::string> builder;
		builder.resize(height);

		for (size_t y = 0; y < height; ++y)
		{
			for (size_t x = 0; x < width; ++x)
			{
				builder[y].append(1, nodes[x][y].Passable() ? passable : wall);
			}
			builder[y].append(1, delim);
		}


		//for (auto &i : path)
		for (std::vector<Node*>::iterator it = path->begin(); it != path->end(); ++it)
		{
			Node* i = *it;
			Node::Location location = i->GetLocation();
			builder[location.second][location.first] = route;
		}

		std::string output;
		//for (auto &b : builder)
		for (std::vector<std::string>::iterator it = builder.begin(); it != builder.end(); ++it)
		{
			std::string b = *it;
			output.append(b);
		}

		return output;
	}

	dimensions Size() const
	{
		return dimensions(width, height);
	}

	Node* GetNode(size_t x, size_t y)
	{
		if (x >= width || y >= height)
		{
			return NULL;
		}

		return &nodes[x][y];
	}

	bool Navigate(Node::Location start, Node::Location target, std::vector<Node*> &path)
	{
		Node* from = GetNode(start.first, start.second);
		Node* to = GetNode(target.first, target.second);

		if (from == NULL || to == NULL)
		{
			return false;
		}

		std::vector<Node*> openList;
		openList.push_back(from);
		Node* pCurrNode = NULL;

		while (!openList.empty())
		{
			//Get best node from open list (lowest F value).
			//Since we sort the list at the end of the previous loop we know
			//the front node is the best
			pCurrNode = openList.front();

			//Exit if we're are the goal
			if (pCurrNode == to)
			{
				break;
			}

			//Remove the node from the open list and place it in the closed
			openList.erase(openList.begin());
			pCurrNode->SetClosed(true); //We use a flag instead of a list for speed
										//Test all of the edge nodes from the current node

			std::set<Node*> edges = pCurrNode->GetEdges();
			for (std::set<Node*>::iterator it = edges.begin(); it != edges.end(); ++it)
				//for (auto& pEdgeNode: pCurrNode->GetEdges())//so simple in newer C++, less error prone
			{
				Node* pEdgeNode = *it;
				//If it's closed we've already analysed it
				if (!pEdgeNode->GetClosed())
				{
					if (!inList(pEdgeNode, &openList))
					{
						openList.push_back(pEdgeNode);
						pEdgeNode->SetCost(pCurrNode->GetCost() + 1);
						pEdgeNode->SetParent(pCurrNode);
					}
					else
					{
						//If this is a better node (lower G cost)
						if (pEdgeNode->GetCost() > pCurrNode->GetCost() + 1)
						{
							pEdgeNode->SetCost(pCurrNode->GetCost() + 1);
							pEdgeNode->SetParent(pCurrNode);
						}
					}
				}
			}
			//Place the lowest F cost item in the open list at the top, so we can
			//access it easily next iteration
			//std::sort(openList.begin(), openList.end(), Grid::compareNodes);//uncommented because we do not have weighted nodes
		}
		//Make sure we actually found a path
		if (to->GetParent() != NULL)
		{
			//Output the path
			//Use a stack because it is LIFO
			while (pCurrNode != NULL)
			{
				if (!pCurrNode->Passable())
				{
					pCurrNode = pCurrNode->GetParent();
					continue;
				}
				path.push_back(pCurrNode);
				pCurrNode = pCurrNode->GetParent();
			}
			std::reverse(path.begin(), path.end());
		}

		reset();

		return true;
	}
};

Grid grid(79, 24);

int main()
{
	grid.FromASCII("\
===============================================================================\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||=|||||||||||||||||||||||||||||||||||||||||=\
=||||||||||||||||||||||||||||||||||||=||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||========||||||||||||||===================\
=|||||||||||||||||||||||||||||||||||||=|||||||||||||||||||||=||||||||||||||||||\
=||||||||||||||||=======||||||||||||||||=|||||||||||||||||||=|=================\
=|||||||||||||||||||||||||||||===========|||||||||||||||||||=|===|||||||||||||=\
=||||||||||||||||||||||||||||||||||||||||=||||||||||||||||||=|||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
=|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||=\
===============================================================================\
");
	std::vector<Node*> nodes;
	grid.Navigate(Node::Location(1, 1), Node::Location(78, 5), nodes);

	//std::cout << grid.ToASCII('\n', ' ', 'X', '.') << std::endl;
	//std::cout << std::endl;
	std::cout << grid.ToASCII('\n', ' ', 'X', '.', &nodes) << std::endl;
	std::cout << nodes.size() << std::flush;
	//while (true);
	return 0;
}