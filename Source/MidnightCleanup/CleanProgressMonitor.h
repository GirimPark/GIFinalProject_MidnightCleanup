// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionMonitor.h"
#include "CleanProgressMonitor.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ACleanProgressMonitor : public AInteractionMonitor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UCleanProgressMonitorBase> CleanAreaWidgetClass;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	class UCleanProgressMonitorBase* CleanAreaWidget;

	void UpdateCount();
};
