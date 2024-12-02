// Fill out your copyright notice in the Description page of Project Settings.


#include "GameServerSession.h"
#include "NetworkWorker.h"
#include "ClientPacketHandler.h"

GameServerSession::GameServerSession(FSocket* Socket) : Socket(Socket)
{
}

GameServerSession::~GameServerSession()
{
	Disconnect();
}

void GameServerSession::Run()
{
	RecvWorker = MakeShared<class RecvWorker, ESPMode::ThreadSafe>(Socket, AsShared());
	SendWorker = MakeShared<class SendWorker, ESPMode::ThreadSafe>(Socket, AsShared());
	HandlePacketWorker = MakeShared<class HandlePacketWorker, ESPMode::ThreadSafe>(AsShared());

	Threads.Add(RecvWorker->GetThread());
	Threads.Add(SendWorker->GetThread());
	Threads.Add(HandlePacketWorker->GetThread());
}

void GameServerSession::Disconnect()
{
	if (RecvWorker)
	{
		RecvWorker->Destroy();
		RecvWorker = nullptr;
	}

	if (SendWorker)
	{
		SendWorker->Destroy();
		SendWorker = nullptr;
	}

	if (HandlePacketWorker)
	{
		HandlePacketWorker->Destroy();
		HandlePacketWorker = nullptr;
	}
}

void GameServerSession::InsertRecvTask(RecvStructRef Packet)
{
	RecvQueue.Enqueue(Packet);
}

bool GameServerSession::PopRecvTask(OUT RecvStructRef& Packet)
{
	return RecvQueue.Dequeue(Packet);
}

void GameServerSession::InsertSendTask(SendStructRef Packet)
{
	SendQueue.Enqueue(Packet);
}

bool GameServerSession::PopSendTask(OUT SendStructRef& Packet)
{
	return SendQueue.Dequeue(Packet);
}

void GameServerSession::ShutdownThreads()
{
	for (auto& Thread : Threads)
		Thread->Kill();
}
