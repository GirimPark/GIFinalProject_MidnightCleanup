// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionDecal.h"
#include "OpacityDecal.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AOpacityDecal : public AInteractionDecal
{
	GENERATED_BODY()
	
public:
	AOpacityDecal();

	virtual void BeginPlay()override;
	virtual void Destroyed()override;
	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	float CurrentOpacity;

	UPROPERTY(Replicated)
	uint8 bWasInteractive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float Reduction;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetOpacity(float Opacity);
	void S2A_SetOpacity_Implementation(float Opacity);

	virtual void DrawOutline(bool Draw) override;
	virtual void HintDraw(bool Draw) override;
};
