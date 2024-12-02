// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionPickUpObject.h"
#include "InteractionWoodenPlank.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionWoodenPlank : public AInteractionPickUpObject
{
	GENERATED_BODY()
public:
	AInteractionWoodenPlank();
	virtual void BeginPlay()override;

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

};
