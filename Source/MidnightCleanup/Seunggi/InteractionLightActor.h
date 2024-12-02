// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionLightActor.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API AInteractionLightActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInteractionLightActor();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetLightComponent(ULightComponentBase* LightComponent);

	UFUNCTION(BlueprintCallable)
	void Initialize();

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class ULightComponentBase* LightComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class AInteractionSwitch* Switch;

	float LightIntensity;
};
