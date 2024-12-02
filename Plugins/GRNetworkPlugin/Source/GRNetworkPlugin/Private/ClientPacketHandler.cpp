// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientPacketHandler.h"
#include "GameServerSession.h"
#include "PacketID.h"
#include "HAL/ThreadManager.h"

void ClientPacketHandler::RegisterCallback(uint16 PacketID, PacketHandlerCallback Callback)
{
	PacketHandlerCallbacks.Add(PacketID, Callback);
}

void ClientPacketHandler::ExecuteHandler(RecvStructRef Packet)
{
	auto Callback = PacketHandlerCallbacks.Find(Packet->Header->PacketID);
	(*Callback)(Packet);
}


HandlePacketWorker::HandlePacketWorker(TSharedPtr<GameServerSession, ESPMode::ThreadSafe> Session) : SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("HandlePacketWorkerThread"));
	FThreadManager::Get().AddThread(Thread->GetThreadID(), Thread);
}

HandlePacketWorker::~HandlePacketWorker()
{
}

uint32 HandlePacketWorker::Run()
{
	while(Running)
	{
		if(!SessionRef.IsValid() || !SessionRef.Pin()->IsConnected())
		{
			UE_LOG(LogTemp, Error, TEXT("Session is Expired"));
			return 0;
		}

		RecvStructRef Packet = MakeShared<RecvStruct>();
		if(SessionRef.Pin()->PopRecvTask(OUT Packet))
		{
			ClientPacketHandler::Instance().ExecuteHandler(Packet);
		}
		
	}

	return 0;
}

void HandlePacketWorker::Stop()
{
	Running = false;
}

void HandlePacketWorker::Destroy()
{
	Running = false;
}