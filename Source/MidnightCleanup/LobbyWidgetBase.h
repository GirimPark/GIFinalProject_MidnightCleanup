// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ULobbyWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lobby")
	TObjectPtr<class UButton> StartBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lobby")
	TObjectPtr<class UScrollBox> ChatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Lobby")
	TObjectPtr<class UEditableTextBox> InputChatBox;

	UFUNCTION()
	void OnStartButton();
	 
	UFUNCTION()
	void OnCommitChat(const FText& Text, ETextCommit::Type CommitMethod);

	void ShowStartButton();

	void AddMessage(const FText& Text);	
};
