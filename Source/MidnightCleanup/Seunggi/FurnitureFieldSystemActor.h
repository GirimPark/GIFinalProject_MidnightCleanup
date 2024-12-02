// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "FurnitureFieldSystemActor.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AFurnitureFieldSystemActor : public AFieldSystemActor
{
	GENERATED_BODY()
	
public:
	AFurnitureFieldSystemActor();

	virtual void BeginPlay()override;

	class URadialFalloff* RadialFalloff;

	void ApplyField(float Magnitude);
};
