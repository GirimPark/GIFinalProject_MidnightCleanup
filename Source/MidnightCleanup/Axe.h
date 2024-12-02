// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTool.h"
#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AAxe : public ABasicTool
{
	GENERATED_BODY()

public:
	AAxe();
	
	UFUNCTION(NetMulticast, Reliable)
	void S2A_EnableEffect();
	void S2A_EnableEffect_Implementation();
};
