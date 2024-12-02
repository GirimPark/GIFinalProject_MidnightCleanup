// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UPauseWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UButton> ReturnBox;

	UPROPERTY()
	TObjectPtr<class UButton> FriendInviteBox;

	UPROPERTY()
	TObjectPtr<class UButton> SettingBox;

	UPROPERTY()
	TObjectPtr<class UButton> QuitBox;

	UPROPERTY()
	TObjectPtr<class UFriendInvitationWidgetBase> FriendInvitationWidget;

	UPROPERTY()
	TObjectPtr<class USettingWidgetBase> SettingWidget;

	UPROPERTY()
	TObjectPtr<class UTextBlock> TempBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	FString LevelNameToQuit = "L_OutGame";

	UFUNCTION()
	void ReturnInGame();

	UFUNCTION()
	void ShowFriendInvitationWidget();

	UFUNCTION()
	void ShowSettingWidget();

	UFUNCTION()
	void OnQuitButtonClicked();

	void OnPopupWidgetClosed();

	UFUNCTION()
	void OnQuitRoomRespond();

	UFUNCTION()
	void OnQuitConfirmButtonClicked();

	UFUNCTION()
	void OnQuitCancelButtonClicked();

	void DisableFriendInviteButton();

	void HidePopupWidget();

private:
	TObjectPtr<class UCanvasPanel> QuitAlertCanvas;
	TObjectPtr<class UButton> QuitConfirmButton;
	TObjectPtr<class UButton> QuitCancelButton;
};
