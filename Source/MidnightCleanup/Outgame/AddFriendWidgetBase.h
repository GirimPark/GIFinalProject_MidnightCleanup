// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketID.h"
#include "Blueprint/UserWidget.h"
#include "AddFriendWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UAddFriendWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetImageFromFriendState(EUserState UserState);

	void SetButtonFromRequestState(ERequestState RequestState);

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnAddFriendRespond(bool Success);

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnCancelAddFriendRespond(bool Success);

	UFUNCTION(BlueprintCallable, Category = "Event")
	void OnAcceptFriendRespond();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UTextBlock> NickNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UTextBlock> FriendStateText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> AddFriendButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> PendingFriendButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	FLinearColor OnlineStateTintColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	FLinearColor InGameStateTintColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property")
	FLinearColor OfflineStateTintColor;
};
