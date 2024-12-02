// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PacketID.h"
#include "FFriendInfo.h"
#include "Protocol.pb.h"
#include "ActivePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GRNETWORKPLUGIN_API UActivePlayer : public UObject
{
	GENERATED_BODY()
public:
	UActivePlayer();
	~UActivePlayer();

	void SetFriendList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& FriendInfoList);
	void SetPendingList(const google::protobuf::RepeatedPtrField<Protocol::FriendInfo>& PendingInfoList);

	void UpdateFriendState(FString Name, EUserState State);

	void AddFriendInfo(const Protocol::FriendInfo& Info);
	void AddPendingInfo(const Protocol::FriendInfo& Info);
	void RemoveFriendInfo(const FString& FriendName);
	void RemovePendingInfo(const FString& UserName);

	bool IsUserInFriendList(const FString& UserName);
	bool IsUserInPendingList(const FString& UserName);

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	FString Username;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<FFriendInfo> FriendList;

	UPROPERTY(BlueprintReadOnly, Category = "Data")
	TArray<FFriendInfo> PendingList;
};
