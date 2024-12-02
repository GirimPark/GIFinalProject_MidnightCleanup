// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"

typedef TFunction<void(RecvStructRef)> PacketHandlerCallback;

class GRNETWORKPLUGIN_API ClientPacketHandler
{
public:
	static ClientPacketHandler& Instance()
	{
		static ClientPacketHandler instance;
		return instance;
	}
	 
	void RegisterCallback(uint16 PacketID, PacketHandlerCallback Callback);

	void ExecuteHandler(RecvStructRef Packet);

private:
	ClientPacketHandler() = default;
	~ClientPacketHandler() = default;
	ClientPacketHandler(const ClientPacketHandler&) = delete;
	ClientPacketHandler& operator=(const ClientPacketHandler&) = delete;


private:
	TMap<uint16, PacketHandlerCallback> PacketHandlerCallbacks;
};

class GRNETWORKPLUGIN_API HandlePacketWorker : public FRunnable
{
public:
	HandlePacketWorker(TSharedPtr<class GameServerSession, ESPMode::ThreadSafe> Session);
	~HandlePacketWorker();

	virtual uint32 Run() override;
	virtual void Stop() override;

	void Destroy();

	FRunnableThread* GetThread() const { return Thread; }

private:
	FRunnableThread* Thread = nullptr;
	TAtomic<bool> Running = true;
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
};