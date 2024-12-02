// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicInteractionObject.h"
#include "InteractionPickUpObject.generated.h"

UENUM(BlueprintType)
enum class EPickUpObjectType : uint8
{
	PickUpObjectType_Tool = 0 UMETA(DisplayName = "Tool"),
	PickUpObjectType_Trash = 1 UMETA(DisplayName = "Trash"),
	PickUpObjectType_Box = 2 UMETA(DisplayName = "Box"),
	PickUpObjectType_Furniture = 3 UMETA(DisplayName = "Furniture"),
	PickUpObjectType_Bucket = 4 UMETA(DisplayName = "Bucket"),
	PickUpObjectType_Props = 5 UMETA(DisplayName = "Props"),
	PickUpObjectType_WoodenPlank = 6 UMETA(DisplayName = "WoodenPlank")

};

UCLASS()
class MIDNIGHTCLEANUP_API AInteractionPickUpObject : public ABasicInteractionObject
{
	GENERATED_BODY()

public:
	AInteractionPickUpObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPickUpObjectType PickUpObjectType;

	virtual void BeginPlay() override;

	virtual void InterAction(APawn* Character) override;
	
	virtual void DropObject(APawn* Character);

	virtual void DropPreviousObject(APawn* Character);

	virtual void DropProcess(APawn* Character);

	virtual void DrawOutline(bool Draw) override;

	virtual void UpdateToolTip(APlayerCharacter* Player) override;

	UFUNCTION(NetMulticast, Reliable)
	void PickUp(APawn* Character);
	void PickUp_Implementation(APawn* Character);

	virtual void PickUpTrash(APawn* Character) PURE_VIRTUAL(AInteractionPickUpObject::PickUpTrash, );

	virtual void DropTrash(APawn* Character) PURE_VIRTUAL(AInteractionPickUpObject::DropTrash, );

	virtual void SetTransform() PURE_VIRTUAL(AInteractionPickUpObject::SetTransform, );

	virtual void VisibleObject(bool State) PURE_VIRTUAL(AInteractionPickUpObject::VisibleObject, );

	UFUNCTION(NetMulticast, Reliable)
	 void Drop(APawn* Character);
	 void Drop_Implementation(APawn* Character);

	 UFUNCTION(NetMulticast, Reliable)
	 void DropPrevious(APawn* Character);
	 void DropPrevious_Implementation(APawn* Character);

	virtual void PickUpObject(class APawn* Character);
	//void DropObject(class APawn* Character);

	void PickUpTool(class APawn* Character);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	AActor* ObjectOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	AActor* LastOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	AInteractionPickUpObject* DuplicateActor;

	void SetObjectOwner(APawn* Character);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetObjectOwner(APawn* Character);
	void S2A_SetObjectOwner_Implementation(APawn* Character);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetDuplicateActor(AInteractionPickUpObject* Actor);
	void S2A_SetDuplicateActor_Implementation(AInteractionPickUpObject* Actor);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SetOverlapEvents(bool State);
	void S2A_SetOverlapEvents_Implementation(bool State);

	UFUNCTION(NetMulticast, Reliable)
	void S2A_PlayDropSound();
	void S2A_PlayDropSound_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void BoxBeginHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit);

	UFUNCTION()
	void OnRep_SetCleaningState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* Sphere;

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing ="OnRep_SetCleaningState")
	uint8 bIsCleaning : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* DropSound;

	UPROPERTY()
	UAudioComponent* DropSoundAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* StealSound;

	UPROPERTY()
	UAudioComponent* StealSoundAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PhysicsHandleDistance=120;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GrabLocation=FVector(0,0,35);
};
