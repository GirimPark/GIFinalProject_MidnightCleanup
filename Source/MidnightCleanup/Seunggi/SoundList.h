// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundList.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ASoundList : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Audio")
	TMap<FString ,class USoundCue*> SoundCues;

};
