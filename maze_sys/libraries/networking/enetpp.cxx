#include <enet/enetpp.hxx>

NetworkBase::NetworkBase(enet_uint16 port)
	: member(nullptr)
{
	initialisation_code = enet_initialize();
	atexit(enet_deinitialize);

	address.port = port;
}

NetworkBase::~NetworkBase()
{
	if (member != nullptr)
	{
		enet_host_destroy(member);
	}
}

void NetworkBase::SetHost(std::string hostname, unsigned short port)
{
	enet_address_set_host(&address, hostname.c_str());
	address.port = port;
}

const ENetAddress NetworkBase::Address()
{
	return address;
}

const ENetEvent NetworkBase::Event()
{
	return event;
}

int NetworkBase::GetInitCode()
{
	return initialisation_code;
}

bool NetworkBase::Good()
{
	return member != nullptr;
}

int NetworkBase::Pull(enet_uint32 timeout)
{
	return enet_host_service(member, &event, 0);
}

int NetworkBase::Send(ENetPeer* peer, const void* data, size_t bytes, _ENetPacketFlag flags)
{
	return enet_peer_send(peer, 0, enet_packet_create(data, bytes, flags));
}

int NetworkBase::Send(ENetPeer* peer, const packet_vec& data, _ENetPacketFlag flags)
{
	return enet_peer_send(peer, 0, enet_packet_create(data.data(), data.size(), flags));
}

int NetworkBase::Send(ENetPeer* peer, const PacketData& data, _ENetPacketFlag flags)
{
	return enet_peer_send(peer, 0, enet_packet_create(data.Serialize(), data.size(), flags));
}

std::vector<uint8_t> NetworkBase::GetPacketData(ENetPacket* p) const
{
	std::vector<uint8_t> vec((uint8_t*)p->data, (uint8_t*)(p->data + p->dataLength));
	enet_packet_destroy(p);
	return vec;
}

bool NetworkBase::GetPacketData(ENetPacket* p, PacketData& data) const
{
	if (p && p->data)
	{
		data.Deserialize(p->data, p->dataLength);
		enet_packet_destroy(p);
		return true;
	}
	return false;
}

NetworkServer::NetworkServer(enet_uint16 port)
	: NetworkBase(port)
{
	address.host = ENET_HOST_ANY;
}

bool NetworkServer::Create(size_t max_connections)
{
	if (member == nullptr)
	{
		member = enet_host_create(&address	/* the address to bind the server host to							*/,
			max_connections					/* allow up to max_connections clients and/or outgoing connections	*/,
			1								/* allow up to 1 channels to be used, 0								*/,
			0								/* assume any amount of incoming bandwidth							*/,
			0								/* assume any amount of outgoing bandwidth							*/);
		return true;
	}
	return false;
}

void NetworkServer::Broadcast(const void* data, size_t bytes, _ENetPacketFlag flags)
{
	enet_host_broadcast(member, 0, enet_packet_create(data, bytes, flags));
}

void NetworkServer::Broadcast(const packet_vec& data, _ENetPacketFlag flags)
{
	enet_host_broadcast(member, 0, enet_packet_create(data.data(), data.size(), flags));
}

void NetworkServer::Broadcast(const PacketData& data, _ENetPacketFlag flags)
{
	enet_host_broadcast(member, 0, enet_packet_create(data.Serialize(), data.size(), flags));
}

NetworkClient::NetworkClient(enet_uint16 port)
	:	NetworkBase(port),
		peer(nullptr)
{
	enet_address_set_host(&address, "127.0.0.1");
}

NetworkClient::~NetworkClient()
{
	if (peer != nullptr)
	{
		enet_peer_reset(peer);
	}
}

ENetPeer * NetworkClient::Connect(std::string hostname, unsigned short port)
{
	if (peer != nullptr)
	{
		enet_peer_reset(peer);
	}
	SetHost(hostname, port);
	peer = enet_host_connect(member, &address, 1, 0);
	return peer;
}

bool NetworkClient::Create()
{
	if (member == nullptr)
	{
		member = enet_host_create(nullptr,
			1								/* allow up to max_connections clients and/or outgoing connections	*/,
			1								/* allow up to 1 channels to be used, 0								*/,
			0								/* assume any amount of incoming bandwidth							*/,
			0								/* assume any amount of outgoing bandwidth							*/);
		return true;
	}
	return false;
}

int NetworkClient::Send(const void* data, size_t bytes, _ENetPacketFlag flags)
{
	return NetworkBase::Send(peer, data, bytes, flags);
}

int NetworkClient::Send(const packet_vec& data, _ENetPacketFlag flags)
{
	return NetworkBase::Send(peer, data.data(), data.size(), flags);
}

int NetworkClient::Send(const PacketData& data, _ENetPacketFlag flags)
{
	return NetworkBase::Send(peer, data.Serialize(), data.size(), flags);
}
