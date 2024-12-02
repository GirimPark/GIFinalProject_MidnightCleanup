// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UTitleWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Base
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TObjectPtr<class UButton> GameStartBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TObjectPtr<class UButton> SettingBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TObjectPtr<class UButton> ExitBox;

	UFUNCTION()
	void OnGameStartButton();

	UFUNCTION()
	void OnSettingButton();

	UFUNCTION()
	void OnExitButton();

	// Game Start
	FName IpAddress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UCanvasPanel> GameStartPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UCanvasPanel> InputAddressPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UEditableTextBox> InputNameBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UEditableTextBox> InputAddressBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UButton> ServerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UButton> ConnectBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Start")
	TObjectPtr<class UButton> NextBox;

	UFUNCTION()
	void OnCommitName(const FText& Text);

	UFUNCTION()
	void OnEnterName(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnCommitAddress(const FText& Text);

	UFUNCTION()
	void OnEnterAddress(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnServerButton();

	UFUNCTION()
	void OnConnectButton();

	UFUNCTION()
	void OnNextButton();

	UFUNCTION()
	void SaveUserID();

	void ShowServerButton();

	void HideServerButton();
};
