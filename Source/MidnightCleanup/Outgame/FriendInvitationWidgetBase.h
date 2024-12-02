// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFriendInfo.h"
#include "Blueprint/UserWidget.h"
#include "FriendInvitationWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UFriendInvitationWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetParentWidget(class UPauseWidgetBase* Parent) { ParentWidget = Parent; }

	UFUNCTION()
	void OnReturnButtonClicked();

	UFUNCTION()
	void OnWidgetVisible(ESlateVisibility NewVisibility);

	UFUNCTION()
	void OnInviteFriendRespond(bool Success, FString InvitedUserName);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	TSubclassOf<class UPauseFriendListWidgetBase> FriendListWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float DelayTime = 5.f;

private:
	void SetFriendList(TArray<FFriendInfo> FriendList);

	void SortFriendInfoArray(TArray<FFriendInfo>& FriendInfoArray);


	TObjectPtr<class UScrollBox> FriendListScrollBox;

	TObjectPtr<class UButton> ReturnButton;

	class UPauseWidgetBase* ParentWidget;

	FTimerHandle TimerHandle;
};
