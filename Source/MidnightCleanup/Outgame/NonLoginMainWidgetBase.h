// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NonLoginMainWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UNonLoginMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateRoomButtonClicked();

	UFUNCTION()
	void OnJoinRoomButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();

	UFUNCTION()
	void OnJoinRoomCheckButtonClicked();

	UFUNCTION()
	void OnIpAddressTextCommitted(const FText& Text, ETextCommit::Type CommitType);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> CreateRoomButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> JoinRoomButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> SettingButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UCanvasPanel> JoinRoomCanvas;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UButton> JoinRoomCheckButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	TObjectPtr<class UEditableTextBox> IpAddressText;

private:
	class USettingWidgetBase* SettingWidget;
};
