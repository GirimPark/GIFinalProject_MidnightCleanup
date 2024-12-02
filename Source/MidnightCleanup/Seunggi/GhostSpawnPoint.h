// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnPoint.h"
#include "GhostSpawnPoint.generated.h"

UENUM(BlueprintType)
enum class EGhostSpawnType : uint8 {
	Trash = 0 UMETA(DisplayName = "Trash"),
	Light = 1  UMETA(DisplayName = "Light"),
	Door = 2     UMETA(DisplayName = "Door"),
};

UCLASS()
class MIDNIGHTCLEANUP_API AGhostSpawnPoint : public ASpawnPoint
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGhostSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	class USplineComponent* PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGhostSpawnType GhostSpawnType = EGhostSpawnType::Trash;
	
};
