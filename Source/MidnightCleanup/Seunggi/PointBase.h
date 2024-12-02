// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointBase.generated.h"

UENUM(BlueprintType)
enum class EPointType : uint8 {
	Spawn = 0 UMETA(DisplayName = "Spawn"),
	RunAway = 1  UMETA(DisplayName = "RunAway"),
	
};


UCLASS()
class MIDNIGHTCLEANUP_API APointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APointBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	EPointType PointType;
};
