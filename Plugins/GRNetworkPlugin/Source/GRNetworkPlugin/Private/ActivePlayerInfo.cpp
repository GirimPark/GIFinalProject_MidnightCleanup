// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivePlayerInfo.h"

#include "PacketID.h"


ActivePlayerInfo::ActivePlayerInfo()
{
}

ActivePlayerInfo::~ActivePlayerInfo()
{
}

void ActivePlayerInfo::SetFriendList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& FriendInfoList)
{
	for (const auto& FriendInfo : FriendInfoList)
	{
		FriendList.Emplace({ UTF8_TO_TCHAR(FriendInfo.username().c_str()), static_cast<EUserState>(FriendInfo.state()) });
	}
}

void ActivePlayerInfo::SetPendingList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& PendingInfoList)
{
	for (const auto& PendingInfo : PendingInfoList)
	{
		PendingList.Emplace({ UTF8_TO_TCHAR(PendingInfo.username().c_str()), static_cast<EUserState>(PendingInfo.state()) });
	}
}

void ActivePlayerInfo::AddFriendInfo(const Protocol::FriendInfo& Info)
{
	FriendList.Emplace({ UTF8_TO_TCHAR(Info.username().c_str()), static_cast<EUserState>(Info.state()) });
}

void ActivePlayerInfo::AddPendingInfo(const Protocol::FriendInfo& Info)
{
	PendingList.Emplace(UTF8_TO_TCHAR(Info.username().c_str()), static_cast<EUserState>(Info.state()));
}
