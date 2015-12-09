#ifndef PACKETINFO_H
#define PACKETINFO_H

enum ServerPacketType
{
	SPT_Unknown,

	SPT_Identify,
	SPT_IdentifyAcknowledged,
	SPT_IdentifyDenied,

	SPT_GotCorruptPacket,
};

#endif