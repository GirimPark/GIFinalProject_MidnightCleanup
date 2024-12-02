// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/BasicInteractionObject.h"
#include "InteractionKeypad.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionKeypad : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	virtual void InterAction(APawn* Character) override;
	virtual void DrawOutline(bool Draw) override;
	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_ShowRewardWidget(APlayerCharacter* Player);
	void S2A_ShowRewardWidget_Implementation(APlayerCharacter* Player);
};
