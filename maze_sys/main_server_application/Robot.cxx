Robot::Robot(int address, ENetPeer peer)
{
	address = address;
	peer = peer;
}

int Robot::GetID()
{
	return id;
}

ENetPeer Robot::GetPeer()
{
	return peer;
}
