// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipmentSelectionWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UEquipmentSelectionWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnReturnButtonClicked();

private:
	TObjectPtr<class UTextBlock> NicknameText;
	TObjectPtr<class UTextBlock> CashText;

	TObjectPtr<class UButton> ReturnButton;
};
