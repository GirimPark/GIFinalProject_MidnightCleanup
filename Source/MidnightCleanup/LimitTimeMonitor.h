// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionMonitor.h"
#include "LimitTimeMonitor.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ALimitTimeMonitor : public AInteractionMonitor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULimitTimeBase> LimitTimeWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class ULimitTimeBase> LimitTimeWidget;

	void UpdatePlayTime();
};
