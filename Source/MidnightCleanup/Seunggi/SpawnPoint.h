// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointBase.h"
#include "SpawnPoint.generated.h"

UENUM(BlueprintType)
enum class ESpawnType : uint8 {
	Event = 0 UMETA(DisplayName = "Event"),
	Ghost = 1  UMETA(DisplayName = "Ghost"),

};


UCLASS()
class MIDNIGHTCLEANUP_API ASpawnPoint : public APointBase
{
	GENERATED_BODY()
public:
	ASpawnPoint();

public:
	ESpawnType SpawnType;
};
