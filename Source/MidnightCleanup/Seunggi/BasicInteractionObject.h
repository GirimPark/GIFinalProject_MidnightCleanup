// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicInteractionObject.generated.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	ObjectType_PickUp = 0 UMETA(DisplayName = "PickUp"),
	ObjectType_Static = 1 UMETA(DisplayName = "Static"),
	ObjectType_Furniture = 2 UMETA(DisplayName = "Furniture"),
};

UCLASS()
class MIDNIGHTCLEANUP_API ABasicInteractionObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicInteractionObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InterAction(APawn* Character) PURE_VIRTUAL(ABasicInteractionObject::InterAction, );

	virtual void DrawOutline(bool Draw) PURE_VIRTUAL(ABasicInteractionObject::DrawOutline);

	virtual void UpdateToolTip(class APlayerCharacter* Player) PURE_VIRTUAL(ABasicInteractionObject::UpdateToolTip);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SoundPlay();
	void S2A_SoundPlay_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SoundStop();
	void S2A_SoundStop_Implementation();
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	EObjectType ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* ObjectSound;

	UPROPERTY()
	UAudioComponent* ObjectSoundAudio;

	UFUNCTION(BlueprintImplementableEvent)
	void EnableEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableEffect();

	UFUNCTION(BlueprintImplementableEvent)
	void SetEffect(class UNiagaraSystem* NiagaraSystem);
};
