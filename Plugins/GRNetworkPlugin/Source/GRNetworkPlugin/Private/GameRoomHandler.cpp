// Fill out your copyright notice in the Description page of Project Settings.


#include "GameRoomHandler.h"
#include "ClientPacketHandler.h"
#include "GameServerSession.h"
#include "PacketID.h"
#include "Protocol.pb.h"
#include "LogicHandler.h"


GameRoomHandler::GameRoomHandler(TWeakPtr<class GameServerSession> Session, TStrongObjectPtr<class ULogicHandler> OwnerLogicHandler) : SessionRef(Session), OwnerLogicHandlerRef(OwnerLogicHandler.Get())
{
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_CREATE_ROOM_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleCreateRoomResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_JOIN_ROOM_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleJoinRoomResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_QUIT_ROOM_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleQuitRoomResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_START_GAME_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleStartGameResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_END_GAME_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleEndGameResponse(Packet);
		});

	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_INVITE_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleInviteFriendResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2O_INVITE_FRIEND_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleInviteFriendNotification(Packet);
		});
}

GameRoomHandler::~GameRoomHandler()
{
}

void GameRoomHandler::RequestCreateRoom()
{
	// Payload
	TSharedPtr<Protocol::C2S_CreateRoomRequest> CreateRoomRequest = MakeShared<Protocol::C2S_CreateRoomRequest>();

	// Header
	std::string SerializedPayload;
	CreateRoomRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_CREATE_ROOM_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = CreateRoomRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestJoinRoom(FString RoomCode)
{
	// Payload
	TSharedPtr<Protocol::C2S_JoinRoomRequest> JoinRoomRequest = MakeShared<Protocol::C2S_JoinRoomRequest>();
	JoinRoomRequest->set_roomcode(TCHAR_TO_UTF8(*RoomCode));

	// Header
	std::string SerializedPayload;
	JoinRoomRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_JOIN_ROOM_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = JoinRoomRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestQuitRoom()
{
	// Payload
	TSharedPtr<Protocol::C2S_QuitRoomRequest> QuitRoomRequest = MakeShared<Protocol::C2S_QuitRoomRequest>();

	// Header
	std::string SerializedPayload;
	QuitRoomRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_QUIT_ROOM_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = QuitRoomRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestInviteFriend(FString Username)
{
	// Payload
	TSharedPtr<Protocol::C2S_InviteFriendRequest> InviteFriendRequest = MakeShared<Protocol::C2S_InviteFriendRequest>();
	InviteFriendRequest->set_username(TCHAR_TO_UTF8(*Username));

	// Header
	std::string SerializedPayload;
	InviteFriendRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_INVITE_FRIEND_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = InviteFriendRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestStartGame()
{
	// Payload
	TSharedPtr<Protocol::C2S_StartGameRequest> StartGameRequest = MakeShared<Protocol::C2S_StartGameRequest>();

	// Header
	std::string SerializedPayload;
	StartGameRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_START_GAME_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = StartGameRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void GameRoomHandler::RequestEndGame()
{
	// Payload
	TSharedPtr<Protocol::C2S_EndGameRequest> EndGameRequest = MakeShared<Protocol::C2S_EndGameRequest>();

	// Header
	std::string SerializedPayload;
	EndGameRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_END_GAME_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = EndGameRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}


void GameRoomHandler::HandleCreateRoomResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_CreateRoomResponse CreateRoomResponse;
	CreateRoomResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	AsyncTask(ENamedThreads::GameThread, [this, CreateRoomResponse]()
		{
			OwnerLogicHandlerRef->OnCreateRoomRespond.Broadcast(CreateRoomResponse.success().value(),  UTF8_TO_TCHAR(CreateRoomResponse.roomcode().c_str()));
		});
}

void GameRoomHandler::HandleJoinRoomResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_JoinRoomResponse JoinRoomResponse;
	JoinRoomResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());
	FString ipAddress = UTF8_TO_TCHAR(JoinRoomResponse.ipaddress().c_str());

	AsyncTask(ENamedThreads::GameThread, [this, JoinRoomResponse, ipAddress]()
		{
			OwnerLogicHandlerRef->OnJoinRoomRespond.Broadcast(JoinRoomResponse.resultcode(), ipAddress);
		});
}

void GameRoomHandler::HandleQuitRoomResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_QuitRoomResponse QuitRoomResponse;
	QuitRoomResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (!QuitRoomResponse.success().value())
		return;
	
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnQuitRoomSucceed.Broadcast();
		});
}

void GameRoomHandler::HandleInviteFriendResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_InviteFriendResponse InviteFriendResponse;
	InviteFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());
	FString InvitedUserName = UTF8_TO_TCHAR(InviteFriendResponse.invitedusername().c_str());

	AsyncTask(ENamedThreads::GameThread, [this, InviteFriendResponse, InvitedUserName]()
		{
			OwnerLogicHandlerRef->OnInviteFriendRespond.Broadcast(InviteFriendResponse.success().value(), InvitedUserName);
		});
}

void GameRoomHandler::HandleStartGameResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_StartGameResponse StartGameResponse;
	StartGameResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (!StartGameResponse.success().value())
		return;

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnStartGameRespond.Broadcast();
		});
}

void GameRoomHandler::HandleEndGameResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_EndGameResponse EndGameResponse;
	EndGameResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (!EndGameResponse.success().value())
		return;

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnEndGameRespond.Broadcast();
		});
}

void GameRoomHandler::HandleInviteFriendNotification(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2O_InviteFriendNotification InviteFriendNotification;
	InviteFriendNotification.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	AsyncTask(ENamedThreads::GameThread, [this, InviteFriendNotification]()
		{
			OwnerLogicHandlerRef->OnInviteFriendNotified.Broadcast(InviteFriendNotification.username().c_str(), InviteFriendNotification.roomcode().c_str());
		});
}
