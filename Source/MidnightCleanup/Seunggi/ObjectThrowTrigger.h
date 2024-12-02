// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "ObjectThrowTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AObjectThrowTrigger : public ABasicEntity
{
	GENERATED_BODY()
	
public:
	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void TriggerEvent();

public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float ImpulsePower;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ThrowVector;
};
