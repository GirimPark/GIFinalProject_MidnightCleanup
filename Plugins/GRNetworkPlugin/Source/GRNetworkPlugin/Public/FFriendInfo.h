// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PacketID.h"
#include "FFriendInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GRNETWORKPLUGIN_API FFriendInfo
{
	GENERATED_BODY()

	FString Name;
	EUserState State;

	FORCEINLINE bool operator==(const FString& _Name)
	{
		return Name == _Name;
	}
};