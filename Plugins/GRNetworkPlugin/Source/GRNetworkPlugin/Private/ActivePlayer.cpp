// Fill out your copyright notice in the Description page of Project Settings.


#include "ActivePlayer.h"

UActivePlayer::UActivePlayer()
{
}

UActivePlayer::~UActivePlayer()
{
}

void UActivePlayer::SetFriendList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& FriendInfoList)
{
	for (const auto& FriendInfo : FriendInfoList)
	{
		FriendList.Emplace(FFriendInfo{ UTF8_TO_TCHAR(FriendInfo.username().c_str()), static_cast<EUserState>(FriendInfo.state()) });
	}
}

void UActivePlayer::SetPendingList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& PendingInfoList)
{
	for (const auto& PendingInfo : PendingInfoList)
	{
		PendingList.Emplace(FFriendInfo{ UTF8_TO_TCHAR(PendingInfo.username().c_str()), static_cast<EUserState>(PendingInfo.state()) });
	}
}

void UActivePlayer::UpdateFriendState(FString Name, EUserState State)
{
	int32 Index = FriendList.IndexOfByPredicate([Name](const FFriendInfo& Info)
		{
			return (Info.Name == Name);
		});

	FriendList[Index] = { Name, State };
}

void UActivePlayer::AddFriendInfo(const Protocol::FriendInfo& Info)
{
	FriendList.Emplace(FFriendInfo{ UTF8_TO_TCHAR(Info.username().c_str()), static_cast<EUserState>(Info.state()) });
}

void UActivePlayer::AddPendingInfo(const Protocol::FriendInfo& Info)
{
	PendingList.Emplace(FFriendInfo{ UTF8_TO_TCHAR(Info.username().c_str()), static_cast<EUserState>(Info.state()) });
}

void UActivePlayer::RemoveFriendInfo(const FString& FriendName)
{
	FriendList.RemoveAll([&](const FFriendInfo& FriendInfo)
		{
			return FriendInfo.Name == FriendName;
		});
}

void UActivePlayer::RemovePendingInfo(const FString& UserName)
{
	PendingList.RemoveAll([&](const FFriendInfo& FriendInfo)
		{
			return FriendInfo.Name == UserName;
		});
}

bool UActivePlayer::IsUserInFriendList(const FString& UserName)
{
	return (FriendList.FindByKey(UserName) != nullptr);
}

bool UActivePlayer::IsUserInPendingList(const FString& UserName)
{
	return (PendingList.FindByKey(UserName) != nullptr);
}
