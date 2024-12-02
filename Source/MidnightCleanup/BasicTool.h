// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/InteractionPickUpObject.h"
#include "BasicTool.generated.h"

UENUM(BlueprintType)
enum class EToolType : uint8
{
	ToolType_Mop = 0 UMETA(DisplayName = "Mop"),
	ToolType_Flashlight = 1 UMETA(DisplayName = "Flashlight"),
	ToolType_Axe = 2 UMETA(DisplayName = "Axe"),
	ToolType_Sponge = 3 UMETA(DisplayName = "Sponge"),
	ToolType_None = 4 UMETA(DisplayName = "None")
};

UCLASS()
class MIDNIGHTCLEANUP_API ABasicTool : public AInteractionPickUpObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicTool();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	int32 Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 MaxDurability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 Reduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EToolType ToolType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	uint8 bIsActivate : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetachDistance = 50;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void DropPreviousTool(class APlayerCharacter* Player);
	virtual void DetachPreviousSocket(class APlayerCharacter* Player);

	virtual void SpawnTool()PURE_VIRTUAL(ABasicTool::SpawnTool);
	UFUNCTION(BlueprintCallable)
	virtual FString GetName();

	virtual void SetName(FString InName);

	virtual EToolType GetToolType();
	
	virtual void SetToolType(EToolType InToolType);

	virtual void VisibleObject(bool State) override;

	virtual void PickUpObject(class APawn* Character) override;

	virtual void DropProcess(APawn* Character) override;

	void PickUpTool(APawn* Character);

	virtual void UpdateToolTip(class APlayerCharacter* Player) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void S2A_VisibleTool(bool State);
	virtual void S2A_VisibleTool_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	virtual void S2A_DropPreviousTool(class APlayerCharacter* Player);
	virtual void S2A_DropPreviousTool_Implementation(class APlayerCharacter* Player);

	UFUNCTION(NetMulticast, Reliable)
	virtual void S2A_DetachTool(class APlayerCharacter* Player);
	virtual void S2A_DetachTool_Implementation(class APlayerCharacter* Player);

	UFUNCTION(NetMulticast, Reliable)
	virtual void S2A_AttachPreviousTool(class APlayerCharacter* Player);
	virtual void S2A_AttachPreviousTool_Implementation(class APlayerCharacter* Player);

	virtual void PickUpFPSSponge(class APlayerCharacter* Player);

	virtual void DropSponge(class APlayerCharacter* Player);
};
