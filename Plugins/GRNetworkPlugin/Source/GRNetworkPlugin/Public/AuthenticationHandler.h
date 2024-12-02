// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientPacket.h"

class GRNETWORKPLUGIN_API AuthenticationHandler
{
public:
	AuthenticationHandler(TWeakPtr<class GameServerSession, ESPMode::ThreadSafe> Session, TStrongObjectPtr<class ULogicHandler> OwnerLogicHandler);
	~AuthenticationHandler();

	void RequestLogin(FString Username, FString Password);
	void RequestLogout();
	void RequestJoin(FString Username, FString Password);
	void RequestFindFriend(FString FriendName);
	void RequestAddFriend(FString FriendName);	// this function can request AddFriend or AcceptFriend
	void RequestCancelAddFriend(FString FriendName);
	void RequestAcceptFriend(FString FriendName);
	void RequestRefuseFriend(FString UserName);
	void RequestDeleteFriend(FString FriendName);

/// Handler
	void HandleLoginResponse(RecvStructRef Packet);
	void HandleLogoutResponse(RecvStructRef Packet);
	void HandleJoinResponse(RecvStructRef Packet);
	void HandleFindFriendResponse(RecvStructRef Packet);
	void HandleAddFriendResponse(RecvStructRef Packet);
	void HandleCancelAddFriendResponse(RecvStructRef Packet);
	void HandleAcceptFriendResponse(RecvStructRef Packet);
	void HandleRefuseFriendResponse(RecvStructRef Packet);
	void HandleDeleteFriendResponse(RecvStructRef Packet);

	void HandleUpdateStateNotification(RecvStructRef Packet);
	void HandleAddFriendNotification(RecvStructRef Packet);
	void HandleCancelAddFriendNotification(RecvStructRef Packet);
	void HandleAcceptFriendNotification(RecvStructRef Packet);
	void HandleDeleteFriendNotification(RecvStructRef Packet);

private:
	TWeakPtr<GameServerSession, ESPMode::ThreadSafe> SessionRef;
	TWeakObjectPtr<ULogicHandler> OwnerLogicHandlerRef;
};
