// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/BasicInteractionObject.h"
#include "InteractionComputer.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionComputer : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	AInteractionComputer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TObjectPtr<class USplineComponent> CameraMovementPath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsActivate : 1 = false;

	virtual void InterAction(APawn* Character) override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetComputerActivateState(bool State);
	void S2A_SetComputerActivateState_Implementation(bool State);
};
