// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../PlayerCharacter.h"
#include "BasicEntity.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API ABasicEntity : public AActor
{
	GENERATED_BODY()
public:
	ABasicEntity();

	virtual void BeginPlay() override;

	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProcessOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server,Reliable)
	void C2S_SetActivate(bool Activate);
	void C2S_SetActivate_Implementation(bool Activate);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetActivate(bool Activate);
	void S2A_SetActivate_Implementation(bool Activate);

	UFUNCTION(NetMulticast,Reliable)
	void S2C_TriggerEvent(APlayerCharacter* Player, bool Enabled);
	void S2C_TriggerEvent_Implementation(APlayerCharacter* Player, bool Enabled);

	UFUNCTION(NetMulticast, Reliable)
	void TriggerEnd(APlayerCharacter* Player, bool Enabled);
	void TriggerEnd_Implementation(APlayerCharacter* Player, bool Enabled);

	virtual void StoveOn();

	void TriggerEvent(APlayerCharacter* Player);
	void TriggerEventEnd(APlayerCharacter* Player);

	UFUNCTION(Client, Reliable)
	void SetSound(APlayerCharacter* Player,bool Activate);
	void SetSound_Implementation(APlayerCharacter* Player, bool Activate);

	UFUNCTION(Client, Reliable)
	void SetPPActivate(bool Activate);
	void SetPPActivate_Implementation(bool Activate);
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class ABasicInteractionObject* TargetObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	uint8 bActivate : 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* TriggerSound;

	class UAudioComponent* TriggerSoundAudio;

	UPROPERTY(EditAnywhere, Category = "PostProcess")
	class UPostProcessComponent* PostProcessComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PostProcessTime=2;
};
