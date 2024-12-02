// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "StoveObject.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AStoveObject : public ABasicInteractionObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Replicated)
	uint8 bActivate : 1;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class APotObject* Pot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StayTime=5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASpawnPoint* DecalSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATextureDecal> DecalClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ImpulsePower;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ThrowVector;


	class AOverlappedTrigger* OverlappedTrigger;
public:
	virtual void InterAction(APawn* Character)override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void C2S_StopEvent();
	void C2S_StopEvent_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_StopEvent();
	void S2A_StopEvent_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_StartEvent();
	void C2S_StartEvent_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_StartEvent();
	void S2A_StartEvent_Implementation();

	void CreateDecal();
};
