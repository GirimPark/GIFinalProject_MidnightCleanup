// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Protocol.pb.h"

typedef google::protobuf::Message Payload;

struct GRNETWORKPLUGIN_API FPacketHeader
{
	FPacketHeader() : PacketSize(0), PacketID(0)
	{
	}

	FPacketHeader(uint16 PayloadSize, uint16 PacketID) : PacketSize(Size() + PayloadSize), PacketID(PacketID)
	{
	}

	static uint32 Size()
	{
		return sizeof(PacketSize) + sizeof(PacketID);
	}

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& Header)
	{
		Ar << Header.PacketSize;
		Ar << Header.PacketID;
		return Ar;
	}

	uint16 PacketSize;
	uint16 PacketID;
};

struct GRNETWORKPLUGIN_API RecvStruct
{
	TSharedPtr<FPacketHeader> Header;
	TArray<uint8> PayloadBuffer;

	int32 GetPayloadSize()
	{
		return (Header->PacketSize - FPacketHeader::Size());
	}
};

struct GRNETWORKPLUGIN_API SendStruct
{
	TSharedPtr<FPacketHeader> Header;
	TSharedPtr<Payload> Payload;

	TArray<uint8> Buffer;

	void Serialize()
	{
		FMemoryWriter Writer(Buffer);
		Writer << *Header;

		int32 PayloadSize = Payload->ByteSizeLong();
		Buffer.AddUninitialized(PayloadSize);
		if (!Payload->SerializeToArray(Buffer.GetData() + Writer.Tell(), PayloadSize))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to serialize Payload to Buffer"));
		}
	}
};

#define USING_SHARED_PTR(name) using name##Ref = TSharedPtr<struct name>;

USING_SHARED_PTR(FPacketHeader)
USING_SHARED_PTR(RecvStruct)
USING_SHARED_PTR(SendStruct)