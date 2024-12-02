#pragma once
//#pragma pack (push, 1)

#include "PacketID.h"

struct PacketHeader
{
	unsigned short length;
	PacketID packetID;

	PacketHeader() = default;
	PacketHeader(short length, PacketID id)
		: length(length), packetID(id) { }

	// Packet to char*
	void Serialize(char* buffer) const
	{
		std::memcpy(buffer, &length, sizeof(length));
		std::memcpy(buffer + sizeof(length), &packetID, sizeof(packetID));
	}

	// char* to Packet
	static PacketHeader Deserialize(const char* buffer)
	{
		short _length;
		PacketID _type;

		std::memcpy(&_length, buffer, sizeof(_length));
		std::memcpy(&_type, buffer + sizeof(_length), sizeof(_type));

		return PacketHeader(_length, _type);
	}

	static constexpr short Size()
	{
		return sizeof(length) + sizeof(packetID);
	}
};

//#pragma pack(pop)