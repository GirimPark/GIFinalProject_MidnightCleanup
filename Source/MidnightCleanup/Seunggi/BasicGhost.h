// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEntity.h"
#include "BasicGhost.generated.h"

UENUM(BlueprintType)
enum class EGhostState : uint8
{
	Patrol = 0	UMETA(Display = "Patrol"),
	Steal = 1	UMETA(Display = "Steal"),
	Run = 2	UMETA(Display = "Run"),
	Dead = 3	UMETA(Display = "Dead"),
};

UENUM(BlueprintType)
enum class EGhostType : uint8 {
	Trash = 0 UMETA(DisplayName = "Trash"),
	Light = 1  UMETA(DisplayName = "Light"),
	Door = 2     UMETA(DisplayName = "Door"),
};

UCLASS()
class MIDNIGHTCLEANUP_API ABasicGhost : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UPhysicsHandleComponent> PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGhostType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EGhostState State;

	class USplineComponent* PatrolPoints;
	uint32 PatrolIndex=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MoveLocation;

	TArray<AActor*> DetectedObject;

	TArray<class ABasicInteractionObject*> CanStealObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMax = 100, ClampMin = 0), Category = "Probability")
	int32 Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMax = 100, ClampMin = 0), Category = "Probability")
	int32 BoxProbability;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCurveFloat* Curve;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PowerLevel;

public:
	UFUNCTION()
	void ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool CanInteraction();

	bool CanBoxSteal();
};
