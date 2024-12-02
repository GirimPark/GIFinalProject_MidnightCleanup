// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "StoveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AStoveTrigger : public ABasicEntity
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void StoveOn();

	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASpawnPoint* TriggerSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AOverlappedTrigger> TriggerClass;

	class AOverlappedTrigger* OverlappedTrigger;

};

