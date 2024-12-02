// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "OverlappedTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AOverlappedTrigger : public ABasicEntity
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Destroyed() override;

	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	APlayerCharacter* PlayerCharacter=nullptr;
};
