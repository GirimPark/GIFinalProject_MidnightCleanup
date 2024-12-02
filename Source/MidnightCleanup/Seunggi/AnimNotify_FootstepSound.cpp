// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FootstepSound.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound\SoundCue.h"

void UAnimNotify_FootstepSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	FVector StartPosition = MeshComp->GetSocketLocation(BoneName);
	FVector EndPosition = StartPosition - FVector(0, 0, 30);

	TArray<TEnumAsByte<EObjectTypeQuery>> Objects;

	Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	FHitResult OutHit;
	TArray<AActor*> ActorsToIgnore;
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), StartPosition, EndPosition, Objects, true, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);
	if (OutHit.GetComponent())
	{
		UPhysicalMaterial* PhysMaterial = OutHit.PhysMaterial.Get();
		if (PhysMaterial)
		{
			switch (PhysMaterial->SurfaceType)
			{
			case EPhysicalSurface::SurfaceType1:
			{
				if(SoundCueList.Num()>0)
				{
					USoundCue* Sound = *SoundCueList.Find("Ground");
					if (Sound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, OutHit.ImpactPoint);
					}
				}
			}
			break;
			case EPhysicalSurface::SurfaceType2:
			{
				if (SoundCueList.Num() > 0)
				{
					USoundCue* Sound = *SoundCueList.Find("Wood");
					if (Sound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, OutHit.ImpactPoint);
					}
				}
			}
			break;
			case EPhysicalSurface::SurfaceType3:
			{
				if(SoundCueList.Num() > 0)
				{
					USoundCue* Sound = *SoundCueList.Find("Metal");
					if (Sound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, OutHit.ImpactPoint);
					}
				}
			}
			break;
			case EPhysicalSurface::SurfaceType4:
			{
				if (SoundCueList.Num() > 0)
				{
					USoundCue* Sound = *SoundCueList.Find("Tatami");
					if (Sound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, OutHit.ImpactPoint);
					}
				}
			}
			break;
			}
		
		}
		
	}
}
