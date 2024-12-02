// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"

class GRNETWORKPLUGIN_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class GameServerSession> Session);
	~RecvWorker();

	virtual uint32 Run() override;
	virtual void Stop() override;

	void Destroy();

	FRunnableThread* GetThread() const { return Thread; }

private:
	bool ReceivePacket(OUT RecvStructRef Packet);
	bool ReceivePacketHeader(OUT RecvStructRef Packet);
	bool ReceiveDesiredBytes(uint8* Results, int32 Size);

private:
	FRunnableThread* Thread = nullptr;
	TAtomic<bool> Running = true;
	FSocket* Socket;
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
};

class GRNETWORKPLUGIN_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* Socket, TSharedPtr<class GameServerSession> Session);
	~SendWorker();

	virtual uint32 Run() override;
	virtual void Stop() override;

	void Destroy();

	FRunnableThread* GetThread() const { return Thread; }

private:
	bool SendPacket(SendStructRef Packet);
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

private:
	FRunnableThread* Thread = nullptr;
	TAtomic<bool> Running = true;
	FSocket* Socket;
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
};