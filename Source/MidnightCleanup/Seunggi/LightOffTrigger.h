// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "LightOffTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ALightOffTrigger : public ABasicEntity
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void TriggerStay(APawn* Player);

	UFUNCTION()
	void OnRep_AlreadyPeople();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	FTimerHandle TimerHandle;

	UPROPERTY(ReplicatedUsing="OnRep_AlreadyPeople")
	bool AlreadyInPlayer=false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float StayTime;
};
