// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"
#include "Sockets.h"

/**
 * 
 */
class GRNETWORKPLUGIN_API GameServerSession : public TSharedFromThis<GameServerSession>
{
public:
	GameServerSession(FSocket* Socket);
	~GameServerSession();

	void Run();

	void Disconnect();

	bool IsConnected() const
	{
		return (Socket->GetConnectionState() == SCS_Connected);
	}

	void InsertRecvTask(RecvStructRef Packet);
	bool PopRecvTask(OUT RecvStructRef& Packet);
	void InsertSendTask(SendStructRef Packet);
	bool PopSendTask(OUT SendStructRef& Packet);

	void ShutdownThreads();

private:
	FSocket* Socket;

	TArray<FRunnableThread*> Threads;
	TSharedPtr<class RecvWorker, ESPMode::ThreadSafe> RecvWorker;
	TSharedPtr<class SendWorker, ESPMode::ThreadSafe> SendWorker;
	TSharedPtr<class HandlePacketWorker, ESPMode::ThreadSafe> HandlePacketWorker;

	TQueue<RecvStructRef> RecvQueue;
	TQueue<SendStructRef> SendQueue;
	FCriticalSection RecvQueueLock;
	FCriticalSection SendQueueLock;
};
