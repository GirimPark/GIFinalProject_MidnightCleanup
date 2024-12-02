// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/InteractionPickUpObject.h"
#include "InteractionBucket.generated.h"

UENUM(BlueprintType)
enum class EBucketState : uint8
{
	BucketState_Clean = 0 UMETA(DisplayName = "Clean"),
	BucketState_Normal = 1 UMETA(DisplayName = "Normal"),
	BucketState_Dirty = 2 UMETA(DisplayName = "Dirty")
};

UCLASS()
class MIDNIGHTCLEANUP_API AInteractionBucket : public AInteractionPickUpObject
{
	GENERATED_BODY()

public:
	AInteractionBucket();

	EBucketState BucketState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,ReplicatedUsing = OnRep_ChangeState)
	int32 Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDurability=100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (ClampMin = 1))
	int32 SpongeDecrease=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NormalDurability=60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DirtyDurability=30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* ClaenMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* NormalMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInterface* DirtyMaterial;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* WaterPlane;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* SpoilDetector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATextureDecal> DecalClass;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	virtual void Tick(float DeltaTime)override;

	virtual void Destroyed()override;

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	virtual void PickUpObject(class APawn* Character);

	virtual void DropProcess(APawn* Character);

	void Cleaning(APawn* Character);

	void CreateDecal(FVector CreatePoint, FRotator CreateRotator);

	UFUNCTION()
	void OnRep_ChangeState();

	UFUNCTION()
	void ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bIsFilled:1;

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetAndEnableEffect(class UNiagaraSystem* System);
	void S2A_SetAndEnableEffect_Implementation(class UNiagaraSystem* System);

	UFUNCTION(BlueprintImplementableEvent)
	void EnableSpoilEffect();
};
