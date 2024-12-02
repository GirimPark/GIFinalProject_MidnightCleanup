// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "MaskObject.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AMaskObject : public ABasicInteractionObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

	FVector TargetLocation;
	FRotator TargetRotator;

	UPROPERTY(Replicated)
	class APlayerCharacter* AttachPlayer;

	UPROPERTY(ReplicatedUsing = OnRep_AttachToPlayer)
	AMaskObject* FPSMask;

	UPROPERTY(Replicated)
	bool bActivate=false;

	UPROPERTY(Replicated)
	bool bAttach = false;
public:
	AMaskObject();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void InterAction(APawn* Character)override;

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	// Network
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnRep_AttachToPlayer();

	UFUNCTION(Server,Reliable)
	void C2S_AttachToPlayer(class APlayerCharacter* Player);
	void C2S_AttachToPlayer_Implementation(class APlayerCharacter* Player);



	UFUNCTION(NetMulticast,Reliable)
	void S2A_AttachToPlayer(class APlayerCharacter* Player);
	void S2A_AttachToPlayer_Implementation(class APlayerCharacter* Player);


	UFUNCTION(NetMulticast,Reliable)
	void S2A_SetTargetLocationAndRotation(FVector Location,FRotator Roatation);
	void S2A_SetTargetLocationAndRotation_Implementation(FVector Location, FRotator Roatation);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_EnableEffect();
	void S2A_EnableEffect_Implementation();
};
