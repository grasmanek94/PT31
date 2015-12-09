Robot::Robot(ENetPeer peer)
{
	peer = peer;
}

ENetPeer Robot::GetPeer()
{
	return peer;
}
