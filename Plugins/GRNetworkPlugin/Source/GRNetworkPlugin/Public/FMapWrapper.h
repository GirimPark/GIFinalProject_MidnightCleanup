// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketID.h"
#include "FMapWrapper.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GRNETWORKPLUGIN_API FMapWrapper
{
	GENERATED_BODY()

	TMap<FString, EUserState> Map;
};
