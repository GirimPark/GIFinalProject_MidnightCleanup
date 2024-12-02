// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTool.h"
#include "Mop.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API AMop : public ABasicTool
{
	GENERATED_BODY()

public:
	AMop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 EraseCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 CleanDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 DirtyDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FVector DecalSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATextureDecal> DecalClass;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstance* HeadMI = nullptr;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstance* StickMI = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* SwingSound;

	UPROPERTY()
	UAudioComponent* SwingSoundAudio;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SpawnTool()override;

	virtual void DrawOutline(bool Draw) override;

	void SetCollisionType(const FName& InType);

	UFUNCTION(Server, Reliable)
	void C2S_DecreaseDurability(class ATextureDecal* InDecal, APawn* Character);
	void C2S_DecreaseDurability_Implementation(class ATextureDecal* InDecal, APawn* Character);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetMopDurability(int32 InDurability);
	void S2A_SetMopDurability_Implementation(int32 InDurability);

	void CreateDecal(FVector ImpactPoint, FVector Normal);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetMopMaterial(int32 InDurability);
	void S2A_SetMopMaterial_Implementation(int32 InDurability);

	UFUNCTION(NetMulticast,UnReliable)
	void S2A_SwingSoundPlay();
	void S2A_SwingSoundPlay_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetFPSMop(AActor* InPlayer, int32 InDurability, int32 InMaxDurability, int32 InCleanDurability, int32 InDirtyDurability);
	void S2A_SetFPSMop_Implementation(AActor* InPlayer, int32 InDurability, int32 InMaxDurability, int32 InCleanDurability, int32 InDirtyDurability);

	void SetMopMaterial(int32 InDurability);
};
