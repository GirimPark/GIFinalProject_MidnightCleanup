// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketID.h"
#include "Blueprint/UserWidget.h"
#include "PauseFriendListWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UPauseFriendListWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void RequestInviteFriend();

	void SetFriendStateImage(EUserState State);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UTextBlock> NickNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UImage> OnlineStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UImage> OfflineStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UImage> IngameStateImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> InviteFriendButton;
};
