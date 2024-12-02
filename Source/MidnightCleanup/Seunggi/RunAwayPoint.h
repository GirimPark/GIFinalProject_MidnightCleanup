// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PointBase.h"
#include "RunAwayPoint.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API ARunAwayPoint : public APointBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunAwayPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
