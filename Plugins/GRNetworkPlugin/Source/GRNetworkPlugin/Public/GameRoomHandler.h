// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"

class GRNETWORKPLUGIN_API GameRoomHandler
{
public:
	GameRoomHandler(TWeakPtr<class GameServerSession> Session, TStrongObjectPtr<class ULogicHandler> OwnerLogicHandler);
	~GameRoomHandler();

	void RequestCreateRoom();
	void RequestJoinRoom(FString RoomCode);
	void RequestQuitRoom();
	void RequestInviteFriend(FString Username);
	void RequestStartGame();
	void RequestEndGame();

	/// Handler
	void HandleCreateRoomResponse(RecvStructRef Packet);
	void HandleJoinRoomResponse(RecvStructRef Packet);
	void HandleQuitRoomResponse(RecvStructRef Packet);
	void HandleInviteFriendResponse(RecvStructRef Packet);
	void HandleStartGameResponse(RecvStructRef Packet);
	void HandleEndGameResponse(RecvStructRef Packet);

	void HandleInviteFriendNotification(RecvStructRef Packet);

private:
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
	TWeakObjectPtr<ULogicHandler> OwnerLogicHandlerRef;
};
