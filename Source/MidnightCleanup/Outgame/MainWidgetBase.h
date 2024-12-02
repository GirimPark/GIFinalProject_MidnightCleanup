// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnRoomCreated();

	UFUNCTION(BlueprintCallable)
	void OnRoomJoined(FString IpAddress);

	UFUNCTION()
	void OnFriendManagementButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();

	UFUNCTION()
	void OnJoinRoomRespond(int32 ResultCode, FString IpAddress);

	UFUNCTION()
	void SetVisibleFriendAlert();

	UFUNCTION()
	void SetCollapsedFriendAlert();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnCreditButtonClicked();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FString CreatedRoomLevelName;

private:
	TObjectPtr<class UButton> FriendManagementButton;
	TObjectPtr<class UButton> SettingButton;
	TObjectPtr<class UImage> FriendAlertImage;
	TObjectPtr<class UEditableTextBox> RoomCodeText;
	TObjectPtr<class UButton> JoinRoomButton;
	TObjectPtr<class UTextBlock> JoinRoomText;
	TObjectPtr<class UTextBlock> JoinRoomResText;
	TObjectPtr<class UButton> CreditButton;

	class USettingWidgetBase* SettingWidget;
};
