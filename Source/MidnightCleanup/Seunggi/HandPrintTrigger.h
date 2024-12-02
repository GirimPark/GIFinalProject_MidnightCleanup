// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEntity.h"
#include "HandPrintTrigger.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API AHandPrintTrigger : public ABasicEntity
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class ASpawnPoint*> HandPrintPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> HandPrintTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AOpacityDecal> LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AOpacityDecal> RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* ObjectSound;

	UPROPERTY()
	UAudioComponent* ObjectSoundAudio;

	int Index=0;

	virtual void OverlapFucntion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(Server, Reliable)
	void C2S_SpawnHandPrint();
	void C2S_SpawnHandPrint_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2A_SoundPlay(FVector Location);
	void S2A_SoundPlay_Implementation(FVector Location);
};
