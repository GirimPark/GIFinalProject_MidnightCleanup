// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EventWrapper.generated.h"

/**
 * 
 */
UCLASS()
class GRNETWORKPLUGIN_API UEventWrapper : public UObject
{
	GENERATED_BODY()

public:
	void CreateEvent();

	void Trigger();

	void Wait();

private:
	FEvent* Event;
};
