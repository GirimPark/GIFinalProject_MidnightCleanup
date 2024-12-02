// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WoodenPlankDisposal.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API AWoodenPlankDisposal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWoodenPlankDisposal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsFullyOverlapped(class AInteractionWoodenPlank* WoodenPlank);
public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 CleaningCount=0;

	TArray<AActor*> Array;
};
