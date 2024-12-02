// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Seunggi/InteractionPickUpObject.h"
#include "Net/UnrealNetwork.h"
#include "InteractionBox.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AInteractionBox : public AInteractionPickUpObject
{
	GENERATED_BODY()
	
public:
	AInteractionBox();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* Box;

	TArray<TObjectPtr<class AInteractionTrash>> Trashes;

	UPROPERTY(BlueprintReadOnly, Replicated)
	uint8 bIsOverlap : 1;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 TrashNum;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* CloseBox;

	class APlayerCharacter* Owner;


public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)override;

	virtual void Destroyed()override;

	void SetBoxSize(int32 InSize);

	void AddTrash(AInteractionTrash* InTrash);
	void RemoveTrash(AInteractionTrash* InTrash);

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void PickUpTrash(APawn* Character) override;
	virtual void DropTrash(APawn* Character) override;

	virtual void PickUpObject(class APawn* Character);

	virtual void DropProcess(APawn* Character);
};
