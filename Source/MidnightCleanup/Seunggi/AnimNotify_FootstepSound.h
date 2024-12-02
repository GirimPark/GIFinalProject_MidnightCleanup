// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FootstepSound.generated.h"

/**
 * 
 */
UCLASS()
class MIDNIGHTCLEANUP_API UAnimNotify_FootstepSound : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FString,class USoundCue*> SoundCueList;

	UPROPERTY(Editanywhere,BlueprintReadWrite)
	FName BoneName;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
