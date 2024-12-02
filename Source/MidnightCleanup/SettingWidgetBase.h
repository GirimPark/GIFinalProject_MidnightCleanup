// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API USettingWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetParentWidget(class UPauseWidgetBase* Parent)
	{
		ParentWidget = Parent;
	}

	UFUNCTION()
	void OnReturnButtonClicked();

private:
	TObjectPtr<class UButton> ReturnButton;

	class UPauseWidgetBase* ParentWidget;
};
