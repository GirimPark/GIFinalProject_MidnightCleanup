// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "InteractionSwitch.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionSwitch : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* Switch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AInteractionLightActor*> Lights;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Replicated)
	uint8 bIsLighting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TurnOnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TurnOffRotation;

	AInteractionSwitch();

	virtual void InterAction(APawn* Character) override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime)override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(NetMulticast, Reliable)
	void SetIntensity(float Value, AInteractionLightActor* LightActor);
	void SetIntensity_Implementation(float Value, AInteractionLightActor* LightActor);

	UFUNCTION(NetMulticast, Reliable)
	void SetLightingState(bool State);
	void SetLightingState_Implementation(bool State);

	UFUNCTION(BlueprintCallable)
	void Initialize();
};
