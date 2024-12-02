// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionPickUpObject.h"
#include "LargePickUpObject.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API ALargePickUpObject : public AInteractionPickUpObject
{
	GENERATED_BODY()
	
public:
	class APlayerCharacter* Player1 = nullptr;
	class APlayerCharacter* Player2 = nullptr;

	virtual void InterAction(APawn* Character) override;

	virtual void DropProcess(APawn* Character) override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_PickUp(APawn* Character);
	void S2A_PickUp_Implementation(APawn* Character);

	
};
