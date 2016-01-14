#include "ServBot.hxx"

ServBot::ServBot(size_t id)
	:	_id(id),
		online(false)
{ }

ServBot::~ServBot()
{ }

size_t ServBot::ID() const
{
	return _id;
}

ENetPeer* ServBot::GetPeer() const
{
	return peer;
}

void ServBot::SetPeer(ENetPeer* peer)
{
	this->peer = peer;
}

bool ServBot::IsOnline()
{
	return online;
}

void ServBot::SetOnline(bool isOnline)
{
	online = isOnline;
}
