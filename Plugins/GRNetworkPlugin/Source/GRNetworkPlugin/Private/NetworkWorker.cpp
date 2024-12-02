// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkWorker.h"
#include "Sockets.h"
#include "GameServerSession.h"
#include "ClientPacket.h"
#include "HAL/ThreadManager.h"

RecvWorker::RecvWorker(FSocket* Socket, TSharedPtr<class GameServerSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"));
	FThreadManager::Get().AddThread(Thread->GetThreadID(), Thread);
}

RecvWorker::~RecvWorker()
{

}

uint32 RecvWorker::Run()
{
	while(Running && Socket->GetConnectionState() == SCS_Connected)
	{
		RecvStructRef Packet = MakeShared<RecvStruct>();

		if(ReceivePacket(OUT Packet))
		{
			if(!SessionRef.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("Session is Expired"));
				return 0;
			}

			SessionRef.Pin()->InsertRecvTask(Packet);
		}
	}

	return 0;
}

void RecvWorker::Stop()
{
	Running = false;
}

void RecvWorker::Destroy()
{
	Running = false;
}

bool RecvWorker::ReceivePacket(OUT RecvStructRef Packet)
{
	// 패킷 헤더 파싱
	if (!ReceivePacketHeader(Packet))
		return false;
	

	// 패킷 내용 파싱
	if(Packet->Header->PacketSize == FPacketHeader::Size())
		return true;

	Packet->PayloadBuffer.AddZeroed(Packet->Header->PacketSize);
	if (!ReceiveDesiredBytes(&Packet->PayloadBuffer[0], Packet->Header->PacketSize - FPacketHeader::Size()))
		return false;

	return true;
}

bool RecvWorker::ReceivePacketHeader(OUT RecvStructRef Packet)
{
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(FPacketHeader::Size());

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), FPacketHeader::Size()) == false)
		return false;

	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
	}

	Packet->Header = MakeShared<FPacketHeader>(Header);

	return true;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* Results, int32 Size)
{
	if (!Socket)
		return false;

	uint32 PendingDataSize;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
		return false;

	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size);

		if (NumRead <= 0)
			return false;

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}

SendWorker::SendWorker(FSocket* Socket, TSharedPtr<class GameServerSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
	FThreadManager::Get().AddThread(Thread->GetThreadID(), Thread);
}

SendWorker::~SendWorker()
{
}

uint32 SendWorker::Run()
{
	while (Running && Socket->GetConnectionState() == SCS_Connected)
	{
		if(!SessionRef.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Session is Expired"));
			return 0;
		}

		SendStructRef Packet = MakeShared<SendStruct>();
		if (SessionRef.Pin()->PopSendTask(OUT Packet))
		{
			SendPacket(Packet);
		}
		
	}

	return 0;
}

void SendWorker::Stop()
{
	Running = false;
}

void SendWorker::Destroy()
{
	Running = false;
}

bool SendWorker::SendPacket(SendStructRef Packet)
{
	Packet->Serialize();

	if (!SendDesiredBytes(&Packet->Buffer[0], Packet->Header->PacketSize))
		return false;

	return true;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	if (!Socket)
		return false;

	while (Size > 0)
	{
		int32 BytesSent = 0;
		if (Socket->Send(Buffer, Size, BytesSent) == false)
			return false;

		Size -= BytesSent;
		Buffer += BytesSent;
	}

	return true;
}
