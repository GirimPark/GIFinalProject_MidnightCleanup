// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/BasicInteractionObject.h"
#include "InteractionMonitor.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API AInteractionMonitor : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionMonitor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> Widget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InterAction(APawn* Character) override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
