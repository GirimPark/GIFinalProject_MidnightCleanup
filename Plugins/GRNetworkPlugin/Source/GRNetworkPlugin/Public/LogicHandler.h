// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FFriendInfo.h"
#include "LogicHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResponseHandlerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseHandlerDelegate_WithBool, bool, BoolParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseHandlerDelegate_WithInt, int32, IntParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseHandlerDelegate_WithInfoArray, TArray<FFriendInfo>&, ArrayParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResponseHandlerDelegate_WithString, FString, StrParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseHandlerDelegate_WithIntString, int32, IntParam, FString, StrParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseHandlerDelegate_WithStringString, FString, StrParam1, FString, StrParam2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseHandlerDelegate_WithBoolString, bool, BoolParam, FString, StrParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FResponseHandlerDelegate_WithBoolInfoInt, bool, BoolParam, FFriendInfo, FriendInfoParam, int32, IntParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseHandlerDelegate_WithBoolInfo, bool, BoolParam, FFriendInfo, FriendInfoParam);

UCLASS()
class GRNETWORKPLUGIN_API ULogicHandler : public UObject
{
	GENERATED_BODY()

public:
	ULogicHandler();
	~ULogicHandler();

	void Setup(TSharedPtr<class GameServerSession> ServerSession);

	UFUNCTION(BlueprintCallable)
	class UActivePlayer* GetActivePlayerInfoRef() { return ActivePlayerInfo.Get(); }

	UFUNCTION(BlueprintCallable)
	void RequestLogin(FString Username, FString Password);

	UFUNCTION(BlueprintCallable)
	void RequestLogout();

	UFUNCTION(BlueprintCallable)
	void RequestJoin(FString Username, FString Password);

	UFUNCTION(BlueprintCallable)
	void RequestCreateRoom();

	UFUNCTION(BlueprintCallable)
	void RequestJoinRoom(FString RoomCode);

	UFUNCTION(BlueprintCallable)
	void RequestQuitRoom();

	UFUNCTION(BlueprintCallable)
	void RequestFindFriend(FString FriendName);

	UFUNCTION(BlueprintCallable)
	void RequestAddFriend(FString FriendName);

	UFUNCTION(BlueprintCallable)
	void RequestCancelAddFriend(FString FriendName);

	UFUNCTION(BlueprintCallable)
	void RequestAcceptFriend(FString FriendName);

	UFUNCTION(BlueprintCallable)
	void RequestRefuseFriend(FString UserName);

	UFUNCTION(BlueprintCallable)
	void RequestDeleteFriend(FString FriendName);

	UFUNCTION(BlueprintCallable)
	void RequestInviteFriend(FString FriendName);

	void RequestStartGame();

	void RequestEndGame();


	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnSessionExpired;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithInt OnLoginRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnLogoutRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithBool OnJoinRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithBoolString OnCreateRoomRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithIntString OnJoinRoomRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnQuitRoomSucceed;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithBoolInfoInt OnFindFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithBool OnAddFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnAddFriendNotified;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithBool OnCancelAddFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnCancelAddFriendNotified;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnAcceptFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnAcceptFriendNotified;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnRefuseFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnDeleteFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithBoolString OnInviteFriendRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate_WithStringString OnInviteFriendNotified;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnStartGameRespond;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FResponseHandlerDelegate OnEndGameRespond;


private:
	TStrongObjectPtr<class UActivePlayer> ActivePlayerInfo;

	TWeakPtr<class GameServerSession, ESPMode::ThreadSafe> SessionRef;

	TSharedPtr<class AuthenticationHandler, ESPMode::ThreadSafe> AuthenticationHandler;
	TSharedPtr<class GameRoomHandler, ESPMode::ThreadSafe> GameRoomHandler;
};
