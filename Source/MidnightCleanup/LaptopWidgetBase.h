// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LaptopWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ULaptopWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void EnableLoadingScreen();

	UFUNCTION()
	void OnLaptopWidgetShowed(ESlateVisibility NewVisibility);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayInitSound();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laptop")
	TObjectPtr<class URequestSelectionBase> RequestSelectionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Laptop")
	TObjectPtr<class ULoadingWidgetBase> LoadingBox;

private:
	TObjectPtr<class UWidgetSwitcher> WidgetSwitcher;
};
