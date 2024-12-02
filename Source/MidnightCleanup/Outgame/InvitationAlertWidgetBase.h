// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvitationAlertWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UInvitationAlertWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnAcceptButtonClicked();

	UFUNCTION()
	void OnRefuseButtonClicked();

	UFUNCTION()
	void OnInvitationNotified(FString FriendName, FString RoomCode);

	UFUNCTION()
	void OnPopupVisibilityChanged(ESlateVisibility NewVisibility);


private:
	TObjectPtr<class UTextBlock> InvitationMessageText;
	TObjectPtr<class UButton> AcceptButton;
	TObjectPtr<class UButton> RefuseButton;

	FString InvitedRoomCode;
};
