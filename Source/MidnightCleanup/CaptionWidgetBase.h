// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CaptionWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UCaptionWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void SetCaptionBox(FText InText, float Speed, const FString& InSFXPath);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCaptionTextAnim(float InSpeed, class USoundBase* Sound);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void PlayTutorialTextAnim();

	TObjectPtr<class UTextBlock> CaptionBox;
};
