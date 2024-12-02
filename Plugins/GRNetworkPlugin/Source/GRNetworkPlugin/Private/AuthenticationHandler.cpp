// Fill out your copyright notice in the Description page of Project Settings.


#include "AuthenticationHandler.h"
#include "ClientPacketHandler.h"
#include "GameServerSession.h"
#include "PacketID.h"
#include "Protocol.pb.h"
#include "LogicHandler.h"
#include "ActivePlayer.h"
#include "FFriendInfo.h"


AuthenticationHandler::AuthenticationHandler(TWeakPtr<GameServerSession, ESPMode::ThreadSafe> Session, TStrongObjectPtr<ULogicHandler> OwnerLogicHandler) : SessionRef(Session), OwnerLogicHandlerRef(OwnerLogicHandler.Get())
{
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_LOGIN_RESPONSE , [this](RecvStructRef Packet)
		{
			HandleLoginResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_LOGOUT_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleLogoutResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_JOIN_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleJoinResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_FIND_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleFindFriendResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_ADD_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleAddFriendResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_CANCEL_ADD_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleCancelAddFriendResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_ACCEPT_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleAcceptFriendResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_REFUSE_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleRefuseFriendResponse(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2C_DEL_FRIEND_RESPONSE, [this](RecvStructRef Packet)
		{
			HandleDeleteFriendResponse(Packet);
		});


	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2O_UPDATE_STATE_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleUpdateStateNotification(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2O_ADD_FRIEND_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleAddFriendNotification(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2O_CANCEL_ADD_FRIEND_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleCancelAddFriendNotification(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2O_ACCEPT_FRIEND_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleAcceptFriendNotification(Packet);
		});
	ClientPacketHandler::Instance().RegisterCallback(PacketID::S2O_DEL_FRIEND_NOTIFICATION, [this](RecvStructRef Packet)
		{
			HandleDeleteFriendNotification(Packet);
		});
}

AuthenticationHandler::~AuthenticationHandler()
{
}

void AuthenticationHandler::RequestLogin(FString Username, FString Password)
{
	// Payload
	TSharedPtr<Protocol::C2S_LoginRequest> LoginRequest = MakeShared<Protocol::C2S_LoginRequest>();
	LoginRequest->set_username(TCHAR_TO_UTF8(*Username));
	LoginRequest->set_password(TCHAR_TO_UTF8(*Password));

	// Header
	std::string SerializedPayload;
	LoginRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = 
	MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_LOGIN_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = LoginRequest;

	SessionRef.Pin()->InsertSendTask(Packet);

	OwnerLogicHandlerRef.Get()->GetActivePlayerInfoRef()->Username = Username;
}

void AuthenticationHandler::RequestLogout()
{
	// Payload
	TSharedPtr<Protocol::C2S_LogoutRequest> LogoutRequest = MakeShared<Protocol::C2S_LogoutRequest>();

	// Header
	std::string SerializedPayload;
	LogoutRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_LOGOUT_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = LogoutRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestJoin(FString Username, FString Password)
{
	// Payload
	TSharedPtr<Protocol::C2S_JoinRequest> JoinRequest = MakeShared<Protocol::C2S_JoinRequest>();
	JoinRequest->set_username(TCHAR_TO_UTF8(*Username));
	JoinRequest->set_password(TCHAR_TO_UTF8(*Password));

	// Header
	std::string SerializedPayload;
	JoinRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header =
		MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_JOIN_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = JoinRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestFindFriend(FString FriendName)
{
	// Payload
	TSharedPtr<Protocol::C2S_FindFriendRequest> FindFriendRequest = MakeShared<Protocol::C2S_FindFriendRequest>();
	FindFriendRequest->set_username(TCHAR_TO_UTF8(*FriendName));

	// Header
	std::string SerializedPayload;
	FindFriendRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_FIND_FRIEND_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = FindFriendRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestAddFriend(FString FriendName)
{
	// Check if the friend is already in the pending list.
	if(!OwnerLogicHandlerRef->GetActivePlayerInfoRef()->IsUserInPendingList(FriendName))
	{
		// Request Add Friend
		// Payload
		TSharedPtr<Protocol::C2S_AddFriendRequest> AddFriendRequest = MakeShared<Protocol::C2S_AddFriendRequest>();
		AddFriendRequest->set_username(TCHAR_TO_UTF8(*FriendName));

		// Header
		std::string SerializedPayload;
		AddFriendRequest->SerializeToString(&SerializedPayload);
		TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_ADD_FRIEND_REQUEST));

		// SendPacket
		SendStructRef Packet = MakeShared<SendStruct>();
		Packet->Header = Header;
		Packet->Payload = AddFriendRequest;

		SessionRef.Pin()->InsertSendTask(Packet);
	}
	else
	{
		// Request Accept Friend
		// Payload
		TSharedPtr<Protocol::C2S_AcceptFriendRequest> AddFriendRequest = MakeShared<Protocol::C2S_AcceptFriendRequest>();
		AddFriendRequest->set_acceptedusername(TCHAR_TO_UTF8(*FriendName));

		// Header
		std::string SerializedPayload;
		AddFriendRequest->SerializeToString(&SerializedPayload);
		TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_ACCEPT_FRIEND_REQUEST));

		// SendPacket
		SendStructRef Packet = MakeShared<SendStruct>();
		Packet->Header = Header;
		Packet->Payload = AddFriendRequest;

		SessionRef.Pin()->InsertSendTask(Packet);
	}
}

void AuthenticationHandler::RequestCancelAddFriend(FString FriendName)
{
	TSharedPtr<Protocol::C2S_CancelAddFriendRequest> CancelAddFriendRequest = MakeShared<Protocol::C2S_CancelAddFriendRequest>();
	CancelAddFriendRequest->set_username(TCHAR_TO_UTF8(*FriendName));

	// Header
	std::string SerializedPayload;
	CancelAddFriendRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_CANCEL_ADD_FRIEND_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = CancelAddFriendRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestAcceptFriend(FString FriendName)
{
	TSharedPtr<Protocol::C2S_AcceptFriendRequest> AcceptFriendRequest = MakeShared<Protocol::C2S_AcceptFriendRequest>();
	AcceptFriendRequest->set_acceptedusername(TCHAR_TO_UTF8(*FriendName));

	// Header
	std::string SerializedPayload;
	AcceptFriendRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_ACCEPT_FRIEND_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = AcceptFriendRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestRefuseFriend(FString UserName)
{
	TSharedPtr<Protocol::C2S_RefuseFriendRequest> RefuseFriendRequest = MakeShared<Protocol::C2S_RefuseFriendRequest>();
	RefuseFriendRequest->set_refusedusername(TCHAR_TO_UTF8(*UserName));

	// Header
	std::string SerializedPayload;
	RefuseFriendRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_REFUSE_FRIEND_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = RefuseFriendRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::RequestDeleteFriend(FString FriendName)
{
	TSharedPtr<Protocol::C2S_DelFriendRequest> DeleteFriendRequest = MakeShared<Protocol::C2S_DelFriendRequest>();
	DeleteFriendRequest->set_friendname(TCHAR_TO_UTF8(*FriendName));

	// Header
	std::string SerializedPayload;
	DeleteFriendRequest->SerializeToString(&SerializedPayload);
	TSharedPtr<FPacketHeader> Header = MakeShared<FPacketHeader>(FPacketHeader(SerializedPayload.size(), PacketID::C2S_DEL_FRIEND_REQUEST));

	// SendPacket
	SendStructRef Packet = MakeShared<SendStruct>();
	Packet->Header = Header;
	Packet->Payload = DeleteFriendRequest;

	SessionRef.Pin()->InsertSendTask(Packet);
}

void AuthenticationHandler::HandleLoginResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_LoginResponse LoginResponse;
	LoginResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (LoginResponse.response() == ELoginResponse::LOGIN_SUCCESS)
	{
		UActivePlayer* ActivePlayerInfo = OwnerLogicHandlerRef.Get()->GetActivePlayerInfoRef();
		ActivePlayerInfo->SetFriendList(LoginResponse.friendlist());
		ActivePlayerInfo->SetPendingList(LoginResponse.pendinglist());
	}

	AsyncTask(ENamedThreads::GameThread, [this, LoginResponse]()
		{
			OwnerLogicHandlerRef->OnLoginRespond.Broadcast(LoginResponse.response());
		});
}

void AuthenticationHandler::HandleLogoutResponse(RecvStructRef Packet)
{
	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnLogoutRespond.Broadcast();
		});
}

void AuthenticationHandler::HandleJoinResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_JoinResponse JoinResponse;
	JoinResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	AsyncTask(ENamedThreads::GameThread, [this, JoinResponse]()
		{
			OwnerLogicHandlerRef->OnJoinRespond.Broadcast(JoinResponse.success().value());
		});
}

