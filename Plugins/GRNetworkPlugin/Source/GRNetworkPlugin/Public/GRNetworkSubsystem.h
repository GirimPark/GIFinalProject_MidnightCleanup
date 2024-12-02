// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GRNetworkSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GRNETWORKPLUGIN_API UGRNetworkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void ConnectToServer(FString IpAddress, int32 Port);

	UFUNCTION(BlueprintCallable)
	void DisconnectFromServer();

	UFUNCTION(BlueprintCallable)
	class ULogicHandler* GetLogicHandler();

	UFUNCTION(BlueprintCallable)
	bool IsVerified() { return ServerSession.IsValid(); }


private:
	FSocket* Socket;

	TSharedPtr<class GameServerSession, ESPMode::ThreadSafe> ServerSession;
	TStrongObjectPtr<class ULogicHandler> LogicHandler;
};
