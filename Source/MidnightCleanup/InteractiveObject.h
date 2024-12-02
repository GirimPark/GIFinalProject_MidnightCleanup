// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "InteractiveObject.generated.h"

UCLASS()
class MIDNIGHTCLEANUP_API AInteractiveObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Interactive Object")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Interactive Object")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Interactive Object", Replicated)
	TObjectPtr<class UMaterialInterface> MaterialInterface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_Overlap)
	uint8 bIsOverlap : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Interactive Object", ReplicatedUsing = OnRep_Activate)
	uint8 bIsActivate : 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	

	UFUNCTION()
	void OnRep_Activate();

	UFUNCTION()
	void OnRep_Overlap();

	UFUNCTION()
	void ProcessBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProcessEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void ActivateSequence() PURE_VIRTUAL(AInteractiveObject::ActivateSequence, );

	virtual void OverlapSequence() PURE_VIRTUAL(AInteractiveObject::OverlapSequence, );

	UFUNCTION(Server, Reliable)
	void C2S_SetOverlap(bool State);
	void C2S_SetOverlap_Implementation(bool State);

	UFUNCTION(Client, Reliable)
	void S2C_SetOverlap(bool State);
	void S2C_SetOverlap_Implementation(bool State);
};