void AuthenticationHandler::HandleFindFriendResponse(RecvStructRef Packet)
{
	// Deserialize
	Protocol::S2C_FindFriendResponse FindFriendResponse;
	FindFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	AsyncTask(ENamedThreads::GameThread, [this, FindFriendResponse]()
		{
			FFriendInfo InfoParam = { UTF8_TO_TCHAR(FindFriendResponse.friendinfo().username().c_str()), static_cast<EUserState>(FindFriendResponse.friendinfo().state()) };
			OwnerLogicHandlerRef->OnFindFriendRespond.Broadcast(FindFriendResponse.exist().value(), InfoParam, FindFriendResponse.requeststate());
		});
}

void AuthenticationHandler::HandleAddFriendResponse(RecvStructRef Packet)
{
	Protocol::S2C_AddFriendResponse AddFriendResponse;
	AddFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	AsyncTask(ENamedThreads::GameThread, [this, AddFriendResponse]()
		{
			OwnerLogicHandlerRef->OnAddFriendRespond.Broadcast(AddFriendResponse.success().value());
		});
}

void AuthenticationHandler::HandleCancelAddFriendResponse(RecvStructRef Packet)
{
	Protocol::S2C_CancelAddFriendResponse CancelAddFriendResponse;
	CancelAddFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	AsyncTask(ENamedThreads::GameThread, [this, CancelAddFriendResponse]()
		{
			OwnerLogicHandlerRef->OnCancelAddFriendRespond.Broadcast(CancelAddFriendResponse.success().value());
		});
}

