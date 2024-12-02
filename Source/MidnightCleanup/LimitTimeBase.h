// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LimitTimeBase.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ULimitTimeBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="LimitTime")
	TObjectPtr<class UTextBlock> PlayTimeBox;

	UFUNCTION()
	void UpdatePlayTime(int32 InSeconds);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFontSize(float InSize);
};
