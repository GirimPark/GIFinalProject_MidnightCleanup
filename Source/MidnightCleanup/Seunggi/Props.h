// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionPickUpObject.h"
#include "Props.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AProps : public AInteractionPickUpObject
{
	GENERATED_BODY()
	
public:
	AProps();

	virtual void BeginPlay() override;
	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void DrawOutline(bool Draw) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<class AObjectOrganize> OriginalLocation;
};
