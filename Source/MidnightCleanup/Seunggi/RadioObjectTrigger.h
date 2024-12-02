// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "RadioObjectTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ARadioObjectTrigger : public ABasicEntity
{
	GENERATED_BODY()
	
public:
	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASpawnPoint* TriggerSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AOverlappedTrigger> TriggerClass;

};
