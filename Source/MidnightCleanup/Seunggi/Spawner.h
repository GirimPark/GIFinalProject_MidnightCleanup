// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "Spawner.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API ASpawner : public ABasicInteractionObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InterAction(APawn* Character)override;

	virtual void DrawOutline(bool Draw)override;

	virtual void UpdateToolTip(APlayerCharacter* Player) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AInteractionPickUpObject> SpawnObject;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LimitNum=0;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 CurrentObjectNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnPoint=FVector(0,0,70);
};
