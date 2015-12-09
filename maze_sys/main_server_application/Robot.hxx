#ifndef ROBOT_H
#define ROBOT_H

class Robot {
	public:
		Robot(int addres);
		int GetID();
		ENetPeer GetPeer();
	
	private:
		int id;
		ENetPeer peer;
}

#endif
