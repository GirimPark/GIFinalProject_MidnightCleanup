// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FFriendInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "FriendManagementWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UFriendManagementWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Friend")
	void SetFriendList(TArray<FFriendInfo> FriendList);

	UFUNCTION(BlueprintCallable, Category = "Friend")
	void SetPendingList(TArray<FFriendInfo> PendingList);

	UFUNCTION(BlueprintCallable, Category = "Friend")
	void OnFriendFound(bool Exist, FFriendInfo FriendInfo, int32 RequestState);

	UFUNCTION()
	void OnPendingListButtonClicked();

	UFUNCTION()
	void SetVisibleFriendAlert();

	UFUNCTION()
	void OnSearchActionTriggered();

	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

	void ShowDeleteFriendPopup(FText FriendName);

	TWeakObjectPtr<class UButton> GetFriendListButton() { return FriendListButton; }


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	TSubclassOf<class UFriendListWidgetBase> FriendListWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	TSubclassOf<class UPendingListWidgetBase> PendingListWidgetClass;


private:
	void SortFriendInfoArray(TArray<FFriendInfo>& FriendInfoArray);

	TObjectPtr<class UCanvasPanel> AddFriendCanvas;
	TObjectPtr<class UScrollBox> FriendListScrollBox;
	TObjectPtr<class UScrollBox> PendingListScrollBox;
	TObjectPtr<class UTextBlock> MyNicknameText;
	TObjectPtr<class UTextBlock> NoFriendText;
	TObjectPtr<class UTextBlock> NoPendingText;
	TObjectPtr<class UEditableTextBox> SearchingFriendNameText;
	TObjectPtr<class UTextBlock> FindFriendFailedText;
	TObjectPtr<class UButton> FriendListButton;
	TObjectPtr<class UButton> PendingListButton;
	TObjectPtr<class UButton> SearchFriendButton;
	TObjectPtr<class UImage> FriendAlertImage;

	class UAddFriendWidgetBase* AddFriendWidget;
	class UDeleteFriendAlertWidgetBase* DeleteFriendWidget;
};
