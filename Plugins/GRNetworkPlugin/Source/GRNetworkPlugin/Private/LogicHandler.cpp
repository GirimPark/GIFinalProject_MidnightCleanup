// Fill out your copyright notice in the Description page of Project Settings.


#include "LogicHandler.h"
#include "AuthenticationHandler.h"
#include "GameRoomHandler.h"
#include "GameServerSession.h"
#include "ActivePlayer.h"

ULogicHandler::ULogicHandler()
{
	ActivePlayerInfo = TStrongObjectPtr<UActivePlayer>(NewObject<UActivePlayer>());
}

ULogicHandler::~ULogicHandler()
{
	OnSessionExpired.Clear();
	OnLoginRespond.Clear();
	OnJoinRespond.Clear();
	OnCreateRoomRespond.Clear();
	OnJoinRoomRespond.Clear();
	OnQuitRoomSucceed.Clear();
}

void ULogicHandler::Setup(TSharedPtr<class GameServerSession> ServerSession)
{
	SessionRef = ServerSession;
	AuthenticationHandler = MakeShared<class AuthenticationHandler, ESPMode::ThreadSafe>(SessionRef, TStrongObjectPtr<ULogicHandler>(this));
	GameRoomHandler = MakeShared<class GameRoomHandler, ESPMode::ThreadSafe>(SessionRef, TStrongObjectPtr<ULogicHandler>(this));
}

void ULogicHandler::RequestLogin(FString Username, FString Password)
{
	if(AuthenticationHandler)
		AuthenticationHandler->RequestLogin(Username, Password);
}

void ULogicHandler::RequestLogout()
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestLogout();
}

void ULogicHandler::RequestJoin(FString Username, FString Password)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestJoin(Username, Password);
}

void ULogicHandler::RequestCreateRoom()
{
	if (GameRoomHandler)
		GameRoomHandler->RequestCreateRoom();
}

void ULogicHandler::RequestJoinRoom(FString RoomCode)
{
	if (GameRoomHandler)
		GameRoomHandler->RequestJoinRoom(RoomCode);
}

void ULogicHandler::RequestQuitRoom()
{
	if (GameRoomHandler)
		GameRoomHandler->RequestQuitRoom();
}

void ULogicHandler::RequestFindFriend(FString FriendName)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestFindFriend(FriendName);
}

void ULogicHandler::RequestAddFriend(FString FriendName)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestAddFriend(FriendName);
}

void ULogicHandler::RequestCancelAddFriend(FString FriendName)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestCancelAddFriend(FriendName);
}

void ULogicHandler::RequestAcceptFriend(FString FriendName)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestAcceptFriend(FriendName);
}

void ULogicHandler::RequestRefuseFriend(FString UserName)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestRefuseFriend(UserName);
}

void ULogicHandler::RequestDeleteFriend(FString FriendName)
{
	if (AuthenticationHandler)
		AuthenticationHandler->RequestDeleteFriend(FriendName);
}

void ULogicHandler::RequestInviteFriend(FString FriendName)
{
	if (GameRoomHandler)
		GameRoomHandler->RequestInviteFriend(FriendName);
}

void ULogicHandler::RequestStartGame()
{
	if (GameRoomHandler)
		GameRoomHandler->RequestStartGame();
}

void ULogicHandler::RequestEndGame()
{
	if (GameRoomHandler)
		GameRoomHandler->RequestEndGame();
}
