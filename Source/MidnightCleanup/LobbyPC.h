// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class ULobbyWidgetBase> LobbyWidgetClass;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class ULobbyWidgetBase> LobbyWidget;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Text);
	bool C2S_SendMessage_Validate(const FText& Text);
	void C2S_SendMessage_Implementation(class FText const& Text);

	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(const FText& Text);
	void S2C_SendMessage_Implementation(class FText const& Text);
};