void AuthenticationHandler::HandleAcceptFriendResponse(RecvStructRef Packet)
{
	Protocol::S2C_AcceptFriendResponse AcceptFriendResponse;
	AcceptFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (!AcceptFriendResponse.success().value())
		return;

	// Edit PendingList & FriendList
	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->RemovePendingInfo(UTF8_TO_TCHAR(AcceptFriendResponse.newfriendinfo().username().c_str()));
	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->AddFriendInfo(AcceptFriendResponse.newfriendinfo());

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnAcceptFriendRespond.Broadcast();
		});
}

void AuthenticationHandler::HandleRefuseFriendResponse(RecvStructRef Packet)
{
	Protocol::S2C_RefuseFriendResponse RefuseFriendResponse;
	RefuseFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (!RefuseFriendResponse.success().value())
		return;
		
	// Edit PendingList
	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->RemovePendingInfo(UTF8_TO_TCHAR(RefuseFriendResponse.refusedusername().c_str()));

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnRefuseFriendRespond.Broadcast();
		});
}

void AuthenticationHandler::HandleDeleteFriendResponse(RecvStructRef Packet)
{
	Protocol::S2C_DelFriendResponse DeleteFriendResponse;
	DeleteFriendResponse.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	if (!DeleteFriendResponse.success().value())
		return;

	// Edit FriendList
	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->RemoveFriendInfo(UTF8_TO_TCHAR(DeleteFriendResponse.delfriendname().c_str()));

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnDeleteFriendRespond.Broadcast();
		});
}

void AuthenticationHandler::HandleUpdateStateNotification(RecvStructRef Packet)
{
	UActivePlayer* ActivePlayerInfo = OwnerLogicHandlerRef.Get()->GetActivePlayerInfoRef();

	Protocol::S2O_UpdateStateNotification UpdateStateNotification;
	UpdateStateNotification.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());
	FString FriendName = UTF8_TO_TCHAR(UpdateStateNotification.friendinfo().username().c_str());
	int32 UpdatedState = UpdateStateNotification.friendinfo().state();
	ActivePlayerInfo->UpdateFriendState(FriendName, static_cast<EUserState>(UpdatedState));
}

void AuthenticationHandler::HandleAddFriendNotification(RecvStructRef Packet)
{
	// Edit Pending List
	Protocol::S2O_AddFriendNotification AddFriendNotification;
	AddFriendNotification.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->PendingList.Emplace(FFriendInfo{ UTF8_TO_TCHAR(AddFriendNotification.username().c_str()), EUserState::ONLINE });

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnAddFriendNotified.Broadcast();
		});
}

void AuthenticationHandler::HandleCancelAddFriendNotification(RecvStructRef Packet)
{
	// Edit Pending List
	Protocol::S2O_CancelAddFriendNotification CancelAddFriendNotification;
	CancelAddFriendNotification.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->RemovePendingInfo(UTF8_TO_TCHAR(CancelAddFriendNotification.username().c_str()));

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnCancelAddFriendNotified.Broadcast();
		});
}

void AuthenticationHandler::HandleAcceptFriendNotification(RecvStructRef Packet)
{
	// Append FriendList
	Protocol::S2O_AcceptFriendNotification AcceptFriendNotification;
	AcceptFriendNotification.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->AddFriendInfo(AcceptFriendNotification.newfriendinfo());

	AsyncTask(ENamedThreads::GameThread, [this]()
		{
			OwnerLogicHandlerRef->OnAcceptFriendNotified.Broadcast();
		});
}

void AuthenticationHandler::HandleDeleteFriendNotification(RecvStructRef Packet)
{
	// Edit FriendList
	Protocol::S2O_DelFriendNotification DeleteFriendNotification;
	DeleteFriendNotification.ParseFromArray(Packet->PayloadBuffer.GetData(), Packet->GetPayloadSize());

	OwnerLogicHandlerRef->GetActivePlayerInfoRef()->RemoveFriendInfo(UTF8_TO_TCHAR(DeleteFriendNotification.delfriendname().c_str()));
}
