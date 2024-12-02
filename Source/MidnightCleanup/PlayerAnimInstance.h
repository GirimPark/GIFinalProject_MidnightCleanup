// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsJump : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsMove : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsInteractive : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsMopHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bIsPrimaryMopHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsAxeHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bIsPrimaryAxeHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsFlashlightHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsSpongeHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bIsPrimarySpongeHeld : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsLeftClickStart : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsLeftClicking : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsLeftClickOnGoing : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Stat")
	uint8 bIsAnimating : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bIsErasing : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bIsEraseStart : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bFocusFurniture : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bWashSponge : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 bDisableInteraction : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AimPitch;
};
