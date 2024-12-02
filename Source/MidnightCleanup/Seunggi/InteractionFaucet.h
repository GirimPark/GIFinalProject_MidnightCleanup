// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "InteractionFaucet.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionFaucet : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	AInteractionFaucet();

	virtual void BeginPlay()override;

	virtual void InterAction(APawn* Character) override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	UFUNCTION(Netmulticast,Reliable)
	void S2A_FillBucket(class AInteractionBucket* Bucket);
	void S2A_FillBucket_Implementation(class AInteractionBucket* Bucket);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* Hologram;
};
