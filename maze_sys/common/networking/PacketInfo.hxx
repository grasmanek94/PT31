#ifndef PACKETINFO_H
#define PACKETINFO_H
#include <vector>
#include "../JumpPointSearch/JPS.hxx"

//PacketStructure: <ServerPacketType, Data>
enum ServerPacketType
{
	//<*>
	SPT_Unknown,

	//<id>
	SPT_Identify,

	//<null>
	SPT_IdentifyAcknowledged,

	//<id upper limit>
	SPT_IdentifyDenied,

	//<WhichPacketType>
	SPT_GotCorruptPacket,

	//<std::vector<JPS::Position>>
	SPT_FollowPath,

	SPT_NUM_PACKET_TYPES
};

#endif