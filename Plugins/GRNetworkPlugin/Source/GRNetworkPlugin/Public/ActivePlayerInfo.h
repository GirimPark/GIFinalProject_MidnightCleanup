// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketID.h"
#include "Protocol.pb.h"

/**
 * Active Player Information
 * Replication of Server's "User"
 */
class GRNETWORKPLUGIN_API ActivePlayerInfo
{
public:
	ActivePlayerInfo();
	~ActivePlayerInfo();

	void SetUsername(FString name) { Username = name; }

	void SetFriendList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& FriendInfoList);
	void SetPendingList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& PendingInfoList);
	const TMap<FString, EUserState>& GetFriendList() const { return FriendList; }
	const TMap<FString, EUserState>& GetPendingList() const { return PendingList; }

	void AddFriendInfo(const Protocol::FriendInfo& Info);
	void AddPendingInfo(const Protocol::FriendInfo& Info);

private:
	FString Username;
	TMap<FString, EUserState> FriendList;
	TMap<FString, EUserState> PendingList;
};
 