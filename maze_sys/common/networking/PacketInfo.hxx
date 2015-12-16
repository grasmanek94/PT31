#ifndef PACKETINFO_H
#define PACKETINFO_H

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

	SPT_NUM_PACKET_TYPES
};

#endif