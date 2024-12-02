// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeleteFriendAlertWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UDeleteFriendAlertWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnDeleteFriendButtonClicked();

	UFUNCTION()
	void OnReturnButtonClicked();


	void SetFriendName(FString Name) { FriendName = Name; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UTextBlock> DeleteAlertText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> DeleteFriendButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> ReturnButton;

private:
	FString FriendName;
};
