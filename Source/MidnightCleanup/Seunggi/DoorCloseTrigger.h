// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "DoorCloseTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ADoorCloseTrigger : public ABasicEntity
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ABasicInteractionObject* TargetObject2;

	
	UPROPERTY(Replicated)
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StayTime;
public:
	virtual void BeginPlay()override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(Server,Reliable)
	void C2S_CloseDoor(class APlayerCharacter* Player);
	void C2S_CloseDoor_Implementation(class APlayerCharacter* Player);

	void TriggerStay(class APlayerCharacter* Player);
};
