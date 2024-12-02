// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "InteractionDoor.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionDoor : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	AInteractionDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ClosedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OpenedPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsOpened : 1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;


	UPROPERTY()
	float DoorSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* EventSound;

	UPROPERTY()
	UAudioComponent* EventSoundAudio;

	virtual void InterAction(APawn* Player) override;
	virtual void DrawOutline(bool Draw) override;
	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void DoorMove();
	void DoorMove_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void SetDoorSpeed(float Speed);
	void SetDoorSpeed_Implementation(float Speed);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_EventSoundPlay();
	void S2A_EventSoundPlay_Implementation();
};
