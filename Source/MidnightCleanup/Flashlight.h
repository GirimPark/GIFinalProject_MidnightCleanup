// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTool.h"
#include "Flashlight.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AFlashlight : public ABasicTool
{
	GENERATED_BODY()

public:
	AFlashlight();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ULocalLightComponent> LightClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float MaxIntensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInstanceDynamic* MI;

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetFlashlightActivate(bool State);
	void S2A_SetFlashlightActivate_Implementation(bool State);

	void OnFlashlight();
	void OffFlashlight();
};
