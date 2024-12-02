// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/BasicInteractionObject.h"
#include "InteractionFurniture.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionFurniture : public ABasicInteractionObject
{
	GENERATED_BODY()

public:
	AInteractionFurniture();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void InterAction(APawn* Character) override;

	virtual void UpdateToolTip(APlayerCharacter* Player) override;

	virtual void Destroyed()override;

	virtual void DrawOutline(bool state)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	uint8 MaxHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 SpawnTrashNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class AInteractionWoodenPlank>> Trash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bBreak : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UGeometryCollectionComponent* Geometry;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

	float OpacityValue = 100.f;

	FTimerHandle TimerHandle;

	class AFurnitureFieldSystemActor* FieldSystem;

	void CreateTrash(uint8 TrashNum);

	void Axe_Hit(APawn* Character);

	UFUNCTION(NetMulticast,Reliable)
	void S2A_BreakFurniture(float Magnitude);
	void S2A_BreakFurniture_Implementation(float Magnitude);

	void SelfDestroy();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
